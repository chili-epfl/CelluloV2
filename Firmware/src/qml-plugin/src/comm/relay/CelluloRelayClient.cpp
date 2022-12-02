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
 * @file CelluloRelayClient.cpp
 * @brief Relays packets between a server and virtual robot objects
 * @author Ayberk Özgür
 * @date 2016-11-22
 */

#include "CelluloRelayClient.h"

#include "../CelluloCommUtil.h"

namespace Cellulo{

CelluloRelayClient::CelluloRelayClient(CelluloCommUtil::RelayProtocol protocol, QQuickItem* parent) :
    QQuickItem(parent)
{
    lastMacAddr = "";
    currentRobot = -1;
    serverSocket = nullptr;

    this->protocol = protocol;
    switch(protocol){
        case CelluloCommUtil::RelayProtocol::Local:
            serverAddress = "cellulo_relay";
            port = -1;
            break;

        case CelluloCommUtil::RelayProtocol::Tcp:
            serverAddress = "localhost";
            port = CelluloCommUtil::DEFAULT_RELAY_PORT;
            break;
    }
    
    connect(this, SIGNAL(connected()),      this, SLOT(handleConnected()));
    connect(this, SIGNAL(disconnected()),   this, SLOT(handleDisconnected()));

    autoConnect = true;
    connect(&reconnectTimer, SIGNAL(timeout()), this, SLOT(refreshConnection()));
    reconnectTimer.setSingleShot(false);
    reconnectTimer.setInterval(SERVER_RECONNECT_TIME_MILLIS);

    connect(&heartbeatTimeoutTimer, SIGNAL(timeout()), this, SLOT(heartbeatTimedOut()));
    heartbeatTimeoutTimer.setInterval(CelluloCommUtil::RELAY_HEARTBEAT_TIMEOUT);
    heartbeatTimeoutTimer.setSingleShot(false);
    connect(this, SIGNAL(connected()),    &heartbeatTimeoutTimer, SLOT(start()));
    connect(this, SIGNAL(disconnected()), &heartbeatTimeoutTimer, SLOT(stop()));

    connect(&heartbeatTimer, SIGNAL(timeout()), this, SLOT(sendHeartbeat()));
    heartbeatTimer.setSingleShot(false);
    heartbeatTimer.setInterval(CelluloCommUtil::RELAY_HEARTBEAT_INTERVAL);
    connect(this, SIGNAL(connected()),    &heartbeatTimer, SLOT(start()));
    connect(this, SIGNAL(disconnected()), &heartbeatTimer, SLOT(stop()));

    if(autoConnect)
        connectToServer();
}

CelluloRelayClient::~CelluloRelayClient(){
    if(serverSocket){
        serverSocket->close();
        delete serverSocket;
    }
}

Cellulo::CelluloCommUtil::RelayConnectionStatus CelluloRelayClient::getConnectionStatus() const {
    if(serverSocket){
        switch(protocol){
            case CelluloCommUtil::RelayProtocol::Local:{
                switch(((QLocalSocket*)serverSocket)->state()){
                    case QLocalSocket::UnconnectedState:
                        return CelluloCommUtil::RelayConnectionStatusDisconnected;
                    case QLocalSocket::ConnectingState:
                        return CelluloCommUtil::RelayConnectionStatusConnecting;
                    case QLocalSocket::ConnectedState:
                    case QLocalSocket::ClosingState:
                        return CelluloCommUtil::RelayConnectionStatusConnected;
                    default:
                        return CelluloCommUtil::RelayConnectionStatusDisconnected;
                }
            }
            case CelluloCommUtil::RelayProtocol::Tcp:{
                switch(((QTcpSocket*)serverSocket)->state()){
                    case QAbstractSocket::UnconnectedState:
                    case QAbstractSocket::BoundState:
                        return CelluloCommUtil::RelayConnectionStatusDisconnected;
                    case QAbstractSocket::HostLookupState:
                    case QAbstractSocket::ConnectingState:
                        return CelluloCommUtil::RelayConnectionStatusConnecting;
                    case QAbstractSocket::ConnectedState:
                    case QAbstractSocket::ClosingState:
                        return CelluloCommUtil::RelayConnectionStatusConnected;
                    default:
                        return CelluloCommUtil::RelayConnectionStatusDisconnected;
                }
            }
        }
    }

    return CelluloCommUtil::RelayConnectionStatusDisconnected;
}

void CelluloRelayClient::setAutoConnect(bool autoConnect){
    if(this->autoConnect != autoConnect){
        this->autoConnect = autoConnect;
        emit autoConnectChanged();

        if(autoConnect)
            if(getConnectionStatus() == CelluloCommUtil::RelayConnectionStatusDisconnected)
                connectToServer();
    }
}

void CelluloRelayClient::refreshConnection(){
    if(getConnectionStatus() != CelluloCommUtil::RelayConnectionStatusConnected){
        qDebug() << "CelluloRelayClient::refreshConnection(): Connection attempt timed out, will retry";
        disconnectFromServer();
        connectToServer();
    }
}

void CelluloRelayClient::sendHeartbeat(){
    if(getConnectionStatus() == CelluloCommUtil::RelayConnectionStatusConnected){
        CelluloBluetoothPacket heartbeatPacket;
        heartbeatPacket.setCmdPacketType(CelluloBluetoothPacket::CmdPacketTypeHeartbeat);
        serverSocket->write(heartbeatPacket.getCmdSendData());
    }
}

void CelluloRelayClient::setServerAddress(QString serverAddress){
    if(serverAddress != this->serverAddress){
        this->serverAddress = serverAddress;
        if(getConnectionStatus() != CelluloCommUtil::RelayConnectionStatusDisconnected){
            disconnectFromServer();
            connectToServer();
        }
        emit serverAddressChanged();
    }
}

void CelluloRelayClient::setPort(int port){
    if(protocol != CelluloCommUtil::RelayProtocol::Tcp){
        qWarning() << "CelluloRelayClient::setPort(): Port only meaningful for Tcp sockets.";
        return;
    }

    if(port < 0){
        qWarning() << "CelluloRelayClient::setPort(): port given was negative, setting to 0.";
        port = 0;
    }
    else if(port > 0xFFFF){
        qWarning() << "CelluloRelayClient::setPort(): port given was larger than 65535, setting to 65535.";
        port = 0xFFFF;
    }

    if(port != this->port){
        this->port = port;
        if(getConnectionStatus() != CelluloCommUtil::RelayConnectionStatusDisconnected){
            disconnectFromServer();
            connectToServer();
        }
        emit portChanged();
    }
}

void CelluloRelayClient::connectToServer(){
    if(getConnectionStatus() == CelluloCommUtil::RelayConnectionStatusDisconnected){
        lastMacAddr = "";
        localAdapters.clear();
        emit localAdaptersChanged();

        //Should not happen
        if(serverSocket)
            serverSocket->deleteLater();

        switch(protocol){
            case CelluloCommUtil::RelayProtocol::Local:
                serverSocket = new QLocalSocket(this);
                connect((QLocalSocket*)serverSocket, static_cast<void (QLocalSocket::*)(QLocalSocket::LocalSocketError)>(&QLocalSocket::error),
                        [=](QLocalSocket::LocalSocketError error){ qDebug() << "CelluloRelayClient serverSocket error: " << error; });
                break;

            case CelluloCommUtil::RelayProtocol::Tcp:
                serverSocket = new QTcpSocket(this);
                ((QTcpSocket*)serverSocket)->setSocketOption(QAbstractSocket::LowDelayOption, 1);
                connect((QTcpSocket*)serverSocket, static_cast<void (QTcpSocket::*)(QTcpSocket::SocketError)>(&QTcpSocket::error),
                        [=](QTcpSocket::SocketError error){ qDebug() << "CelluloRelayClient serverSocket error: " << error; });
                break;
        }

        connect(serverSocket, SIGNAL(connected()),      this, SIGNAL(connected()));
        connect(serverSocket, SIGNAL(disconnected()),   this, SIGNAL(disconnected()));
        connect(serverSocket, SIGNAL(readyRead()),      this, SLOT(incomingServerData()));

        switch(protocol){
            case CelluloCommUtil::RelayProtocol::Local:
                qInfo() << "CelluloRelayClient::connectToServer(): Connecting to server at " << serverAddress << "...";
                ((QLocalSocket*)serverSocket)->connectToServer(serverAddress);
                break;

            case CelluloCommUtil::RelayProtocol::Tcp:
                qInfo() << "CelluloRelayClient::connectToServer(): Connecting to server at " << serverAddress << " via port " << port << "...";
                ((QTcpSocket*)serverSocket)->connectToHost(serverAddress, port);
                break;
        }

        emit connectionStatusChanged();

        reconnectTimer.start();
    }
}

void CelluloRelayClient::disconnectFromServer(){
    if(serverSocket){
        qInfo() << "CelluloRelayClient::disconnectFromServer(): Disconnecting from server...";

        if(getConnectionStatus() != CelluloCommUtil::RelayConnectionStatusDisconnected){
            switch(protocol){
                case CelluloCommUtil::RelayProtocol::Local:
                    ((QLocalSocket*)serverSocket)->disconnectFromServer();
                    break;

                case CelluloCommUtil::RelayProtocol::Tcp:
                    ((QTcpSocket*)serverSocket)->disconnectFromHost();
                    break;
            }
        }

        disconnect(serverSocket, SIGNAL(readyRead()),      this, SLOT(incomingServerData()));
        disconnect(serverSocket, SIGNAL(connected()),      this, SIGNAL(connected()));
        disconnect(serverSocket, SIGNAL(disconnected()),   this, SIGNAL(disconnected()));

        serverSocket->deleteLater();
        serverSocket = nullptr;

        emit connectionStatusChanged();

        reconnectTimer.stop();

        lastMacAddr = "";
        localAdapters.clear();
        emit localAdaptersChanged();
    }

    if(autoConnect)
        connectToServer();
}

void CelluloRelayClient::handleConnected(){
    qInfo() << "CelluloRelayClient::handleConnected(): Connected to server.";
    reconnectTimer.stop();
    emit connectionStatusChanged();
}

void CelluloRelayClient::handleDisconnected(){
    qInfo() << "CelluloRelayClient::handleConnected(): Disconnected from server.";
    disconnectFromServer();
}

void CelluloRelayClient::heartbeatTimedOut(){
    qInfo() << "CellluloRelayClient::heartbeatTimedOut(): Heartbeat lost, disconnecting client.";
    disconnectFromServer();
}

void CelluloRelayClient::addRobot(CelluloBluetooth* robot, bool select){
    if(!robots.contains(robot)){
        robots.append(robot);
        robot->setRelayClient(this);

        if(select)
            currentRobot = robots.size() - 1;

        emit robotsChanged();
    }
}

void CelluloRelayClient::removeRobot(CelluloBluetooth* robot){
    if(robot != NULL){
        CelluloBluetooth* prevRobot = NULL;
        if(currentRobot >= 0)
            prevRobot = robots[currentRobot];

        if(robots.removeAll(robot) > 0){
            robot->setRelayClient(NULL);
            if(prevRobot == robot)
                prevRobot = NULL;

            emit robotsChanged();
        }

        currentRobot = robots.indexOf(prevRobot);
    }
}

QVariantList CelluloRelayClient::getRobots() const {
    QVariantList robotsVar;
    for(CelluloBluetooth* robot : robots)
        robotsVar << QVariant::fromValue(robot);
    return robotsVar;
}

QVariantList CelluloRelayClient::getLocalAdapters() const {
    QVariantList adaptersVar;
    for(QString const& adapter : localAdapters)
        adaptersVar << QVariant::fromValue(adapter);
    return adaptersVar;
}

void CelluloRelayClient::incomingServerData(){
    QByteArray message = serverSocket->readAll();

    for(int i=0; i<message.length(); i++)

        //Load byte as part of an event message and check end of packet
        if(serverPacket.loadEventByte(message[i]))
            processServerPacket();
}

void CelluloRelayClient::processServerPacket(){
    CelluloBluetoothPacket::EventPacketType packetType = serverPacket.getEventPacketType();

    //Heartbeat
    if(packetType == CelluloBluetoothPacket::EventPacketTypeHeartbeat)
        heartbeatTimeoutTimer.start();

    //Local adapter announcement
    else if(packetType == CelluloBluetoothPacket::EventPacketTypeAnnounceLocalAdapter){
        bool added = (bool)serverPacket.unloadUInt8();
        quint8 firstOctet = serverPacket.unloadUInt8();
        quint8 secondOctet = serverPacket.unloadUInt8();
        quint8 thirdOctet = serverPacket.unloadUInt8();
        quint8 fourthOctet = serverPacket.unloadUInt8();
        quint8 fifthOctet = serverPacket.unloadUInt8();
        quint8 sixthOctet = serverPacket.unloadUInt8();
        QString localAdapter = CelluloCommUtil::getMacAddr(QList<quint8>({firstOctet, secondOctet, thirdOctet, fourthOctet, fifthOctet, sixthOctet})).toUpper();

        if(added && !localAdapters.contains(localAdapter)){
            localAdapters.append(localAdapter);
            emit localAdaptersChanged();
        }
        else if(!added && localAdapters.contains(localAdapter)){
            localAdapters.removeAll(localAdapter);
            emit localAdaptersChanged();
        }
    }

    //Set target robot event
    else if(packetType == CelluloBluetoothPacket::EventPacketTypeSetAddress){
        quint8 firstOctet = serverPacket.unloadUInt8();
        quint8 secondOctet = serverPacket.unloadUInt8();
        quint8 thirdOctet = serverPacket.unloadUInt8();
        quint8 fourthOctet = serverPacket.unloadUInt8();
        quint8 fifthOctet = serverPacket.unloadUInt8();
        quint8 sixthOctet = serverPacket.unloadUInt8();
        QString incomingAddr = CelluloCommUtil::getMacAddr(QList<quint8>({firstOctet, secondOctet, thirdOctet, fourthOctet, fifthOctet, sixthOctet}));

        int newRobot = -1;
        for(int i=0; i<robots.size(); i++)
            if(robots[i]->getMacAddr().compare(incomingAddr, Qt::CaseInsensitive) == 0){
                newRobot = i;
                break;
            }

        if(newRobot < 0){
            currentRobot = -1;
            emit unknownRobotAtServer(incomingAddr);
        }
        else
            currentRobot = newRobot;
    }

    //Some other event but no robot selected yet
    else if(currentRobot < 0)
        qWarning() << "CelluloRelayClient::processServerPacket(): Received event packet but no robot is chosen yet, EventPacketTypeSetAddress must be sent first. Dropping this packet.";

    //Connection status announcement
    else if(packetType == CelluloBluetoothPacket::EventPacketTypeAnnounceConnectionStatus){

        //Connection status
        CelluloBluetoothEnums::ConnectionStatus newStatus = (CelluloBluetoothEnums::ConnectionStatus)serverPacket.unloadUInt8();
        if(robots[currentRobot]->connectionStatus != newStatus){
            robots[currentRobot]->connectionStatus = newStatus;
            emit robots[currentRobot]->connectionStatusChanged();
        }

        //Local adapter MAC address
        QList<quint8> octets;
        for(int i=0;i<6;i++)
            octets.append(serverPacket.unloadUInt8());
        robots[currentRobot]->setLocalAdapterMacAddr(CelluloCommUtil::getMacAddr(octets));
    }

    //Some other command and there is already a target robot
    else
        robots[currentRobot]->processResponse(serverPacket);

    serverPacket.clear();
}

void CelluloRelayClient::sendToServer(QString macAddr, CelluloBluetoothPacket const& packet){

    //Send MAC address only if another robot is targeted
    if(lastMacAddr != macAddr){
        QList<quint8> octets = CelluloCommUtil::getOctets(macAddr);
        if(octets[0] == 0 && octets[1] == 0 && octets[2] == 0 && octets[3] == 0 && octets[4] == 0 && octets[5] == 0){
            qWarning() << "CelluloRelayClient::sendToServer(): Provided MAC address is in the wrong format.";
            return;
        }

        CelluloBluetoothPacket setAddressPacket;
        setAddressPacket.setCmdPacketType(CelluloBluetoothPacket::CmdPacketTypeSetAddress);
        setAddressPacket.load(octets[0]);
        setAddressPacket.load(octets[1]);
        setAddressPacket.load(octets[2]);
        setAddressPacket.load(octets[3]);
        setAddressPacket.load(octets[4]);
        setAddressPacket.load(octets[5]);
        serverSocket->write(setAddressPacket.getCmdSendData());

        lastMacAddr = macAddr;
    }

    //If connect/disconnect requested, need to resend MAC addr in the future
    if(packet.getCmdPacketType() == CelluloBluetoothPacket::CmdPacketTypeSetConnectionStatus)
        lastMacAddr = "";

    //Send actual packet
    serverSocket->write(packet.getCmdSendData());
}

}
