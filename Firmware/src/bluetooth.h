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
 * @file bluetooth.h
 * @brief Header for bluetooth functionality
 * @author Ayberk Özgür
 * @date 2015-05-02
 */

#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include "qml-plugin/include/CelluloBluetoothSharedDefs.h"

#include "power.h"
#include "battery.h"
#include "touch.h"
#include "gesture.h"

#include <GenericTypeDefs.h>

#include "system_config/pic32mz1024ecg064/system_definitions.h"

#define BT_RX_QUEUE_SIZE 256

#define BT_RESET_BAR_PORT_CHANNEL PORT_CHANNEL_C
#define BT_RESET_BAR_PORT_BIT_POS PORTS_BIT_POS_12

#define BT_BAUD_9600_PORT_CHANNEL PORT_CHANNEL_C
#define BT_BAUD_9600_PORT_BIT_POS PORTS_BIT_POS_15

#define BT_RTS_BAR_PORT_CHANNEL PORT_CHANNEL_G
#define BT_RTS_BAR_PORT_BIT_POS PORTS_BIT_POS_8

#define BT_RX_DISABLE() PORTGSET = 0b00000000000000000000001000000000
#define BT_RX_ENABLE()  PORTGCLR = 0b00000000000000000000001000000000

typedef COMMAND_PACKET_TYPE_ENUM_SHARED CmdPacketType;
typedef EVENT_PACKET_TYPE_ENUM_SHARED EventPacketType;

/**
 * @brief Suspends Bluetooth operation (for sleep)
 */
void suspendBluetooth();

/**
 * @brief Resumes Bluetooth operation (after waking up)
 */
void resumeBluetooth();

/**
 * @brief Sends a BOOT_COMPLETE packet
 */
void bluetoothEventBootComplete();

/**
 * @brief Sends a SHUTTING_DOWN packet
 */
void bluetoothEventShuttingDown();

/**
 * @brief Sends a LOW_BATTERY packet
 */
void bluetoothEventLowBattery();

/**
 * @brief Sends a BATTERY_STATE_CHANGED packet
 *
 * @param state The new battery state
 */
void bluetoothEventBatteryStateChanged(unsigned int state);

/**
 * @brief Sends a TOUCH_BEGIN packet
 *
 * @param key 0, 1, 2, 3, 4 or 5
 */
void bluetoothEventTouchBegin(unsigned int key);

/**
 * @brief Sends a TOUCH_LONG_PRESSED packet
 *
 * @param key 0, 1, 2, 3, 4 or 5
 */
void bluetoothEventTouchLongPressed(unsigned int key);

/**
 * @brief Sends a TOUCH_RELEASED packet
 *
 * @param key 0, 1, 2, 3, 4 or 5
 */
void bluetoothEventTouchReleased(unsigned int key);

/**
 * @brief Sends raw touch signal offsets
 *
 * @param keyOffsets Raw touch signal offsets with respect to references
 */
void bluetoothEventTouchRaw(short* keyOffsets);

/**
 * @brief Sends a POSE_CHANGED packet
 *
 * @param x X position in grid coordinates + 2 least significant digits of subgrid coordinates
 * @param y Y position in grid coordinates + 2 least significant digits of subgrid coordinates
 * @param theta Orientation in degrees + 2 least significant digits of subdegree orientation
 */
void bluetoothEventPoseChanged(int x, int y, int theta);

/**
 * @brief Sends a POSE_CHANGED_TIMESTAMPED packet
 *
 * @param x X position in grid coordinates + 2 least significant digits of subgrid coordinates
 * @param y Y position in grid coordinates + 2 least significant digits of subgrid coordinates
 * @param theta Orientation in degrees + 2 least significant digits of subdegree orientation
 */
void bluetoothEventPoseChangedTimestamped(int x, int y, int theta);

/**
 * @brief Sends a KIDNAP packet
 *
 * @param kidnapped Whether kidnapped or returned on some paper
 */
void bluetoothEventKidnapChanged(bool kidnapped);

/**
 * @brief Sends a gesture changed packet
 *
 * @param gesture Current gesture
 */
void bluetoothEventGestureChanged(Gesture gesture);

/**
 * @brief Sends a pose/position/orientation tracking goal reached packet
 */
void bluetoothEventTrackingGoalReached();

/**
 * @brief Sends an ACKNOWLEDGED packet
 */
void bluetoothEventAcknowledged();

/**
 * @brief Sends a CAMERA_IMAGE_LINE packet
 *
 * @param line The line number, IMG_WIDTH many bytes will be sent
 */
void bluetoothEventCameraImageLine(unsigned short line);

/**
 * @brief Sends a DEBUG packet
 */
void bluetoothEventDebug(unsigned int t, unsigned int u);

/**
 * @brief initializes Bluetooth communication
 */
void APP_BT_Initialize();

/**
 * @brief Performs regular Bluetooth communication tasks
 */
void APP_BT_Tasks();

#endif //BLUETOOTH_H
