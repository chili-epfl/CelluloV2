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
 * @file CelluloBluetoothEMP.cpp
 * @brief Source of object that mass reset/shuts down all robots
 * @author Ayberk Özgür
 * @date 2016-10-06
 */

#include "CelluloBluetoothEMP.h"

#include <QTimer>

namespace Cellulo{

CelluloBluetoothEMP::CelluloBluetoothEMP(QQuickItem* parent) : QQuickItem(parent){
    continuous = false;
}

CelluloBluetoothEMP::~CelluloBluetoothEMP(){
    for(CelluloBluetooth* robot : robotsToBlast)
        if(robot != NULL){
            robot->disconnect();
            robot->disconnectFromServer();
            robot->deleteLater();
        }
}

void CelluloBluetoothEMP::setContinuous(bool continuous){
    if(this->continuous != continuous){
        this->continuous = continuous;
        emit continuousChanged();
    }
}

void CelluloBluetoothEMP::clear(){
    for(CelluloBluetooth* robot : robotsToBlast)
        if(robot != NULL){
            robot->disconnect();
            robot->disconnectFromServer();
            robot->deleteLater();
        }
    macAddrToBlast.clear();
    emit macAddrToBlastChanged();
    macAddrBlasted.clear();
    emit macAddrBlastedChanged();
}

void CelluloBluetoothEMP::resetLater(QString macAddr){
    if(inLists(macAddr))
        return;

    macAddrToBlast.append(macAddr);
    emit macAddrToBlastChanged();

    CelluloBluetooth* celluloBluetooth = new CelluloBluetooth();
    celluloBluetooth->setAutoConnect(false);
    celluloBluetooth->setMacAddr(macAddr);

    connect(celluloBluetooth, SIGNAL(connectionStatusChanged()), this, SLOT(resetIfConnected()));
    connect(celluloBluetooth, SIGNAL(connectionStatusChanged()), this, SLOT(requeueIfDisconnected()));
    connect(celluloBluetooth, SIGNAL(bootCompleted()), this, SLOT(moveToBlastedList()));

    robotsToBlast.append(celluloBluetooth);
    startConnections();
}

void CelluloBluetoothEMP::shutdownLater(QString macAddr){
    if(inLists(macAddr))
        return;

    macAddrToBlast.append(macAddr);
    emit macAddrToBlastChanged();

    CelluloBluetooth* celluloBluetooth = new CelluloBluetooth();
    celluloBluetooth->setAutoConnect(false);
    celluloBluetooth->setMacAddr(macAddr);

    connect(celluloBluetooth, SIGNAL(connectionStatusChanged()), this, SLOT(shutdownIfConnected()));
    connect(celluloBluetooth, SIGNAL(connectionStatusChanged()), this, SLOT(requeueIfDisconnected()));

    robotsToBlast.append(celluloBluetooth);
    startConnections();
}

void CelluloBluetoothEMP::moveToBlastedList(CelluloBluetooth* celluloBluetooth){
    if(celluloBluetooth == NULL)
        celluloBluetooth = (CelluloBluetooth*) QObject::sender();
    if(celluloBluetooth != NULL){
        celluloBluetooth->disconnect();
        celluloBluetooth->disconnectFromServer();

        robotsToBlast.removeAll(celluloBluetooth);

        if(macAddrToBlast.removeAll(celluloBluetooth->getMacAddr()) > 0)
            emit macAddrToBlastChanged();
        if(!continuous){
            macAddrBlasted.append(celluloBluetooth->getMacAddr());
            emit macAddrBlastedChanged();
        }

        celluloBluetooth->deleteLater();

        startConnections();
    }
}

void CelluloBluetoothEMP::resetIfConnected(){
    CelluloBluetooth* celluloBluetooth = (CelluloBluetooth*) QObject::sender();
    if(celluloBluetooth != NULL)
        if(celluloBluetooth->getConnectionStatus() == CelluloBluetoothEnums::ConnectionStatusConnected)
            celluloBluetooth->reset();
}

void CelluloBluetoothEMP::shutdownIfConnected(){
    CelluloBluetooth* celluloBluetooth = (CelluloBluetooth*) QObject::sender();
    if(celluloBluetooth != NULL)
        if(celluloBluetooth->getConnectionStatus() == CelluloBluetoothEnums::ConnectionStatusConnected){
            for(int i=0; i<5; i++)
                celluloBluetooth->shutdown();
            QTimer::singleShot(500, this, [this, celluloBluetooth] { moveToBlastedList(celluloBluetooth); });
        }
}

void CelluloBluetoothEMP::requeueIfDisconnected(){
    CelluloBluetooth* celluloBluetooth = (CelluloBluetooth*) QObject::sender();
    if(celluloBluetooth != NULL)
        if(celluloBluetooth->getConnectionStatus() == CelluloBluetoothEnums::ConnectionStatusDisconnected){
            robotsToBlast.removeAll(celluloBluetooth);
            robotsToBlast.append(celluloBluetooth);
            startConnections();
        }
}

void CelluloBluetoothEMP::startConnections(){
    for(int i=0;i<7 && i<robotsToBlast.size();i++)
        robotsToBlast[i]->connectToServer();
}

bool CelluloBluetoothEMP::inLists(QString macAddr){
    for(QString waitingAddr : macAddrToBlast)
        if(waitingAddr == macAddr)
            return true;
    for(QString waitingAddr : macAddrBlasted)
        if(waitingAddr == macAddr)
            return true;
    return false;
}

}
