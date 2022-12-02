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
 * @file main.c
 * @brief Source for entry point
 * @author Ayberk Özgür
 * @date 2015-05-28
 */

#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>                     // Defines EXIT_FAILURE
#include "system/common/sys_module.h"

#include "power.h"
#include "timer.h"
#include "battery.h"
#include "led.h"

#include "cam.h"
#include "bluetooth.h"
#include "touch.h"
#include "gesture.h"
#include "localization.h"
#include "watchdog.h"
#include "motor.h"
#include "motion.h"

int main(void){
    SYS_Initialize(NULL);

    APP_Power_Initialize();

    APP_Timer_Initialize();

    APP_Battery_Initialize();

    APP_BT_Initialize();

    APP_LED_Initialize();

    APP_Cam_Initialize();

    //APP_Touch_Initialize();

    APP_Gesture_Initialize();

    APP_Localization_Initialize();

    APP_Motor_Initialize();

    APP_Motion_Initialize();

    SYS_INT_SourceEnable(INT_SOURCE_EXTERNAL_3);
    SYS_INT_SourceEnable(INT_SOURCE_EXTERNAL_4);

    bluetoothEventBootComplete();

    reportActivity();

    APP_Watchdog_Initialize();
    int k =0; 
    for(k=0; k<1000000; k++) ;

    DRV_USART1_Initialize();
    
    while(true){
        SYS_Tasks();

        APP_Power_Tasks();

        APP_Timer_Tasks();

        APP_Battery_Tasks();

        APP_BT_Tasks();

        APP_LED_Tasks();

        APP_Cam_Tasks();
        //APP_Touch_Tasks();
        APP_Gesture_Tasks();
        APP_Localization_Tasks();
        APP_Motor_Tasks();
        APP_Motion_Tasks();

        APP_Watchdog_Tasks();
    }

    return (EXIT_FAILURE);
}

static unsigned int _excep_code;
static unsigned int _excep_addr;
void _general_exception_handler(void){
    asm volatile ("mfc0 %0,$13" : "=r" (_excep_code));
    asm volatile ("mfc0 %0,$14" : "=r" (_excep_addr));

}

void _simple_tlb_refill_exception_handler(void){
}

void _cache_err_exception_handler(void){
}
