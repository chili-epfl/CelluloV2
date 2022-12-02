/*
 * Copyright (C) 2015 EPFL
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
 * @file battery.h
 * @brief Header for battery functionality
 * @author Ayberk Özgür
 * @date 2015-05-09
 */

#ifndef BATTERY_H
#define BATTERY_H

#include "bluetooth.h"

#include <stdbool.h>
#include <xc.h>
#include <GenericTypeDefs.h>

#include "system_config/pic32mz1024ecg064/system_definitions.h"

#include "qml-plugin/include/CelluloBluetoothSharedDefs.h"

#define BAT_PG_BAR_PORT_CHANNEL PORT_CHANNEL_B
#define BAT_PG_BAR_PORT_BIT_POS PORTS_BIT_POS_5

#define BAT_STAT2_PORT_CHANNEL PORT_CHANNEL_B
#define BAT_STAT2_PORT_BIT_POS PORTS_BIT_POS_4

#define BAT_STAT1_LBO_PORT_CHANNEL PORT_CHANNEL_B
#define BAT_STAT1_LBO_PORT_BIT_POS PORTS_BIT_POS_3

#define VBAT_SENSE_ON_PORT_CHANNEL PORT_CHANNEL_B
#define VBAT_SENSE_ON_PORT_BIT_POS PORTS_BIT_POS_12

#define BAT_CE_PORT_CHANNEL PORT_CHANNEL_B
#define BAT_CE_PORT_BIT_POS PORTS_BIT_POS_13

typedef BATTERY_STATE_ENUM_SHARED BatteryState;

extern BatteryState batteryState; ///< The current battery state

/**
 * @brief Gets whether to stay asleep because of low battery
 *
 * @return Whether to stay asleep because of low battery
 */
bool stayAsleepBattery();

/**
 * @brief Gets whether to stay idle because battery state didn't change
 *
 * @return Whether to stay idle because battery state didn't change
 */
bool stayIdleBattery();

/**
 * @brief Initializes battery functionality
 */
void APP_Battery_Initialize();

/**
 * @brief Performs routine battery tasks
 */
void APP_Battery_Tasks();

#endif //BATTERY_H
