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
 * @file timer.h
 * @brief Header for timing functionality
 * @author Ayberk Özgür
 * @date 2015-05-13
 */

#ifndef TIMER_H
#define TIMER_H

#include <xc.h>
#include <GenericTypeDefs.h>
#include <sys/attribs.h>

#include "system_config/pic32mz1024ecg064/system_definitions.h"

#define IDLE_TIMEOUT_MILLIS 5000   ///< After this much millis of inactivity, the robot will enter idle mode

extern unsigned int systemMillis;  ///< The (approximate) milliseconds passed since last reset

/**
 * @brief Stops timers for sleep
 */
void suspendTimers();

/**
 * @brief Restarts timers (exit from sleep)
 */
void resumeTimers();

/**
 * @brief Resets the idle timeout
 */
void reportActivity();

/**
 * @brief Initializes all timers
 */
void APP_Timer_Initialize();

/**
 * @brief Performs routine timer tasks
 */
void APP_Timer_Tasks();

#endif //TIMER_H
