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
 * @file gesture.h
 * @brief Header for touch key gesture functionality
 * @author Ayberk Özgür
 * @date 2016-06-14
 */

#ifndef GESTURE_H
#define GESTURE_H

#include <GenericTypeDefs.h>
#include "system_config/pic32mz1024ecg064/system_definitions.h"

#include "qml-plugin/include/CelluloBluetoothSharedDefs.h"

#define GESTURE_HOLD_SUM_THRESHOLD 10 ///< If the key value sum is larger than this, the robot is considered held

typedef GESTURE_ENUM_SHARED Gesture;

extern bool gestureEnabled;           ///< Whether raw key signal querying and gesture extraction is done
extern Gesture currentGesture;        ///< Current gesture

/**
 * @brief Initializes touch input gesture functionality
 */
void APP_Gesture_Initialize();

/**
 * @brief Performs regular touch input gesture tasks
 */
void APP_Gesture_Tasks();

#endif //GESTURE_H
