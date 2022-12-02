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
 * @file touch.h
 * @brief Header for touch input functionality
 * @author Ayberk Özgür
 * @date 2015-05-03
 */

#ifndef TOUCH_H
#define TOUCH_H

#include "at42qt1070.h"

#include <GenericTypeDefs.h>

#include "system_config/pic32mz1024ecg064/system_definitions.h"

#include "timer.h"

#define TOUCH_CHANGE_BAR_PORT_CHANNEL PORT_CHANNEL_D
#define TOUCH_CHANGE_BAR_PORT_BIT_POS PORTS_BIT_POS_9

#define LONG_TOUCH_MILLIS 2000
#define SLEEP_WAKEUP_HOLD_MILLIS 3000 ///< 3 finger death punch and 4 finger or more hold duration

#define TOUCH_SAMPLE_PERIOD_8_MS 4    ///< Sample period in multiples of 8 milliseconds

/**
 * @brief List of possible key events
 */
typedef enum {
    KEY_EVENT_NONE = 0,
    KEY_EVENT_TOUCH_BEGIN,
    KEY_EVENT_TOUCHING,
    KEY_EVENT_LONG_TOUCH,
    KEY_EVENT_RELEASE
}KEY_EVENT;

/**
 * @brief Gets the number of keys that are being touched
 *
 * @return Number of keys that are touched now
 */
unsigned char numKeysTouched();

/**
 * @brief Returns whether to stay in sleep or exit
 *
 * @return Whether not all the keys were being held long enough
 */
bool stayAsleepTouch();

/**
 * @brief Suspends touch activity for sleep, must be called last
 */
void suspendTouchForSleep();

/**
 * @brief Samples key states until they are the latest
 *
 * @return Whether the sample was successful
 */
bool sampleKeyStates();

/**
 * @brief Initializes touch input functionality
 */
void APP_Touch_Initialize();

/**
 * @brief Performs regular touch input tasks
 */
void APP_Touch_Tasks();

#endif //TOUCH_H
