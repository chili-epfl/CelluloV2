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
 * @file gesture.c
 * @brief Source for touch key gesture functionality
 * @author Ayberk Özgür
 * @date 2016-06-14
 */

#include "touch.h"
#include "bluetooth.h"
#include "led.h"

bool gestureEnabled = false;
unsigned int lastSignalSampleTime = 0;
unsigned short keySignals[6];
unsigned short keyReferences[6];
short keyRawOffsets[6];
Gesture currentGesture = GestureNone;

void APP_Gesture_Initialize(){
}

inline int getKeyValueSum(){
    return
        keyRawOffsets[0] +
        keyRawOffsets[1] +
        keyRawOffsets[2] +
        keyRawOffsets[3] +
        keyRawOffsets[4] +
        keyRawOffsets[5];
}

bool sampleKeySignals(){
    if(!AT42QT1070GetKeySignals(keySignals) || !AT42QT1070GetKeyReferences(keyReferences)){

        //Reset I2C, it's broken
        DRV_I2C0_DeInitialize();
        Nop(); Nop();
        DRV_I2C0_Initialize();
        Nop(); Nop();
        return false;
    }
    else{
        keyRawOffsets[0] = keySignals[0] - keyReferences[0];
        keyRawOffsets[1] = keySignals[1] - keyReferences[1];
        keyRawOffsets[2] = keySignals[2] - keyReferences[2];
        keyRawOffsets[3] = keySignals[3] - keyReferences[3];
        keyRawOffsets[4] = keySignals[4] - keyReferences[4];
        keyRawOffsets[5] = keySignals[5] - keyReferences[5];
        return true;
    }
}

void APP_Gesture_Tasks(){
    if(!gestureEnabled){
        if(currentGesture != GestureNone){
            currentGesture = GestureNone;
            bluetoothEventGestureChanged(currentGesture);
        }
        return;
    }

    if(systemMillis - lastSignalSampleTime >= 8*TOUCH_SAMPLE_PERIOD_8_MS)
        if(sampleKeySignals()){
            lastSignalSampleTime = systemMillis;

            Gesture newGesture;
            if(getKeyValueSum() >= GESTURE_HOLD_SUM_THRESHOLD)
                newGesture = GestureHold;
            else
                newGesture = GestureNone;
            if(newGesture != currentGesture){
                currentGesture = newGesture;
                bluetoothEventGestureChanged(currentGesture);
            }
        }
}
