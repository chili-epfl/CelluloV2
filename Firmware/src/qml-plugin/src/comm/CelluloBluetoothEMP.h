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
 * @file CelluloBluetoothEMP.h
 * @brief Header of object that mass reset/shuts down all robots
 * @author Ayberk Özgür
 * @date 2015-05-20
 */

#ifndef CELLULOBLUETOOTHEMP_H
#define CELLULOBLUETOOTHEMP_H

#include "CelluloBluetooth.h"

namespace Cellulo{

/**
 * @addtogroup comm
 * @{
 */

/**
 * @brief Object that mass reset/shuts down all robots
 */
class CelluloBluetoothEMP : public QQuickItem {
    /* *INDENT-OFF* */
    Q_OBJECT
    /* *INDENT-ON* */

    /** @brief Whether to shut down or reset the same robots over and over, default false */
    Q_PROPERTY(bool continuous READ getContinuous WRITE setContinuous NOTIFY continuousChanged)

    /** @brief List of robots waiting to be shut down or reset */
    Q_PROPERTY(QStringList macAddrToBlast READ getMacAddrToBlast NOTIFY macAddrToBlastChanged)

    /** @brief List of robots already shut down or reset */
    Q_PROPERTY(QStringList macAddrBlasted READ getMacAddrBlasted NOTIFY macAddrBlastedChanged)

public:

    /** @cond DO_NOT_DOCUMENT */

    /**
     * @brief Creates a new EMP blaster
     *
     * @param parent Parent of this QML item
     */
    explicit CelluloBluetoothEMP(QQuickItem* parent = 0);

    /**
     * @brief Destroys this EMP blaster
     */
    virtual ~CelluloBluetoothEMP();

    /**
     * @brief Gets whether to blast same robots
     *
     * @return Whether to blast same robots
     */
    bool getContinuous(){ return continuous; }

    /**
     * @brief Gets the mac addresses of robots that are waiting to be blasted
     *
     * @return Mac addresses of robots that are waiting to be blasted
     */
    QStringList getMacAddrToBlast() const { return macAddrToBlast; }

    /**
     * @brief Gets the mac addresses of robots that are confirmed to be blasted
     *
     * @return Mac addresses of robots that are confirmed to be blasted
     */
    QStringList getMacAddrBlasted() const { return macAddrBlasted; }

    /** @endcond */

signals:

    /** @cond DO_NOT_DOCUMENT */

    /**
     * @brief Whether to blast same robots strategy changed
     */
    void continuousChanged();

    /**
     * @brief Emitted when list of mac addresses waiting to be blasted changes
     */
    void macAddrToBlastChanged();

    /**
     * @brief Emitted when list of mac addresses confirmed to be blasted changes
     */
    void macAddrBlastedChanged();

    /** @endcond */

public slots:

    /** @cond DO_NOT_DOCUMENT */

    /**
     * @brief Sets whether to blast same robots
     *
     * @param continuous Whether to blast same robots
     */
    void setContinuous(bool continuous);

    /** @endcond */

    /**
     * @brief Initiates connection to the robot to reset it when it's connected
     */
    void resetLater(QString macAddr);

    /**
     * @brief Initiates connection to the robot to shut it down when it's connected
     */
    void shutdownLater(QString macAddr);

    /**
     * @brief Clears both lists, drops all connections
     */
    void clear();

private slots:

    /**
     * @brief Resets the sender CelluloBluetooth if it is connected
     */
    void resetIfConnected();

    /**
     * @brief Shuts down the sender CelluloBluetooth if it is connected
     */
    void shutdownIfConnected();

    /**
     * @brief Puts the sender at the back of the queue if it is disconnected
     */
    void requeueIfDisconnected();

    /**
     * @brief Moves the sender's mac address from the waiting list to done list
     *
     * @param robot Robot to move to the blasted list; moves sender if NULL
     */
    void moveToBlastedList(CelluloBluetooth* robot = NULL);

private:

    bool continuous;                        ///< Whether to blast same robots

    QStringList macAddrToBlast;             ///< List of mac addresses that are being blasted
    QStringList macAddrBlasted;             ///< List of mac addresses that are confirmed to be blasted

    QList<CelluloBluetooth*> robotsToBlast; ///< List of robots that are being blasted

    /**
     * @brief Gets whether the given address is already in one of waiting or done lists
     *
     * @param macAddr Address to check
     * @return Whether the address is in one of the lists
     */
    bool inLists(QString macAddr);

    /**
     * @brief Starts up to 7 connections from the socket queue, the ones already running are unaffected
     */
    void startConnections();

};

/** @} */

}

#endif // CELLULOBLUETOOTHEMP_H
