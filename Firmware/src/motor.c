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
 * @file motor.c
 * @brief Source for motor control functionality
 * @author Ayberk Özgür
 * @date 2015-10-07
 */

#include "motor.h"
#include "timer.h"
#include "led.h"
#include "config.h"

#define PI 3.14159265359f

#if defined(REVERSE_MOTORS)
    #define MA_REV OC1RS
    #define MA_FWD OC2RS
    #define MB_REV OC5RS
    #define MB_FWD OC8RS
    #define MC_REV OC3RS
    #define MC_FWD OC6RS
#else
    #define MA_FWD OC1RS
    #define MA_REV OC2RS
    #define MB_FWD OC5RS
    #define MB_REV OC8RS
    #define MC_FWD OC3RS
    #define MC_REV OC6RS
#endif

void disableAllMotors(){
    setMotorOutput(1,0);
    setMotorOutput(2,0);
    setMotorOutput(3,0);

    //Wait until new duty cycle is loaded
    int i;
    for(i=0; i<MOTOR_PWM_PERIOD; i++)
        Nop();
}

inline void initTimer2(){

    //Timer 2 is the motor PWM source, its period is the PWM period

    T2CON = 0;
    T2CONbits.TCS = 0;       //Use peripheral bus clock 3 (100MHz)
    T2CONbits.TCKPS = 0b000; //1:1 prescaler
    PR2 = MOTOR_PWM_PERIOD;  //The timer frequency is nominally 24.420 KHz

    T2CONbits.ON = 1;
}

inline void initOC(){
    //MA_FWD: OC1
    //MA_REV: OC2
    //MB_FWD: OC5
    //MB_REV: OC8
    //MC_FWD: OC3
    //MC_REV: OC6

    OC1CONbits.OCM = 0b110;
    MA_FWD = 0;

    OC2CONbits.OCM = 0b110;
    MA_REV = 0;

    OC5CONbits.OCM = 0b110;
    MB_FWD = 0;

    OC8CONbits.OCM = 0b110;
    MB_REV = 0;

    OC3CONbits.OCM = 0b110;
    MC_FWD = 0;

    OC6CONbits.OCM = 0b110;
    MC_REV = 0;

    OC1CONbits.ON = 1;
    OC2CONbits.ON = 1;
    OC5CONbits.ON = 1;
    OC8CONbits.ON = 1;
    OC3CONbits.ON = 1;
    OC6CONbits.ON = 1;
}

void setMotorOutput(unsigned char motor, short output){
    if(motor == 1)
        motor = M1_MAPPING;
    else if(motor == 2)
        motor = M2_MAPPING;
    else
        motor = M3_MAPPING;

    if(output > MOTOR_PWM_PERIOD)
        output = MOTOR_PWM_PERIOD;
    else if(output < -MOTOR_PWM_PERIOD)
        output = -MOTOR_PWM_PERIOD;

    switch(motor){
        case 'A':
            if(output >= 0){
                MA_REV = 0;
                MA_FWD = output;
            }
            else{
                MA_FWD = 0;
                MA_REV = -(short)(output*FWD_REV_FACTOR);
            }
            break;
        case 'B':
            if(output >= 0){
                MB_REV = 0;
                MB_FWD = output;
            }
            else{
                MB_FWD = 0;
                MB_REV = -(short)(output*FWD_REV_FACTOR);
            }
            break;
        case 'C':
            if(output >= 0){
                MC_REV = 0;
                MC_FWD = output;
            }
            else{
                MC_FWD = 0;
                MC_REV = -(short)(output*FWD_REV_FACTOR);
            }
            break;
        default:
            break;
    }
}

void APP_Motor_Initialize(){
    initTimer2();
    initOC();
}

void APP_Motor_Tasks(){
}
