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
 * @file pid.h
 * @brief Header for PID controller functionality
 * @author Ayberk Özgür
 * @date 2016-02-16
 */

#ifndef PID_H
#define PID_H

#include <stdbool.h>

#ifdef  __cplusplus
extern "C" {
#endif

/**
 * @brief Object describing the PID controller and its state
 */
typedef struct {
    float kP;           ///< Proportional coeff.
    float kI;           ///< Integral coeff.
    float kD;           ///< Derivative coeff.
    float termI;        ///< I term, i.e sum of errors*kI
    float termILimit;   ///< Limit for I term to prevent windup
    float prevValue;    ///< Previous input value to calculate D term
    bool reset;         ///< The controller was reset and lacks prevValue
} PID_t;

/**
 * @brief Initializes the PID controller
 *
 * @param pid Controller to initialize
 * @param kP Proportional coefficient
 * @param kI Integral coefficient
 * @param kD Derivative coefficient
 * @param limI Limit (absolute) for I term to prevent windup
 */
void initPID(PID_t* pid, float kP, float kI, float kD, float limI);

/**
 * @brief Resets the PID controller, i.e zeroes I term and discards prevValue
 *
 * @param pid Controller to reset
 */
void resetPID(PID_t* pid);

/**
 * @brief Calculates the output of the controller, performing one step
 *
 * @param pid Controller to update
 * @param deltaT Time length of the step
 * @param currentValue Current value of the plant
 * @param goalValue Goal value of the plant
 * @return Control output
 */
float updatePID(PID_t* pid, float deltaT, float currentValue, float goalValue);

#ifdef  __cplusplus
}
#endif

#endif //PID_H
