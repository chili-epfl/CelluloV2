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
 * @file motor.h
 * @brief Header for motor control functionality
 * @author Ayberk Özgür
 * @date 2015-10-07
 */

#ifndef MOTOR_H
#define MOTOR_H

#include <xc.h>
#include <GenericTypeDefs.h>
#include <sys/attribs.h>

#include "system_config/pic32mz1024ecg064/system_definitions.h"

#include "motion.h"
#include "localization.h"

#define MOTOR_PWM_PERIOD 4095
#define FWD_REV_FACTOR 0.933f

/**
 * @brief Sets all motor outputs to zero
 */
void disableAllMotors();

/**
 * @brief Sets the motor output PWM duty
 *
 * @param motor The motor number: 1, 2 or 3
 * @param output From -4095 to 4095
 */
void setMotorOutput(unsigned char motor, short output);

/**
 * @brief Initializes motor control
 */
void APP_Motor_Initialize();

/**
 * @brief Performs regular motor control tasks
 */
void APP_Motor_Tasks();

#endif //MOTOR_H
