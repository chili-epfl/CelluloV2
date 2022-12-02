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
 * @file cam.c
 * @brief Source for camera functionality
 * @author Ayberk Özgür
 * @date 2015-04-30
 */

#include "cam.h"

#include <xc.h>
#include <sys/attribs.h>
#include <peripheral/dma/plib_dma.h>
#include "system_definitions.h"
#include "power.h"
#include "bluetooth.h"
#include "qml-plugin/include/CelluloBluetoothSharedDefs.h"

unsigned char pixels[IMG_WIDTH_SHARED*IMG_HEIGHT_SHARED];
uint32_t lines_phy[IMG_HEIGHT_SHARED];

bool frameRequest;
bool frameConsumed;
bool camSleepReady;
bool autoexposure;
unsigned int currentRow = 0;
unsigned char camImgMean = 0;
unsigned int currentExposurePixels = CAM_AUTOEXPOSURE_INIT_PIXELS;
unsigned int prevExposurePixels = 0;

CAM_STATE currentCamState = CAM_STATE_INVALID;

inline void camFixSkewedRows(){
    register unsigned char* rowFirstPixel;
    register unsigned char* rowLastValidPixel;
    register unsigned char* rowLastPixel;
    register unsigned char* endPixel = pixels + IMG_HEIGHT_SHARED*IMG_WIDTH_SHARED;
    for(rowFirstPixel = pixels; rowFirstPixel < endPixel; rowFirstPixel += IMG_WIDTH_SHARED){

        //Determine last valid pixel
        rowLastPixel = rowFirstPixel + IMG_WIDTH_SHARED - 1;
        for(rowLastValidPixel = rowLastPixel; rowLastValidPixel >= rowFirstPixel; rowLastValidPixel--)
            if(*rowLastValidPixel != 0)
                break;

        //Shift pixels right, as the invalid pixels were due to pixels dropping in the beginning of line
        if(rowFirstPixel < rowLastValidPixel && rowLastValidPixel < rowLastPixel){
            while(rowLastValidPixel >= rowFirstPixel){
                *rowLastPixel = *rowLastValidPixel;
                rowLastPixel--;
                rowLastValidPixel--;
            }

            //Fill the leftmost with black
            for(; rowLastPixel >= rowFirstPixel; rowLastPixel--)
                *rowLastPixel = 0;
        }
    }
}

void setExposureTime(unsigned int time){
    if(time == 0){
        currentExposurePixels = CAM_AUTOEXPOSURE_INIT_PIXELS;
        autoexposure = true;
    }
    else{
        prevExposurePixels = currentExposurePixels;
        currentExposurePixels = time;
        autoexposure = false;
    }
}

void APP_Cam_Initialize(){
    currentCamState = CAM_STATE_INVALID;
    frameRequest = false;
    frameConsumed = false;
    camSleepReady = true;
    autoexposure = true;
    currentRow = 0;

    //Initialize pixel memory
    int i;
    for(i=0; i<IMG_WIDTH_SHARED*IMG_HEIGHT_SHARED; i++)
        pixels[i] = 0;
    for(i=0; i<IMG_HEIGHT_SHARED; i++)
        lines_phy[i] = ConvertToPhysicalAddress(&pixels[i*IMG_WIDTH_SHARED]);

    //Initialize image sensor
    MT9V034Reset();
    MT9V034OutputEnable(TRUE);
    MT9V034SetBinning(MT9V034_BIN_4, MT9V034_BIN_4);
    MT9V034EnableAGC_AEC(false, false);          //Disable auto gain and exposure
    MT9V034SetManualShutterWidth(CAM_AUTOEXPOSURE_INIT_PIXELS/CAM_AUTOEXPOSURE_NUM_PIXELS_IN_ROW, CAM_AUTOEXPOSURE_INIT_PIXELS%CAM_AUTOEXPOSURE_NUM_PIXELS_IN_ROW);
    MT9V034SetAnalogGain(CAM_ANALOG_GAIN);
    MT9V034SetBlackLevelCalibration(true, 0, 0); //Disable auto black level; does us no good
}

void APP_Cam_Tasks(){

    //Stream frame after it was consumed by localization
    if(frameRequest){
        watchdogDisable();
        camFixSkewedRows();
        int i;
        for(i=0; i<IMG_HEIGHT_SHARED; i++)
            bluetoothEventCameraImageLine(i);
        watchdogEnable();
        frameRequest = false;
    }
}

void updateShutterWidth(){
    if(autoexposure){
        prevExposurePixels = currentExposurePixels;
        currentExposurePixels += (CAM_AUTOEXPOSURE_GOAL_MEAN - (int)camImgMean)*CAM_AUTOEXPOSURE_P;
        if(currentExposurePixels > CAM_AUTOEXPOSURE_MAX_PIXELS)
            currentExposurePixels = CAM_AUTOEXPOSURE_MAX_PIXELS;
        else if(currentExposurePixels < CAM_AUTOEXPOSURE_MIN_PIXELS)
            currentExposurePixels = CAM_AUTOEXPOSURE_MIN_PIXELS;
    }

    if(currentExposurePixels != prevExposurePixels)
        if(!MT9V034SetManualShutterWidth(currentExposurePixels/CAM_AUTOEXPOSURE_NUM_PIXELS_IN_ROW, currentExposurePixels%CAM_AUTOEXPOSURE_NUM_PIXELS_IN_ROW)){

            //Reset I2C if write failed
            DRV_I2C0_DeInitialize();
            Nop(); Nop();
            DRV_I2C0_Initialize();
            Nop(); Nop();
            currentExposurePixels = prevExposurePixels;
        }
}

void suspendCam(){
    MT9V034Standby(true);
}

void resumeCam(){
    currentCamState = CAM_STATE_INVALID;
    MT9V034Standby(false);
    SYS_INT_SourceEnable(INT_SOURCE_EXTERNAL_3);
    SYS_INT_SourceEnable(INT_SOURCE_EXTERNAL_4);
    MT9V034OutputEnable(true);
}

bool grabOneFrame(){
    unsigned int i;

    bool idleRequestedOld = idleRequested;

    //Exit standby mode for now
    resumeCam();

    //Very first frame after standby exit is over-exposed, probably due to internal
    //exposure shadow register not being conserved during standby; we discard it
    idleRequested = false;
    currentCamState = CAM_STATE_GRABBING;
    i = 0;
    while(currentCamState == CAM_STATE_GRABBING){
        i++;
        if(i>10000000){
            suspendCam();
            return false;
        }
    }
    idleRequested = idleRequestedOld;

    //This is the real frame
    currentCamState = CAM_STATE_GRABBING;
    i = 0;
    while(currentCamState == CAM_STATE_GRABBING){
        i++;
        if(i>10000000){
            suspendCam();
            return false;
        }
    }

    //Back into standby mode
    suspendCam();

    return true;
}

void __ISR(_EXTERNAL_3_VECTOR, IPL7AUTO) _LineStartHandler(void){
    if(currentCamState == CAM_STATE_GRABBING)
        DCH0CONSET = DCHCON_CHEN_MASK;

    IFS0CLR = 0b00000000000001000000000000000000;
}

void __ISR(_DMA0_VECTOR, IPL6AUTO) _LineFinishedHandler(void){
    currentRow++;

    //Frame complete
    if(currentRow >= IMG_HEIGHT_SHARED){
        currentCamState = CAM_STATE_READY;
        frameConsumed = false;
        if(sleepRequested || idleRequested){
            MT9V034OutputEnable(false);
            SYS_INT_SourceDisable(INT_SOURCE_EXTERNAL_3);
            SYS_INT_SourceDisable(INT_SOURCE_EXTERNAL_4);
            camSleepReady = true;
        }
        BT_RX_ENABLE();
    }

    //Frame ongoing
    else
        DCH0DSA = lines_phy[currentRow];

    DCH0INTCLR = 0x000000ff;
    IFS4CLR = 0b00000000000000000000000001000000;
}

void __ISR(_EXTERNAL_4_VECTOR, IPL6AUTO) _FrameStartHandler(void){
    if(!frameRequest){
        BT_RX_DISABLE();
        currentRow = 0;

        //Start DMA transfer
        DCH0DSA = lines_phy[0];
        currentCamState = CAM_STATE_GRABBING;
        camSleepReady = false;
    }

    IFS0CLR = 0b00000000100000000000000000000000;
}
