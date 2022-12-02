/*
 * Copyright (C) 2018 EPFL
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
 * @file CelluloRobotHubClient.h
 * @brief Uses the robots at a cellulorobothubd
 * @author Ayberk Özgür
 * @date 2018-08-30
 */

#ifndef CELLULOROBOTHUBCLIENT_H
#define CELLULOROBOTHUBCLIENT_H

#include "CelluloTcpRelayClient.h"

#include "../CelluloBluetooth.h"

#include <QJSValue>
#include <QQmlComponent>

namespace Cellulo {

/**
 * @addtogroup comm-relay
 * @{
 */

/**
 * @brief Object that uses the robots connected to a `cellulorobothubd`.
 *
 * Inherits from `CelluloTcpRelayClient`, which in turn inherits from `CelluloRelayClient`, which means that their
 * properties, signals and slots can be used. The most useful among these is the `robots` property that exposes the
 * list of currently available robots in this pool.
 */
class CelluloRobotHubClient : public CelluloTcpRelayClient {
    /* *INDENT-OFF* */
    Q_OBJECT
    /* *INDENT-ON* */

    /** @brief If set, robots will be created out of this component instead of the standard CelluloRobot, default null */
    Q_PROPERTY(QQmlComponent* robotComponent READ getRobotComponent WRITE setRobotComponent NOTIFY robotComponentChanged)

    /**
     * @brief Can be set to a user-defined javascript function that takes the MAC address (string) of an already connected robot and returns a CelluloBluetooth object or CelluloBluetooth-derived object; ignored if robotComponent is also set
     */
    Q_PROPERTY(QJSValue createRobot READ getCreateRobot WRITE setCreateRobot NOTIFY createRobotChanged)

public:

    /** @cond DO_NOT_DOCUMENT */

    /**
     * @brief Creates a new CelluloRobotHubClient with the given QML parent
     *
     * @param parent The QML parent
     */
    CelluloRobotHubClient(QQuickItem* parent = 0);

    /**
     * @brief Destroys this CelluloRobotHubClient
     */
    virtual ~CelluloRobotHubClient();

    /**
     * @brief Gets the robot component that creates new robots
     *
     * @return Current robot component
     */
    QQmlComponent* getRobotComponent(){ return robotComponent; }

    /**
     * @brief Sets the robot component that creates new robots
     *
     * @param robotComponent New robot component
     */
    void setRobotComponent(QQmlComponent* robotComponent);

    /**
     * @brief Gets the createRobot function
     *
     * @return The createRobot function
     */
    QJSValue getCreateRobot() const { return createRobotFunction; }

    /**
     * @brief Sets the createRobot function
     *
     * @param function The createRobot function that takes a string and returns a CelluloBluetooth pointer or CelluloBluetooth-derived object pointer
     */
    void setCreateRobot(QJSValue const& function);

    /** @endcond */

signals:

    /** @cond DO_NOT_DOCUMENT */

    /**
     * @brief Emitted when the robot component changes
     */
    void robotComponentChanged();

    /**
     * @brief Emitted when the createRobot function changes
     */
    void createRobotChanged();

    /** @endcond */

    /**
     * @brief Emitted when a new robot is found in the pool
     *
     * @param robot The newly found robot
     */
    void newRobotFound(CelluloBluetooth* robot);

public slots:

    /**
     * @brief Creates, adds and selects a given robot
     *
     * @param macAddr MAC address of the robot object to create
     */
    void createAddSelectRobot(QString macAddr);

private:

    QQmlComponent* robotComponent; ///< Component to create robots out of
    QJSValue createRobotFunction;  ///< A Javascript function that takes a string and returns a CelluloBluetooth pointer or CelluloBluetooth-derived object pointer

};

/** @} */

}

#endif // CELLULOROBOTHUBCLIENT_H
