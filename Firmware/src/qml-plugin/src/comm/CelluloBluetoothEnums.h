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
 * @file CelluloBluetoothEnums.h
 * @brief Header for CelluloBluetooth enums
 * @author Ayberk Özgür
 * @date 2016-06-07
 */

#ifndef CELLULOBLUETOOTHENUMS_H
#define CELLULOBLUETOOTHENUMS_H

#include <QObject>

#include "CelluloBluetoothSharedDefs.h"
#include "../CelluloEnumDecl.h"

namespace Cellulo{

/**
 * @addtogroup comm
 * @{
 */

/**
 * @brief Wrapper containing all the enums of CelluloBluetooth
 * @singleton
 */
class CelluloBluetoothEnums : public QObject {
    /* *INDENT-OFF* */
    Q_OBJECT
    /* *INDENT-ON* */

public:

    /** @cond DO_NOT_DOCUMENT */

    CelluloBluetoothEnums(QObject* parent = 0);
    ~CelluloBluetoothEnums();

    /** @endcond */

    /**
     * @brief Bluetooth connection status
     */
    CONNECTION_STATUS_ENUM_SHARED;
    Q_ENUM(ConnectionStatus)
    CELLULO_ENUM_DECL(ConnectionStatus)

    /**
     * @brief Battery state of the robot
     */
    BATTERY_STATE_ENUM_SHARED;
    Q_ENUM(BatteryState)
    CELLULO_ENUM_DECL(BatteryState)

    /**
     * @brief LED response mode of the robot
     */
    LED_RESPONSE_MODE_ENUM_SHARED;
    Q_ENUM(LEDResponseMode)
    CELLULO_ENUM_DECL(LEDResponseMode)

    /**
     * @brief Locomotion interactivity mode of the robot
     */
    LOCOMOTION_INTERACTIVITY_MODE_ENUM_SHARED;
    Q_ENUM(LocomotionInteractivityMode)
    CELLULO_ENUM_DECL(LocomotionInteractivityMode)

    /**
     * @brief List of possible visual effects
     */
    VISUAL_EFFECT_ENUM_SHARED;
    Q_ENUM(VisualEffect)
    CELLULO_ENUM_DECL(VisualEffect)

    /**
     * @brief List of possible getures
     */
    GESTURE_ENUM_SHARED;
    Q_ENUM(Gesture)
    CELLULO_ENUM_DECL(Gesture)
};

/** @} */

}

Q_DECLARE_METATYPE(Cellulo::CelluloBluetoothEnums::ConnectionStatus)
Q_DECLARE_METATYPE(Cellulo::CelluloBluetoothEnums::BatteryState)
Q_DECLARE_METATYPE(Cellulo::CelluloBluetoothEnums::LEDResponseMode)
Q_DECLARE_METATYPE(Cellulo::CelluloBluetoothEnums::LocomotionInteractivityMode)
Q_DECLARE_METATYPE(Cellulo::CelluloBluetoothEnums::VisualEffect)
Q_DECLARE_METATYPE(Cellulo::CelluloBluetoothEnums::Gesture)

#endif // CELLULOBLUETOOTHENUMS_H
