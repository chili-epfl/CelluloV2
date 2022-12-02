/*
 * Copyright (C) 2016 EPFL
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see http://www.gnu.org/licenses/.
 */

/**
 * @file CelluloBluezUtil.cpp
 * @brief All things Cellulo done with the bluez c API
 * @author Ayberk Özgür
 * @date 2018-09-04
 */

#include "CelluloBluezUtil.h"

#include <unistd.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>

namespace Cellulo {

bool CelluloBluezUtil::bindToLocalAdapter(QBluetoothSocket* socket, QString const& localAdapterMacAddr){
    struct sockaddr_rc localAddrStruct;
    localAddrStruct.rc_family = AF_BLUETOOTH;
    //qDebug() << "rc_channel: " << localAddrStruct.rc_channel;
    //localAddrStruct.rc_channel = (uint8_t) 1;

    //Open new low level BTPROTO_RFCOMM socket
    int socketDescriptor = ::socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
    if(socketDescriptor < 0){
        qCritical() << "CelluloBluezUtil::bindToLocalAdapter(): socket() returned error: " << strerror(errno);
        return false;
    }

    str2ba(localAdapterMacAddr.toStdString().c_str(), &(localAddrStruct.rc_bdaddr)); //Always returns 0, does not return error

    //Bind the BTPROTO_RFCOMM socket to the local adapter
    if(bind(socketDescriptor, (struct sockaddr*)(&localAddrStruct), sizeof(localAddrStruct)) < 0){
        qCritical() << "CelluloBluezUtil::bindToLocalAdapter(): bind() returned error: " << strerror(errno);
        ::close(socketDescriptor);
        return false;
    }

    //Set the BTPROTO_RFCOMM socket as QBluetoothSocket's low level socket
    if(!socket->setSocketDescriptor(socketDescriptor, QBluetoothServiceInfo::RfcommProtocol, QBluetoothSocket::UnconnectedState)){
        qCritical() << "CelluloBluezUtil::bindToLocalAdapter(): QBluetoothSocket::setSocketDescriptor() failed.";
        ::close(socketDescriptor);
        return false;
    }

    return true;
}

bool CelluloBluezUtil::connectedOverWrongLocalAdapter(QString const& macAddr, QString const& correctLocalAdapterMacAddr, int& wrongLocalAdapterDevID){
    if(correctLocalAdapterMacAddr == "")
        return false;

    /*
     * Get all local adapters, adapted from hci_for_each_dev()
     */

    struct hci_dev_list_req* hciDevListReq;
    struct hci_dev_req* hciDevReq;

    //Open BTPROTO_HCI socket
    int btprotoHciSocket = ::socket(AF_BLUETOOTH, SOCK_RAW | SOCK_CLOEXEC, BTPROTO_HCI);
    if(btprotoHciSocket < 0){
        qCritical() << "CelluloBluezUtil::connectedOverWrongLocalAdapter(): BTPROTO_HCI socket cannot be opened.";
        return false;
    }

    //Allocate all devices request input/output struct
    hciDevListReq = (struct hci_dev_list_req*)malloc(HCI_MAX_DEV*sizeof(*hciDevReq) + sizeof(*hciDevListReq));
    if(!hciDevListReq){
        qCritical() << "CelluloBluezUtil::connectedOverWrongLocalAdapter(): hci_dev_list_req* cannot be allocated, error: " << strerror(errno);
        close(btprotoHciSocket);
        return false;
    }

    //Request parameters and result "iterator" struct
    memset(hciDevListReq, 0, HCI_MAX_DEV*sizeof(*hciDevReq) + sizeof(*hciDevListReq));
    hciDevListReq->dev_num = HCI_MAX_DEV;
    hciDevReq = hciDevListReq->dev_req;

    //Make the actual request
    if(ioctl(btprotoHciSocket, HCIGETDEVLIST, (void*)hciDevListReq) < 0){
        qCritical() << "CelluloBluezUtil::connectedOverWrongLocalAdapter(): HCIGETDEVLIST request failed, error: " << strerror(errno);
        free(hciDevListReq);
        close(btprotoHciSocket);
        return false;
    }

    char localaddrbuf[18], addrbuf[18];
    struct hci_conn_list_req* hciConnListReq;
    struct hci_conn_info* hciConnInfo;

    //Allocate all connections request input/output struct
    hciConnListReq = (struct hci_conn_list_req*)malloc(20*sizeof(*hciConnInfo) + sizeof(*hciConnListReq));
    if(!hciConnListReq){
        qCritical() << "CelluloBluezUtil::connectedOverWrongLocalAdapter(): hci_conn_list_req* cannot be allocated, error: " << strerror(errno);
        free(hciDevListReq);
        close(btprotoHciSocket);
        return false;
    }

    //Traverse all open adapters
    for(int i = 0; i < hciDevListReq->dev_num; i++, hciDevReq++){
        if(hci_test_bit(HCI_UP, &hciDevReq->dev_opt)){

            //Get device info to check the MAC address
            struct hci_dev_info hciDevInfo;
            hciDevInfo.dev_id = hciDevReq->dev_id;
            if(ioctl(btprotoHciSocket, HCIGETDEVINFO, (void*)&hciDevInfo)){
                qCritical() << "CelluloBluezUtil::connectedOverWrongLocalAdapter(): HCIGETDEVINFO request failed on device id: " << hciDevReq->dev_id << ", error: " << strerror(errno);
                continue;
            }

            //Check all local adapters with different address than the desired one
            ba2str(&hciDevInfo.bdaddr, localaddrbuf);
            if(QString(localaddrbuf).toUpper() != correctLocalAdapterMacAddr.toUpper()){

                /*
                 * Get all connections on this local adapter, adapted from hcitool::conn_list()
                 */

                //Request parameters and result "iterator" struct
                hciConnListReq->dev_id = hciDevReq->dev_id;
                hciConnListReq->conn_num = 20;
                hciConnInfo = hciConnListReq->conn_info;

                //Make the actual request
                if(ioctl(btprotoHciSocket, HCIGETCONNLIST, (void*)hciConnListReq)){
                    qCritical() << "CelluloBluezUtil::connectedOverWrongLocalAdapter(): HCIGETCONNLIST request failed, error: " << strerror(errno);
                    continue;
                }

                //Check all connections (shows all including trying to connect and connected)
                for(int j = 0; j < hciConnListReq->conn_num; j++, hciConnInfo++){

                    //Check if this robot is on this local adapter
                    ba2str(&hciConnInfo->bdaddr, addrbuf);
                    if(QString(addrbuf).toUpper() == macAddr.toUpper()){
                        wrongLocalAdapterDevID = hciDevReq->dev_id;

                        free(hciConnListReq);
                        free(hciDevListReq);
                        close(btprotoHciSocket);

                        qInfo() << "CelluloBluezUtil::connectedOverWrongLocalAdapter(): Robot " << addrbuf << " is on wrong local adapter " << localaddrbuf << "!";
                        return true;
                    }
                }
            }
        }
    }

    free(hciConnListReq);
    free(hciDevListReq);
    close(btprotoHciSocket);
    return false;
}

bool CelluloBluezUtil::disconnectFromLocalAdapter(QString const& macAddr, int localAdapterDevID){

    /*
     * Disconnect a device from a specific local adapter, adapted from hcitool::cmd_dc()
     */

    if(localAdapterDevID < 0){
        qCritical() << "CelluloBluezUtil::disconnectFromLocalAdapter(): Invalid localAdapterDevID: " << localAdapterDevID;
        return false;
    }

    //Open low level HCI device
	int devHandle = hci_open_dev(localAdapterDevID);
	if(devHandle < 0){
		qCritical() << "CelluloBluezUtil::disconnectFromLocalAdapter(): HCI device open failed, error: " << strerror(errno);
		return false;
	}

    //Allocate connection info request input/output struct
    struct hci_conn_info_req* hciConnInfoReq;
	hciConnInfoReq = (struct hci_conn_info_req*)malloc(sizeof(*hciConnInfoReq) + sizeof(struct hci_conn_info));
	if(!hciConnInfoReq){
		qCritical() << "CelluloBluezUtil::disconnectFromLocalAdapter(): hci_conn_info_req* cannot be allocated, error: " << strerror(errno);
        hci_close_dev(devHandle);
		return false;
	}

    //Make the actual connection info request to get the connection handle
    bdaddr_t bdaddr;
    str2ba(macAddr.toStdString().c_str(), &bdaddr);
	bacpy(&hciConnInfoReq->bdaddr, &bdaddr);
	hciConnInfoReq->type = ACL_LINK;
	if(ioctl(devHandle, HCIGETCONNINFO, (unsigned long)hciConnInfoReq) < 0){
		qCritical() << "CelluloBluezUtil::disconnectFromLocalAdapter(): HCIGETCONNINFO request failed, error: " << strerror(errno);
        free(hciConnInfoReq);
        hci_close_dev(devHandle);
		return false;
	}

    //Disconnect the connection
	if(hci_disconnect(devHandle, htobs(hciConnInfoReq->conn_info->handle), HCI_OE_USER_ENDED_CONNECTION, 5000) < 0){
        qCritical() << "CelluloBluezUtil::disconnectFromLocalAdapter(): hci_disconnect() failed, error: " << strerror(errno);
        if(errno == EPERM)
            qCritical() << "CelluloBluezUtil::disconnectFromLocalAdapter(): Need to run with root privileges.";
        free(hciConnInfoReq);
    	hci_close_dev(devHandle);
        return false;
    }

    free(hciConnInfoReq);
	hci_close_dev(devHandle);
    return true;
}

}
