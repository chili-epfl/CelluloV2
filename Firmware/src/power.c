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
 * @file power.c
 * @brief Source for power and reset functionality
 * @author Ayberk Özgür
 * @date 2015-05-17
 */

#include "power.h"
#include "led.h"
#include "mt9v034.h"
#include "cam.h"
#include "bluetooth.h"
#include "localization.h"
#include "watchdog.h"
#include "motor.h"

BOOT_STATE bootState = BOOT_STATE_INVALID;
bool sleepRequested;
bool idleRequested;
bool exitIdleByTimer;

void softwareReset(){
    SYSKEY = 0x00000000; //Write invalid key to force lock
    SYSKEY = 0xAA996655; //Write Key1 to SYSKEY
    SYSKEY = 0x556699AA; //Write Key2 to SYSKEY

    //OSCCON is now unlocked

    RSWRSTSET = 1;  //set SWRST bit to arm reset
    unsigned int dummy;
    dummy = RSWRST; //Read RSWRST register to trigger reset
    while(1) ;      //prevent any unwanted code execution until reset occurs
}

inline void sleep(){
    SYSKEY = 0x00000000; //Write invalid key to force lock
    SYSKEY = 0xAA996655; //Write Key1 to SYSKEY
    SYSKEY = 0x556699AA; //Write Key2 to SYSKEY

    //OSCCON is now unlocked

    OSCCONbits.SLPEN = 1; //Enable sleep when WAIT is executed

    SYSKEY = 0x00000000;  //Write invalid key to force lock

    asm volatile ("wait");
}

inline void idle(){
    SYSKEY = 0x00000000; //Write invalid key to force lock
    SYSKEY = 0xAA996655; //Write Key1 to SYSKEY
    SYSKEY = 0x556699AA; //Write Key2 to SYSKEY

    //OSCCON is now unlocked

    OSCCONbits.SLPEN = 0; //Enable idle when WAIT is executed

    SYSKEY = 0x00000000;  //Write invalid key to force lock

    asm volatile ("wait");
}

void requestSleep(){
    sleepRequested = true;
}

void requestIdle(){

    //These effects are dynamic
    if(visualEffect != VisualEffectAlertAll &&
       visualEffect != VisualEffectAlertSingle &&
       visualEffect != VisualEffectWaiting &&
       visualEffect != VisualEffectBlink &&
       visualEffect != VisualEffectBreathe &&
       visualEffect != VisualEffectPulse)
        idleRequested = true;
}

inline void enableSleepExitTouch(){

    //If TOUCH_CHANGE_BAR is not cleared, exit from sleep is not recoverable
    if(!sampleKeyStates())
        softwareReset();

    //Enable CN interrupt on TOUCH_CHANGE_BAR
    CNENDbits.CNIED9 = 1;
    PLIB_INT_SourceFlagClear(INT_ID_0, INT_SOURCE_CHANGE_NOTICE_D);
    SYS_INT_SourceEnable(INT_SOURCE_CHANGE_NOTICE_D);
}

inline void enableIdleExitTimer(){
    exitIdleByTimer = false;

    //Enable timer6/7 to exit after 1 second to check cam activity
    T6CON = 0;
    T7CON = 0;
    T6CONbits.TCS = 0;               //Use peripheral bus clock 3 (100MHz)
    T6CONbits.T32 = 1;               //32-bit mode
    T6CONbits.TCKPS = 0b111;         //1:256 prescaler
    PR6 = IDLE_EXIT_TIMER_PERIOD_LO; //The timer will count exactly 1s
    PR7 = IDLE_EXIT_TIMER_PERIOD_HI;
    T6CONbits.ON = 1;
}

inline void enableIdleExitTouch(){

    //Sample key states to clear external TOUCH_CHANGE_BAR; if it fails it is recoverable since we scan again next second
    sampleKeyStates();

    //Enable CN interrupt on TOUCH_CHANGE_BAR
    CNENDbits.CNIED9 = 1;
    PLIB_INT_SourceFlagClear(INT_ID_0, INT_SOURCE_CHANGE_NOTICE_D);
    SYS_INT_SourceEnable(INT_SOURCE_CHANGE_NOTICE_D);
}

inline void disableIdleExitInterrupts(){

    //Disable CN interrupt on TOUCH_CHANGE_BAR
    CNENDbits.CNIED9 = 0;
    SYS_INT_SourceDisable(INT_SOURCE_CHANGE_NOTICE_D);

    //Disable timer6/7
    T6CONbits.ON = 0;
}

void APP_Power_Initialize(){
    sleepRequested = false;
    idleRequested = false;

    //Set TOUCH_CHANGE_BAR interrupt priority
    SYS_INT_VectorPrioritySet(INT_VECTOR_CHANGE_NOTICE_D, INT_PRIORITY_LEVEL7);
    SYS_INT_VectorSubprioritySet(INT_VECTOR_CHANGE_NOTICE_D, INT_SUBPRIORITY_LEVEL3);

    //Set Timer6/7 interrupt priority
    SYS_INT_VectorPrioritySet(INT_VECTOR_T7, INT_PRIORITY_LEVEL7);
    SYS_INT_VectorSubprioritySet(INT_VECTOR_T7, INT_SUBPRIORITY_LEVEL2);
    PLIB_INT_SourceFlagClear(INT_ID_0, INT_SOURCE_TIMER_7);
    SYS_INT_SourceEnable(INT_SOURCE_TIMER_7);

    //Get the boot state, cause of reset
    if(RCONbits.EXTR)
        bootState = BOOT_STATE_MCLR;
    else if(RCONbits.SWR)
        bootState = BOOT_STATE_SOFTWARE_RESET;
    else if(RCONbits.CMR)
        bootState = BOOT_STATE_CFG_WORD_MISMATCH;
    else if(RCONbits.WDTO)
        bootState = BOOT_STATE_WATCHDOG_TIMEOUT;
    else if(RCONbits.DMTO)
        bootState = BOOT_STATE_DEADMAN_TIMEOUT;
    else if(RCONbits.POR)
        bootState = BOOT_STATE_POWER_ON;
    else if(RCONbits.BOR)
        bootState = BOOT_STATE_BROWNOUT;
    else
        bootState = BOOT_STATE_INVALID;

    RCONbits.POR = 0;
    RCONbits.BOR = 0;
    RCONbits.EXTR = 0;
    RCONbits.SWR = 0;
    RCONbits.CMR = 0;
    RCONbits.WDTO = 0;
    RCONbits.DMTO = 0;
}

void APP_Power_Tasks(){

    if(sleepRequested){

        //Wait for LEDs to turn off completely and current image frame to complete
        if(!effectTransition && camSleepReady){
            disableAllMotors();
            suspendLEDsSleep();
            suspendBluetooth();
            suspendCam();

            watchdogDisable();

            do {
                do {
                    suspendTimers();
                    suspendTouchForSleep();

                    //Enter sleep mode
                    enableSleepExitTouch();
                    sleep();
                    //No need to disable touch interrupts, it disables itself
                    //Sleep mode has ended here

                    //Resume timers first because we need them to count touch duration to know whether to exit sleep or not
                    resumeTimers();

                } while(stayAsleepTouch());
            } while(stayAsleepBattery());

            watchdogEnable();

            resumeLEDsSleep();
            resumeBluetooth();
            resumeCam();

            sleepRequested = false;
            reportActivity();
        }
    }
    else if(idleRequested){

        //Wait for LEDs to turn off completely and current image frame to complete
        if(!effectTransition && camSleepReady){
            disableAllMotors();
            suspendLEDsIdle();
            //Do not suspend Bluetooth, we want communication while idle
            suspendTimers();
            suspendCam();

            do {

                //Enter idle mode
                enableIdleExitTimer();
                enableIdleExitTouch();
                idle();
                disableIdleExitInterrupts();
                //Idle mode has ended here
                //Exit from idle mode clears the watchdog timer automatically

                //If we were waken up by touch/bluetooth activity, just wake up and do not check for CV activity
                if(!exitIdleByTimer)
                    break;

                if(!grabOneFrame())
                    softwareReset(); //TODO: IF POSSIBLE, REPLACE THIS WITH MT9V034RESET ONLY
            } while(stayIdleCV() && stayIdleBattery());

            resumeCam();
            resumeTimers();
            resumeLEDsIdle();
            //Do not resume Bluetooth because we don't power it down in the first place

            idleRequested = false;
            reportActivity();
        }
    }
}

void __ISR(_CHANGE_NOTICE_D_VECTOR, ipl7AUTO) _SleepIdleTouchExitHandler(void){
    CNENDbits.CNIED9 = 0;
    SYS_INT_SourceDisable(INT_SOURCE_CHANGE_NOTICE_D);

    PLIB_INT_SourceFlagClear(INT_ID_0, INT_SOURCE_CHANGE_NOTICE_D);
}

void __ISR(_TIMER_7_VECTOR, ipl7AUTO) _IdleTimerExitHandler(void){
    T6CONbits.ON = 0;
    exitIdleByTimer = true;
    PLIB_INT_SourceFlagClear(INT_ID_0, INT_SOURCE_TIMER_7);
}
