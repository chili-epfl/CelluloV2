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
 * @file power.h
 * @brief Header for power and reset functionality
 * @author Ayberk Özgür
 * @date 2015-05-17
 */

#ifndef POWER_H
#define POWER_H

#include <xc.h>
#include <GenericTypeDefs.h>
#include "system_config/pic32mz1024ecg064/system_definitions.h"

//This is 1 second with a 1:256 prescaler
#define IDLE_EXIT_TIMER_PERIOD_HI 0x0005
#define IDLE_EXIT_TIMER_PERIOD_LO 0xF5E1

/**
 * @brief Describes the power/reset states when the device booted up the last time
 */
typedef enum {
    BOOT_STATE_POWER_ON = 0,      ///< Device was powered on normally
    BOOT_STATE_BROWNOUT,          ///< Device voltage dropped below acceptable levels
    BOOT_STATE_MCLR,              ///< External reset was given using MCLR_BAR pin
    BOOT_STATE_SOFTWARE_RESET,    ///< Software reset was given
    BOOT_STATE_CFG_WORD_MISMATCH, ///< Corrupted configuration bits were loaded
    BOOT_STATE_WATCHDOG_TIMEOUT,  ///< Watchdog was not kicked soon enough
    BOOT_STATE_DEADMAN_TIMEOUT,   ///< Software died
    BOOT_STATE_INVALID = -1       ///< Boot state was not determined (yet or ever)
}BOOT_STATE;

extern BOOT_STATE bootState;      ///< Power/reset state when the device booted up the last time
extern bool sleepRequested;       ///< Flag to indicate sleep command was given
extern bool idleRequested;        ///< Flag to indicate need to enter idle mode to save power

/**
 * @brief Performs a software reset sequence
 */
void softwareReset();

/**
 * @brief Requests to enter sleep mode
 */
void requestSleep();

/**
 * @brief Initializes power and reset functionality and states
 */
void APP_Power_Initialize();

/**
 * @brief Performs regular power and reset tasks
 */
void APP_Power_Tasks();

#endif //POWER_H
