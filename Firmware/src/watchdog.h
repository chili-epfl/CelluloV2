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
 * @file watchdog.h
 * @brief Header for watchdog functionality
 * @author Ayberk Özgür
 * @date 2015-05-28
 */

#ifndef WATCHDOG_H
#define WATCHDOG_H

/**
 * @brief Resets the watchdog timer
 */
void watchdogKick();

/**
 * @brief Enables the watchdog timer
 *
 * Currently the watchdog period is 2.048 seconds
 */
void watchdogEnable();

/**
 * @brief Disables the watchdog timer
 */
void watchdogDisable();

/**
 * @brief Initializes watchdog functionality
 */
void APP_Watchdog_Initialize();

/**
 * @brief Kicks the watchdog
 */
void APP_Watchdog_Tasks();

#endif //WATCHDOG_H
