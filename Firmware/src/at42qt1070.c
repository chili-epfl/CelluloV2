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
 * @file at42qt1070.c
 * @brief Source for AT42QT1070 touch sensor functionality
 * @author Ayberk Özgür
 * @date 2015-05-03
 */

#include "at42qt1070.h"

#include "util.h"
#include "power.h"

unsigned char keyStateBuf[2];
unsigned char* keyState;

void AT42QT1070Init(){
    keyState = &keyStateBuf[1];
}

void AT42QT1070Reset(){

    //For now, HW reset doesn't work. We will SW reset instead.

    //Reset sequence
    //PLIB_PORTS_PinClear(PORTS_ID_0, TOUCH_RESET_BAR_PORT_CHANNEL, TOUCH_RESET_BAR_PORT_BIT_POS);
    //for(i=0;i<1000000;i++) Nop();
    //PLIB_PORTS_PinSet(PORTS_ID_0, TOUCH_RESET_BAR_PORT_CHANNEL, TOUCH_RESET_BAR_PORT_BIT_POS);
    //for(i=0;i<1000000;i++) Nop();
    unsigned char c = 0xFF;
    if(!I2CWriteBytes(AT42QT1070_I2C_SLAVE_WRITE_ADDR, AT42QT1070_REG_ADDR_RESET_BAR, 1, (unsigned char*)&c))
        softwareReset();
    delayTenNanos(50000000); //Delay 500 ms //TODO: CHECK THIS, REDUCE IF POSSIBLE
}

void AT42QT1070SetAKS(unsigned char key, unsigned char group){
    if(group > 3 ||key > 6)
        return;

    unsigned char regAddr = AT42QT1070_REG_ADDR_AVE_AKS_KEY_0 + key;
    unsigned char regVal;
    if(!I2CReadBytes(AT42QT1070_I2C_SLAVE_WRITE_ADDR, regAddr, 1, &regVal))
        softwareReset();
    regVal = (regVal & 0b11111100) + group;
    if(!I2CWriteBytes(AT42QT1070_I2C_SLAVE_WRITE_ADDR, regAddr, 1, &regVal))
        softwareReset();
}

void AT42QT1070SetAVE(unsigned char key, unsigned char ave){
    if(key > 6 || (ave != 0 && ave != 1 && ave != 2 && ave != 4 && ave != 8 && ave != 16 && ave != 32))
        return;

    unsigned char regAddr = AT42QT1070_REG_ADDR_AVE_AKS_KEY_0 + key;
    unsigned char regVal;
    if(!I2CReadBytes(AT42QT1070_I2C_SLAVE_WRITE_ADDR, regAddr, 1, &regVal))
        softwareReset();
    regVal = (regVal & 0b00000011) + (ave << 2);
    if(!I2CWriteBytes(AT42QT1070_I2C_SLAVE_WRITE_ADDR, regAddr, 1, &regVal))
        softwareReset();
}

void AT42QT1070DisableKey(unsigned char key){
    AT42QT1070SetAVE(key, 0);
}

void AT42QT1070SetMaxOnDuration(unsigned char oneHundredSixtyMillis){
    if(!I2CWriteBytes(AT42QT1070_I2C_SLAVE_WRITE_ADDR, AT42QT1070_REG_ADDR_MAX_ON_DURATION, 1, &oneHundredSixtyMillis))
        softwareReset();
}

inline void AT42QT1070DisableMaxOnDuration(){
    AT42QT1070SetMaxOnDuration(0);
}

void AT42QT1070SetSamplePeriod(unsigned char eightMillis){
    if(!I2CWriteBytes(AT42QT1070_I2C_SLAVE_WRITE_ADDR, AT42QT1070_REG_ADDR_LP, 1, &eightMillis))
        softwareReset();
}

bool AT42QT1070GetKeyStates(bool* keys){
    if(!I2CReadBytes(AT42QT1070_I2C_SLAVE_WRITE_ADDR, AT42QT1070_REG_ADDR_DETECTION_STATUS, 2, keyStateBuf))
        return false;
    keys[0] = (*keyState & 0b00000001) >> 0;
    keys[1] = (*keyState & 0b00000010) >> 1;
    keys[2] = (*keyState & 0b00000100) >> 2;
    keys[3] = (*keyState & 0b00001000) >> 3;
    keys[4] = (*keyState & 0b00010000) >> 4;
    keys[5] = (*keyState & 0b00100000) >> 5;
    return true;
}

bool AT42QT1070GetKeySignals(unsigned short* signals){

    //Read all 12 bytes at once
    unsigned char signalsBigEndian[12];
    if(I2CReadBytes(AT42QT1070_I2C_SLAVE_WRITE_ADDR, AT42QT1070_REG_ADDR_KEY_SINGAL_0, 12, signalsBigEndian)){
        unsigned char* signalsChar = (unsigned char*)signals;

        signalsChar[0] = signalsBigEndian[1];
        signalsChar[1] = signalsBigEndian[0];

        signalsChar[2] = signalsBigEndian[3];
        signalsChar[3] = signalsBigEndian[2];

        signalsChar[4] = signalsBigEndian[5];
        signalsChar[5] = signalsBigEndian[4];

        signalsChar[6] = signalsBigEndian[7];
        signalsChar[7] = signalsBigEndian[6];

        signalsChar[8] = signalsBigEndian[9];
        signalsChar[9] = signalsBigEndian[8];

        signalsChar[10] = signalsBigEndian[11];
        signalsChar[11] = signalsBigEndian[10];

        return true;
    }
    else
        return false;
}

bool AT42QT1070GetKeyReferences(unsigned short* references){

    //Read all 12 bytes at once
    unsigned char referencesBigEndian[12];
    if(I2CReadBytes(AT42QT1070_I2C_SLAVE_WRITE_ADDR, AT42QT1070_REG_ADDR_REFERENCE_DATA_0, 12, referencesBigEndian)){
        unsigned char* referencesChar = (unsigned char*)references;

        referencesChar[0] = referencesBigEndian[1];
        referencesChar[1] = referencesBigEndian[0];

        referencesChar[2] = referencesBigEndian[3];
        referencesChar[3] = referencesBigEndian[2];

        referencesChar[4] = referencesBigEndian[5];
        referencesChar[5] = referencesBigEndian[4];

        referencesChar[6] = referencesBigEndian[7];
        referencesChar[7] = referencesBigEndian[6];

        referencesChar[8] = referencesBigEndian[9];
        referencesChar[9] = referencesBigEndian[8];

        referencesChar[10] = referencesBigEndian[11];
        referencesChar[11] = referencesBigEndian[10];

        return true;
    }
    else
        return false;
}
