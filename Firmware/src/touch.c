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
 * @file touch.c
 * @brief Source for touch input functionality
 * @author Ayberk Özgür
 * @date 2015-05-03
 */

#include "touch.h"
#include "bluetooth.h"

bool keyTouchStates[6] = {false, false, false, false, false, false};
bool keyTouchStatesNew[6];
bool keyTouchLong[6] = {false, false, false, false, false, false};

KEY_EVENT keyEvent[6];

int keyTouchStartTime[6] = {-1,-1,-1,-1,-1,-1};
bool sleepRequestedTouch = false;
int threeFinger024TouchBeginTime = -1;
int threeFinger135TouchBeginTime = -1;
int fourOrMoreFingerTouchBeginTime = -1;

void APP_Touch_Initialize(){
    AT42QT1070Init();

    AT42QT1070Reset();

    //Disable AKS to make multitouch possible
    AT42QT1070SetAKS(0,0);
    AT42QT1070SetAKS(1,0);
    AT42QT1070SetAKS(2,0);
    AT42QT1070SetAKS(3,0);
    AT42QT1070SetAKS(4,0);
    AT42QT1070SetAKS(5,0);

    //We don't use key 6
    AT42QT1070DisableKey(6);

    //Set 8ms sampling period
    AT42QT1070SetSamplePeriod(TOUCH_SAMPLE_PERIOD_8_MS);

    //Disable max on duration
    AT42QT1070DisableMaxOnDuration();
}

inline unsigned char numKeysTouched(){
    return
        (keyTouchStates[0] ? 1 : 0) +
        (keyTouchStates[1] ? 1 : 0) +
        (keyTouchStates[2] ? 1 : 0) +
        (keyTouchStates[3] ? 1 : 0) +
        (keyTouchStates[4] ? 1 : 0) +
        (keyTouchStates[5] ? 1 : 0);
}

bool sampleKeyStates(){
    unsigned int i = 0;
    while(!PLIB_PORTS_PinGet(PORTS_ID_0, TOUCH_CHANGE_BAR_PORT_CHANNEL, TOUCH_CHANGE_BAR_PORT_BIT_POS)){
        if(!AT42QT1070GetKeyStates(keyTouchStatesNew))
            i++;
        if(i>100){

            //Reset I2C, it's broken
            DRV_I2C0_DeInitialize();
            Nop(); Nop();
            DRV_I2C0_Initialize();
            Nop(); Nop();
            return false;
        }
    }
    return true;
}

void touchQueryKeys(){
    if(!sampleKeyStates())
        return;

    //Decode taps and tap times
    unsigned int i;
    for(i=0; i<6; i++){
        if(keyTouchStatesNew[i] && !keyTouchStates[i]){ //Just started touching
            keyTouchStartTime[i] = systemMillis;
            keyEvent[i] = KEY_EVENT_TOUCH_BEGIN;
        }
        else if(!keyTouchStatesNew[i] && keyTouchStates[i]){ //Released
            keyEvent[i] = KEY_EVENT_RELEASE;
            keyTouchLong[i] = false;
        }
        else if(keyTouchStatesNew[i] && keyTouchStates[i]){ //Keep touching
            keyEvent[i] = KEY_EVENT_TOUCHING;
            if(keyTouchStartTime[i] >= 0 && systemMillis - keyTouchStartTime[i] >= LONG_TOUCH_MILLIS){
                keyEvent[i] = KEY_EVENT_LONG_TOUCH;
                keyTouchLong[i] = true;
                keyTouchStartTime[i] = -1;
            }
        }
        else
            keyEvent[i] = KEY_EVENT_NONE;

        keyTouchStates[i] = keyTouchStatesNew[i];
    }
}

void suspendTouchForSleep(){
    unsigned int i;
    for(i=0; i<6; i++){
        keyTouchLong[i] = false;
        keyTouchStates[i] = false;
    }
}

bool getThreeFingerDeathPunch(){

    //Check for three finger death punches
    if( keyTouchStates[0] && keyTouchStates[2] && keyTouchStates[4] &&
        !keyTouchStates[1] && !keyTouchStates[3] && !keyTouchStates[5]){
        if(threeFinger024TouchBeginTime < 0)
            threeFinger024TouchBeginTime = systemMillis;
    }
    else
        threeFinger024TouchBeginTime = -1;
    if( !keyTouchStates[0] && !keyTouchStates[2] && !keyTouchStates[4] &&
        keyTouchStates[1] && keyTouchStates[3] && keyTouchStates[5]){
        if(threeFinger135TouchBeginTime < 0)
            threeFinger135TouchBeginTime = systemMillis;
    }
    else
        threeFinger135TouchBeginTime = -1;

    //Request whether punched long enough
    return (threeFinger024TouchBeginTime >= 0 && systemMillis - threeFinger024TouchBeginTime >= SLEEP_WAKEUP_HOLD_MILLIS) ||
           (threeFinger135TouchBeginTime >= 0 && systemMillis - threeFinger135TouchBeginTime >= SLEEP_WAKEUP_HOLD_MILLIS);
}

bool getFourOrMoreFingerHold(){

    //Check for four or more finger touches
    unsigned int i, numKeysTouched = 0;
    for(i=0; i<6; i++)
        if(keyTouchStates[i])
            numKeysTouched++;
    if(numKeysTouched >= 4){
        if(fourOrMoreFingerTouchBeginTime < 0)
            fourOrMoreFingerTouchBeginTime = systemMillis;
    }
    else
        fourOrMoreFingerTouchBeginTime = -1;

    //Request whether punched long enough
    return fourOrMoreFingerTouchBeginTime >= 0 && systemMillis - fourOrMoreFingerTouchBeginTime >= SLEEP_WAKEUP_HOLD_MILLIS;
}

bool stayAsleepTouch(){
    unsigned int i;
    for(i=0; i<6; i++){
        keyTouchStartTime[i] = -1;
        keyTouchStates[i] = false;
    }

    int timeSinceNotFourFingerHold = -1;
    while(1){
        touchQueryKeys();

        if(getFourOrMoreFingerHold())
            return false;

        if(fourOrMoreFingerTouchBeginTime < 0){
            if(timeSinceNotFourFingerHold < 0)
                timeSinceNotFourFingerHold = systemMillis;
        }
        else
            timeSinceNotFourFingerHold = -1;

        if(timeSinceNotFourFingerHold >= 0 && systemMillis - timeSinceNotFourFingerHold > 1000)
            return true;
    }
}

void APP_Touch_Tasks(){

    //Query regular binary key states
    touchQueryKeys();

    //Send events first
    unsigned int i;
    for(i=0; i<6; i++){
        if(keyEvent[i] == KEY_EVENT_TOUCH_BEGIN){
            keyTouched(i, true);
            bluetoothEventTouchBegin(i);
        }
        else if(keyEvent[i] == KEY_EVENT_LONG_TOUCH)
            bluetoothEventTouchLongPressed(i);
        else if(keyEvent[i] == KEY_EVENT_RELEASE){
            keyTouched(i, false);
            bluetoothEventTouchReleased(i);
        }
    }

    //Request sleep if punched
    if(getThreeFingerDeathPunch()){
        if(!sleepRequestedTouch){
            bluetoothEventShuttingDown();
            requestSleep();
            sleepRequestedTouch = true;
        }
    }
    else
        sleepRequestedTouch = false;
}
