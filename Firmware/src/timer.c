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
 * @file timer.c
 * @brief Source for timing functionality
 * @author Ayberk Özgür
 * @date 2015-05-13
 */

#include "timer.h"

unsigned int systemMillis;
unsigned int lastActivity;

inline void initTimer8(){

    //Timer 8 counts the milliseconds

    systemMillis = 0;

    T8CON = 0;
    T8CONbits.TCS = 0;       //Use peripheral bus clock 3 (100MHz)
    T8CONbits.TCKPS = 0b101; //1:32 prescaler
    PR8 = 3125;              //The timer will count exactly (nominally) 1 ms

    SYS_INT_VectorPrioritySet(INT_VECTOR_T8, INT_PRIORITY_LEVEL1);
    SYS_INT_VectorSubprioritySet(INT_VECTOR_T8, INT_SUBPRIORITY_LEVEL0);
    PLIB_INT_SourceFlagClear(INT_ID_0, INT_SOURCE_TIMER_8);
    SYS_INT_SourceEnable(INT_SOURCE_TIMER_8);

    T8CONbits.ON = 1;
}

void APP_Timer_Initialize(){
    initTimer8();
}

void APP_Timer_Tasks(){
    //if(systemMillis > lastActivity + IDLE_TIMEOUT_MILLIS)
    //    requestIdle();
}

void resumeTimers(){
    T8CON = 0;
    T8CONbits.TCS = 0;       //Use peripheral bus clock 3 (100MHz)
    T8CONbits.TCKPS = 0b101; //1:32 prescaler
    PR8 = 3125;              //The timer will count exactly (nominally) 1 ms
    T8CONbits.ON = 1;
}

void suspendTimers(){
    T8CONbits.ON = 0;
}

inline void reportActivity(){
    lastActivity = systemMillis;
}

void __ISR(_TIMER_8_VECTOR, ipl1AUTO) _Timer8Handler(void){
    systemMillis++;
    PLIB_INT_SourceFlagClear(INT_ID_0, INT_SOURCE_TIMER_8);
}
