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
 * @file pid.c
 * @brief Source for PID controller functionality
 * @author Ayberk Özgür
 * @date 2016-02-16
 */

#include "pid.h"

void initPID(PID_t* pid, float kP, float kI, float kD, float limI){
    pid->kP = kP;
    pid->kI = kI;
    pid->kD = kD;
    pid->termI = 0.0;
    pid->termILimit = limI;
    pid->prevValue = 0.0;
    pid->reset = true;
}

void resetPID(PID_t* pid){
    pid->reset = true;
    pid->termI = 0;
}

float updatePID(PID_t* pid, float deltaT, float currentValue, float goalValue){
    if(pid->reset){
        pid->reset = false;
        pid->prevValue = currentValue;
    }

    //P term
    float error = goalValue - currentValue;

    //I term
    pid->termI += pid->kI*deltaT*error;
    if(pid->termI > pid->termILimit)
        pid->termI = pid->termILimit;
    else if(pid->termI < -pid->termILimit)
        pid->termI = -pid->termILimit;

    //D term
    float valueD = (currentValue - pid->prevValue)/deltaT;
    pid->prevValue = currentValue;

    //Control output
    return pid->kP*error + pid->termI - pid->kD*valueD;
}
