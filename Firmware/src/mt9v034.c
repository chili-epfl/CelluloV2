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
 * @file mt9v034.c
 * @brief Source for MT9V034 image sensor functionality
 * @author Ayberk Özgür
 * @date 2015-04-30
 */

#include "mt9v034.h"
#include "power.h"

void MT9V034Reset(){
    int i;

    //These pins' default LAT settings seem to get corrupted (on low battery/brownout/low VDD maybe?)
    PLIB_PORTS_PinClear(PORTS_ID_0, IMG_EXPOSURE_PORT_CHANNEL, IMG_EXPOSURE_PORT_BIT_POS);
    PLIB_PORTS_PinClear(PORTS_ID_0, IMG_STANDBY_PORT_CHANNEL, IMG_STANDBY_PORT_BIT_POS);

    //Reset sequence
    PLIB_PORTS_PinClear(PORTS_ID_0, IMG_RESET_BAR_PORT_CHANNEL, IMG_RESET_BAR_PORT_BIT_POS);
    for(i=0; i<10000000; i++) Nop(); //TODO: CHECK THIS, REDUCE IF POSSIBLE
    PLIB_PORTS_PinSet(PORTS_ID_0, IMG_RESET_BAR_PORT_CHANNEL, IMG_RESET_BAR_PORT_BIT_POS);
    for(i=0; i<10000000; i++) Nop(); //TODO: CHECK THIS, REDUCE IF POSSIBLE

    //Clear bit 9 of CHIP CONTROL register
    MT9V034Register reg;
    if(!I2CReadBytes(MT9V034_I2C_SLAVE_WRITE_ADDR, MT9V034_REG_ADDR_CHIP_CONTROL, 2, (unsigned char*)&reg))
        softwareReset();
    reg.Bits.B9 = 0;                //TODO: CHECK IF THIS IS REALLY NECESSARY ANYMORE
    if(!I2CWriteBytes(MT9V034_I2C_SLAVE_WRITE_ADDR, MT9V034_REG_ADDR_CHIP_CONTROL, 2, (unsigned char*)&reg))
        softwareReset();
    for(i=0; i<1000000; i++) Nop(); //TODO: CHECK THIS, REDUCE IF POSSIBLE
}

void MT9V034Standby(BOOL enable){
    int i;
    if(enable){
        PLIB_PORTS_PinSet(PORTS_ID_0, IMG_STANDBY_PORT_CHANNEL, IMG_STANDBY_PORT_BIT_POS);



        //TODO: CHECK THIS:
        for(i=0; i<2000000; i++) ; //Wait at least (752/2)*(480/2)/2 SYSCLK cycles
    }
    else{
        for(i=0; i<1000; i++) ;    //Wait at least 10 SYSCLK cycles
        PLIB_PORTS_PinClear(PORTS_ID_0, IMG_STANDBY_PORT_CHANNEL, IMG_STANDBY_PORT_BIT_POS);
        for(i=0; i<1000; i++) ;    //Wait at least 10 SYSCLK cycles
    }
}

void MT9V034OutputEnable(BOOL enable){
    if(enable)
        PLIB_PORTS_PinSet(PORTS_ID_0, IMG_OE_PORT_CHANNEL, IMG_OE_PORT_BIT_POS);
    else
        PLIB_PORTS_PinClear(PORTS_ID_0, IMG_OE_PORT_CHANNEL, IMG_OE_PORT_BIT_POS);
}

void MT9V034SetBinning(MT9V034_BINNING rowBin, MT9V034_BINNING colBin){
    MT9V034Register reg;
    if(!I2CReadBytes(MT9V034_I2C_SLAVE_WRITE_ADDR, MT9V034_REG_ADDR_READ_MODE_CONTEXT_A, 2, (unsigned char*)&reg))
        softwareReset();

    switch(rowBin){
        case MT9V034_NO_BINNING:
            reg.Bits.B1 = 0; reg.Bits.B0 = 0;
            break;
        case MT9V034_BIN_2:
            reg.Bits.B1 = 0; reg.Bits.B0 = 1;
            break;
        case MT9V034_BIN_4:
            reg.Bits.B1 = 1; reg.Bits.B0 = 0;
            break;
        default:
            break;
    }

    switch(colBin){
        case MT9V034_NO_BINNING:
            reg.Bits.B3 = 0; reg.Bits.B2 = 0;
            break;
        case MT9V034_BIN_2:
            reg.Bits.B3 = 0; reg.Bits.B2 = 1;
            break;
        case MT9V034_BIN_4:
            reg.Bits.B3 = 1; reg.Bits.B2 = 0;
            break;
        default:
            break;
    }

    if(!I2CWriteBytes(MT9V034_I2C_SLAVE_WRITE_ADDR, MT9V034_REG_ADDR_READ_MODE_CONTEXT_A, 2, (unsigned char*)&reg))
        softwareReset();
}

void MT9V034EnableAGC_AEC(bool enableAGC, bool enableAEC){
    MT9V034Register reg;
    if(!I2CReadBytes(MT9V034_I2C_SLAVE_WRITE_ADDR, MT9V034_REG_ADDR_AEC_AGC_ENABLE_A_B, 2, (unsigned char*)&reg))
        softwareReset();
    reg.Bits.B0 = enableAEC;
    reg.Bits.B1 = enableAGC;
    if(!I2CWriteBytes(MT9V034_I2C_SLAVE_WRITE_ADDR, MT9V034_REG_ADDR_AEC_AGC_ENABLE_A_B, 2, (unsigned char*)&reg))
        softwareReset();
}

bool MT9V034SetManualShutterWidth(unsigned short coarse, unsigned short fine){
    if(coarse > 32765)
        coarse = 32765;
    if(fine > 1774)
        fine = 1774;
    else if(fine < 260 && coarse == 0)
        fine = 260;

    MT9V034Register reg;

    //Coarse
    reg.Bytes.Low = coarse % 0x0100;
    reg.Bytes.High = (coarse >> 8) & 0x00FF;
    if(!I2CWriteBytes(MT9V034_I2C_SLAVE_WRITE_ADDR, MT9V034_REG_ADDR_COARSE_TOTAL_SHUTTER_WIDTH_CONTEXT_A, 2, (unsigned char*)&reg))
        return false;

    //Fine
    reg.Bytes.Low = fine % 0x0100;
    reg.Bytes.High = (fine >> 8) & 0x00FF;
    if(!I2CWriteBytes(MT9V034_I2C_SLAVE_WRITE_ADDR, MT9V034_REG_ADDR_FINE_SHUTTER_WIDTH_TOTAL_CONTEXT_A, 2, (unsigned char*)&reg))
        return false;

    return true;
}

void MT9V034SetAnalogGain(unsigned char gain){
    if(gain < 16)
        gain = 16;
    else if(gain > 64)
        gain = 64;

    MT9V034Register reg;
    if(!I2CReadBytes(MT9V034_I2C_SLAVE_WRITE_ADDR, MT9V034_REG_ADDR_ANALOG_GAIN_CONTEXT_A, 2, (unsigned char*)&reg))
        softwareReset();
    reg.Bytes.Low = gain;
    if(!I2CWriteBytes(MT9V034_I2C_SLAVE_WRITE_ADDR, MT9V034_REG_ADDR_ANALOG_GAIN_CONTEXT_A, 2, (unsigned char*)&reg))
        softwareReset();
}

void MT9V034SetBlackLevelCalibration(bool manual, unsigned char numFramesLog2, char manualOffset){
    if(numFramesLog2 > 7)
        numFramesLog2 = 7;
    if(manualOffset < -127)
        manualOffset = -127;

    MT9V034Register reg;

    //Control
    if(!I2CReadBytes(MT9V034_I2C_SLAVE_WRITE_ADDR, MT9V034_REG_ADDR_BL_CALIB_CONTROL, 2, (unsigned char*)&reg))
        softwareReset();
    reg.Bits.B0 = manual;
    reg.Bits.B5 = numFramesLog2 % 2;
    numFramesLog2 = numFramesLog2 >> 1;
    reg.Bits.B6 = numFramesLog2 % 2;
    numFramesLog2 = numFramesLog2 >> 1;
    reg.Bits.B7 = numFramesLog2 % 2;
    if(!I2CWriteBytes(MT9V034_I2C_SLAVE_WRITE_ADDR, MT9V034_REG_ADDR_BL_CALIB_CONTROL, 2, (unsigned char*)&reg))
        softwareReset();

    //Manual offset
    if(!I2CReadBytes(MT9V034_I2C_SLAVE_WRITE_ADDR, MT9V034_REG_ADDR_BLACK_LEVEL_CALIBRATION_VALUE, 2, (unsigned char*)&reg))
        softwareReset();
    reg.Bytes.Low = manualOffset;
    if(!I2CWriteBytes(MT9V034_I2C_SLAVE_WRITE_ADDR, MT9V034_REG_ADDR_BLACK_LEVEL_CALIBRATION_VALUE, 2, (unsigned char*)&reg))
        softwareReset();
}

void MT9V034SetRowNoiseCorrection(bool enable, bool useGlobalBlackLevelAverage, unsigned short rowNoiseConstant){
    if(rowNoiseConstant > 1023)
        rowNoiseConstant = 1023;

    MT9V034Register reg;

    //Control
    if(!I2CReadBytes(MT9V034_I2C_SLAVE_WRITE_ADDR, MT9V034_REG_ADDR_ROW_NOISE_CORR_CONTROL, 2, (unsigned char*)&reg))
        softwareReset();
    reg.Bits.B0 = enable;
    reg.Bits.B1 = useGlobalBlackLevelAverage;
    if(!I2CWriteBytes(MT9V034_I2C_SLAVE_WRITE_ADDR, MT9V034_REG_ADDR_ROW_NOISE_CORR_CONTROL, 2, (unsigned char*)&reg))
        softwareReset();

    //Manual offset
    if(!I2CReadBytes(MT9V034_I2C_SLAVE_WRITE_ADDR, MT9V034_REG_ADDR_ROW_NOISE_CONSTANT, 2, (unsigned char*)&reg))
        softwareReset();
    reg.Bytes.Low = rowNoiseConstant % 0x0100;
    reg.Bytes.High = rowNoiseConstant >> 8;
    if(!I2CWriteBytes(MT9V034_I2C_SLAVE_WRITE_ADDR, MT9V034_REG_ADDR_ROW_NOISE_CONSTANT, 2, (unsigned char*)&reg))
        softwareReset();
}
