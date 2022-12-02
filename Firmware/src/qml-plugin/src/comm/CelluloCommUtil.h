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
 * @file CelluloCommUtil.h
 * @brief Common objects used by Cellulo communication objects
 * @author Ayberk Özgür
 * @date 2017-03-15
 */

#ifndef CELLULOCOMMUTIL_H
#define CELLULOCOMMUTIL_H

#include <QObject>
#include <QString>
#include <QList>
#include <QDebug>

#include "../CelluloEnumDecl.h"

namespace Cellulo{

/**
 * @addtogroup comm
 * @{
 */

/**
 * @brief Cellulo communication utilities
 * @singleton
 */
class CelluloCommUtil : public QObject {
    /* *INDENT-OFF* */
    Q_OBJECT
    /* *INDENT-ON* */

public:

    /** @cond DO_NOT_DOCUMENT */

    CelluloCommUtil(QObject* parent = 0);
    virtual ~CelluloCommUtil();

    static const int DEFAULT_RELAY_PORT = 2556;         ///< Default TCP port for Cellulo relay communication

    /**
     * @brief Underlying transfer protocols supported by the relay servers and clients
     */
    enum class RelayProtocol{
        Local = 0,   ///< Unix domain socket
        Tcp          ///< TCP socket
    };

    /**
     * @brief Gets the octets of a given MAC address
     *
     * @param macAddr Bluetooth MAC address in the form of XX:XX:XX:XX:XX:XX
     * @return 6 octets from left to right
     */
    static QList<quint8> getOctets(QString const& macAddr);

    /**
     * @brief Gets the MAC address made of the given octets
     *
     * @param octets 6 octets from left to right
     * @return Bluetooth MAC address in the form of "XX:XX:XX:XX:XX:XX"; returns empty string if octets not valid or the address is 00:00:00:00:00:00
     */
    static QString getMacAddr(QList<quint8> const& octets);

    /** @endcond */

    static constexpr int RELAY_HEARTBEAT_INTERVAL = 1500;   ///< RelayClients will send a heartbeat message with this interval (milliseconds)
    static constexpr int RELAY_HEARTBEAT_TIMEOUT = 6000;    ///< RelayServer will drop a connection if this many milliseconds pass without a heartbeat

    /**
     * @brief Relay connection status
     */
    enum RelayConnectionStatus {
        RelayConnectionStatusDisconnected = 0, ///< Idle and not connected
        RelayConnectionStatusConnecting = 1,   ///< Actively trying to connect
        RelayConnectionStatusConnected = 2,    ///< Connected
        RelayConnectionStatusNumElements
    };
    Q_ENUM(RelayConnectionStatus)
    CELLULO_ENUM_DECL(RelayConnectionStatus)

    /**
     * @brief Tests whether cellulorobotpoold is installed
     *
     * @return Whether cellulorobotpoold is installed
     */
    Q_INVOKABLE static bool testRobotPoolDaemon();

    /**
     * @brief Attempts to launch a cellulorobotpoold
     *
     * @return Whether successfully launched
     */
    Q_INVOKABLE static bool startRobotPoolDaemon();

    /**
     * @brief Attempts to stop the already existing cellulorobotpoold
     *
     * @return Whether successfully stopped
     */
    Q_INVOKABLE static bool stopRobotPoolDaemon();

};

/** @} */

}

Q_DECLARE_METATYPE(Cellulo::CelluloCommUtil::RelayConnectionStatus)

#endif // CELLULOCOMMUTIL_H
