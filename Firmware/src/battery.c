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
 * @file battery.c
 * @brief Source for battery functionality
 * @author Ayberk Özgür
 * @date 2015-05-09
 */

#include "battery.h"
#include "led.h"

bool BAT_PG_BAR;
bool BAT_STAT2;
bool BAT_STAT1_LBO;
BatteryState batteryState = BatteryStateShutdown;
int lowBatteryTime = -1;

BatteryState getBatteryState(){
    BatteryState newState = batteryState;
    BAT_PG_BAR = PLIB_PORTS_PinGet(PORTS_ID_0, BAT_PG_BAR_PORT_CHANNEL, BAT_PG_BAR_PORT_BIT_POS);
    BAT_STAT2 = PLIB_PORTS_PinGet(PORTS_ID_0, BAT_STAT2_PORT_CHANNEL, BAT_STAT2_PORT_BIT_POS);
    BAT_STAT1_LBO = PLIB_PORTS_PinGet(PORTS_ID_0, BAT_STAT1_LBO_PORT_CHANNEL, BAT_STAT1_LBO_PORT_BIT_POS);

    if(BAT_PG_BAR){
        if(BAT_STAT1_LBO){
            if(BAT_STAT2)
                newState = BatteryStateDischarging;
            else{
                //TODO: Impossible, throw exception
            }
        }
        else{
            if(BAT_STAT2)
                newState = BatteryStateLow;
            else{
                //TODO: Impossible, throw exception
            }
        }
    }
    else{
        if(BAT_STAT1_LBO){
            if(BAT_STAT2)
                newState = BatteryStateShutdown;
            else
                newState = BatteryStateCharged;
        }
        else{
            if(BAT_STAT2)
                newState = BatteryStateCharging;
            else
                newState = BatteryStateError;
        }
    }

    return newState;
}

void batteryCharge(bool enable){
    if(enable)
        PLIB_PORTS_PinSet(PORTS_ID_0, BAT_CE_PORT_CHANNEL, BAT_CE_PORT_BIT_POS);
    else
        PLIB_PORTS_PinClear(PORTS_ID_0, BAT_CE_PORT_CHANNEL, BAT_CE_PORT_BIT_POS);
}

bool stayAsleepBattery(){
    batteryState = getBatteryState();
    if(batteryState == BatteryStateLow){
        lowBatteryAnimation();
        return true;
    }
    return false;
}

bool stayIdleBattery(){
    return batteryState == getBatteryState();
}

void APP_Battery_Initialize(){
    batteryCharge(true);
    PLIB_PORTS_PinClear(PORTS_ID_0, VBAT_SENSE_ON_PORT_CHANNEL, VBAT_SENSE_ON_PORT_BIT_POS); //ADC silicon errata is a joke on PIC32MZ
    batteryState = getBatteryState();
}

void APP_Battery_Tasks(){
    BatteryState newState = getBatteryState();

    if(newState != batteryState){
        bluetoothEventBatteryStateChanged(newState);
        reportActivity();

        if(newState == BatteryStateCharging)
            setVisualEffect(VisualEffectAlertAll, 0, 255, 0, 2);
        else if(newState == BatteryStateCharged)
            setVisualEffect(VisualEffectAlertAll, 0, 255, 0, 4);
        else if(newState == BatteryStateDischarging)
            setVisualEffect(VisualEffectAlertAll, 255, 255, 0, 2);
        else if(newState == BatteryStateLow){
            setVisualEffect(VisualEffectAlertAll, 255, 0, 0, 10);
            lowBatteryTime = systemMillis;
        }
    }

    if(newState == BatteryStateLow)
        if(lowBatteryTime >= 0 && systemMillis - lowBatteryTime > 300*19){
            bluetoothEventLowBattery();
            requestSleep();
            lowBatteryTime = -1;
        }

    batteryState = newState;
}
