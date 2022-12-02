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
 * @file CelluloRelayClient.h
 * @brief Relays packets between a server and virtual robot objects
 * @author Ayberk Özgür
 * @date 2016-11-22
 */

#ifndef CELLULORELAYCLIENT_H
#define CELLULORELAYCLIENT_H

#include <QQuickItem>
#include <QIODevice>
#include <QTcpServer>
#include <QTcpSocket>
#include <QLocalServer>
#include <QLocalSocket>

#include "../CelluloBluetooth.h"
#include "../CelluloBluetoothPacket.h"
#include "../CelluloCommUtil.h"

namespace Cellulo{

class CelluloBluetooth;

/**
 * @addtogroup comm-relay
 * @{
 */

/**
 * @brief Object that relays packets between a `CelluloRelayServer` and virtual robot objects.
 *
 * The robots that are connected to this object (i.e having their relay client set as this object) have all their
 * commands routed to the `CelluloRelayServer` that is connected to this client, effectively making the
 * `CelluloBluetooth`s under control of this client to become virtual robots that represent the physical robots
 * connected to the server side.
 *
 * @abstract
 */
class CelluloRelayClient : public QQuickItem {
    /* *INDENT-OFF* */
    Q_OBJECT
    /* *INDENT-ON* */

    /** @brief Connection status to the server, read-only */
    Q_PROPERTY(Cellulo::CelluloCommUtil::RelayConnectionStatus connectionStatus READ getConnectionStatus NOTIFY connectionStatusChanged)

    /** @brief Address to connect to, i.e name of the domain socket (default is "cellulo_relay") or the IP address of the TCP socket (default is "localhost") */
    Q_PROPERTY(QString serverAddress READ getServerAddress WRITE setServerAddress NOTIFY serverAddressChanged)

    /** @brief Port to connect to in TCP (default is 2556), unused in local sockets */
    Q_PROPERTY(int port READ getPort WRITE setPort NOTIFY portChanged)

    /** @brief Whether to try to reconnect when the connection drops or in the beginning, default is true */
    Q_PROPERTY(bool autoConnect READ getAutoConnect WRITE setAutoConnect NOTIFY autoConnectChanged)

    /** @brief List of local adapters present at the server */
    Q_PROPERTY(QVariantList localAdapters READ getLocalAdapters NOTIFY localAdaptersChanged)

    /** @brief List of virtual robots currently under control of this client, read-only */
    Q_PROPERTY(QVariantList robots READ getRobots NOTIFY robotsChanged)

    friend class CelluloBluetooth;

public:

    /** @cond DO_NOT_DOCUMENT */

    /**
     * @brief Creates a new CelluloRelayClient with the given QML parent
     *
     * @param protocol Underlying transfer protocol to use
     * @param parent The QML parent
     */
    CelluloRelayClient(CelluloCommUtil::RelayProtocol protocol, QQuickItem* parent = 0);

    /**
     * @brief Destroys this CelluloRelayClient
     */
    virtual ~CelluloRelayClient();

    /**
     * @brief Gets the status of connection to the server
     *
     * @return Connection status
     */
    Cellulo::CelluloCommUtil::RelayConnectionStatus getConnectionStatus() const;

    /**
     * @brief Gets whether the socket tries to reconnect when it drops
     *
     * @return Whether the socket tries to reconnect when it drops
     */
    bool getAutoConnect(){ return autoConnect; }

    /**
     * @brief Sets whether the socket will try to reconnect when it drops
     *
     * @param autoReconnect Whether the socket will try to reconnect when it drops
     */
    void setAutoConnect(bool autoConnect);

    /**
     * @brief Gets the current server's address
     *
     * @return Current server address, e.g `127.0.0.1` for TCP
     */
    QString getServerAddress() const { return serverAddress; }

    /**
     * @brief Sets the server address to connect to
     *
     * @param peer The new server address, e.g `127.0.0.1` for TCP
     */
    void setServerAddress(QString serverAddress);

    /**
     * @brief Gets the current port
     *
     * @return Current port
     */
    int getPort() const { return port; }

    /**
     * @brief Sets the port
     *
     * @param port The new port, must be in [0,65535]
     */
    void setPort(int port);

    /**
     * @brief Gets the list of robots belonging to this client
     *
     * @return List of robots belonging to this client
     */
    QVariantList getRobots() const;

    /**
     * @brief Gets the list of local adapters at the server
     *
     * @return List of local adapters at the server
     */
    QVariantList getLocalAdapters() const;

    /** @endcond */

public slots:

    /**
     * @brief Initiates a connection to the service on the server
     */
    void connectToServer();

    /**
     * @brief Starts closing the socket
     */
    void disconnectFromServer();

    /**
     * @brief Adds the robot to the robots list, sets the robot's relay client to this object
     *
     * @param robot New robot
     * @param select Selects the newly added robot as current; pass true if robot is added as a result of an unknownRobotAtServer() signal
     */
    void addRobot(Cellulo::CelluloBluetooth* robot, bool select = false);

    /**
     * @brief Removes the robot from the robots list
     *
     * @param robot Robot to remove
     */
    void removeRobot(Cellulo::CelluloBluetooth* robot);

signals:

    /** @cond DO_NOT_DOCUMENT */

    /**
     * @brief Emitted when connected/disconnected to/from server
     */
    void connectionStatusChanged();

    /**
     * @brief Auto connect strategy changed
     */
    void autoConnectChanged();

    /**
     * @brief Emitted when the server address changes
     */
    void serverAddressChanged();

    /**
     * @brief Emitted whe the port changes
     */
    void portChanged();

    /**
     * @brief Emitted when the robot list changes
     */
    void robotsChanged();

    /**
     * @brief Emitted when the list of local adapters at the server changes
     */
    void localAdaptersChanged();

    /** @endcond */

    /**
     * @brief Emitted when the server socket is connected
     */
    void connected();

    /**
     * @brief Emitted when the server socket is disconected
     */
    void disconnected();

    /**
     * @brief Emitted when the server has a robot already connected that is not in the robots list covered by this client
     *
     * @param macAddr MAC address of the unknown robot
     */
    void unknownRobotAtServer(QString macAddr);

private slots:

    /**
     * @brief Load the server data into a packet to be processed
     */
    void incomingServerData();

    /**
     * @brief Connection is not happening, reconnect
     */
    void refreshConnection();

    /**
     * @brief Adjusts internal state on connection established
     */
    void handleConnected();

    /**
     * @brief Adjusts internal state on connection unmade
     */
    void handleDisconnected();

    /**
     * @brief Sends a heartbeat to the server if connected
     */
    void sendHeartbeat();

    /**
     * @brief Disconnects the server since the heartbeat was lost
     */
    void heartbeatTimedOut();

protected:

    QString serverAddress;  ///< Server's address, e.g "127.0.0.1"

private:

    /**
     * @brief Relays the packet from the server to the robot, or queues for relay, or sets target robot
     */
    void processServerPacket();

    /**
     * @brief Relays the packet from the robot to the server, or queues for relay
     *
     * @param macAddr Full MAC address of the target robot on the server
     * @param packet The packet to relay
     */
    void sendToServer(QString macAddr, CelluloBluetoothPacket const& packet);

    static const int SERVER_RECONNECT_TIME_MILLIS = 5000; ///< Will try to reconnect to the server after this much time

    CelluloCommUtil::RelayProtocol protocol;              ///< Underlying transfer protocol

    bool autoConnect;                                     ///< Whether to try to reconnect to the relay server if connection is lost
    QTimer reconnectTimer;                                ///< Timeout timer to reconnect if connection fails

    QTimer heartbeatTimer;                                ///< Timer to periodically announce the presence of the client
    QTimer heartbeatTimeoutTimer;                         ///< Timer to check the presence of the server

    quint16 port;                                         ///< Port to connect to on the server

    QIODevice* serverSocket;                              ///< Socket to server that handles communication
    CelluloBluetoothPacket serverPacket;                  ///< Server's incoming packet

    int currentRobot;                                     ///< Current robot index to relay messages to, set by a CmdPacketTypeSetAddress
    QList<CelluloBluetooth*> robots;                      ///< List of robots to relay to/from

    QList<QString> localAdapters;                         ///< List of local adapters present at the server

    QString lastMacAddr;                                  ///< MAC address of the last CmdPacketTypeSetAddress packet sent to the server

};

/** @} */

}

#endif // CELLULORELAYCLIENT_H
