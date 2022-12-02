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
 * @file localization.c
 * @brief Source for pattern localization
 * @author Lukas Hostettler
 * @date 2015-05-19
 */

#include "localization.h"

#define abs(X) ((X) >= 0 ? (X) : -(X))
#define max(X,Y) ((X) > (Y) ? (X) : (Y))

DecodingError poseDecodingError = NO_INFO;
int poseX = 0;
int poseY = 0;
int poseTheta = 0;
float poseThetaRad = 0.0f;
bool kidnapped = true;
unsigned int poseTimestamp = 0;
bool timestamping = false;
unsigned int bcastPeriod = 100; //10Hz default

DotsPositionInfo * posInfo;
int poseXOld = 0;
int poseYOld = 0;
int poseThetaOld = 0;

bool updatePose(){
    posInfo = dotsLocalize(posInfo,pixels,IMG_HEIGHT_SHARED,IMG_WIDTH_SHARED);
    camImgMean = posInfo->imgMean;

    poseDecodingError = dotsGetError(posInfo);
    if(poseDecodingError == DECODING_NO_ERROR)
        poseTimestamp = systemMillis;

    poseX = dotsGetIntX(posInfo);
    poseY = dotsGetIntY(posInfo);
    poseThetaRad = -dotsGetAngle(posInfo); //Correct wrong angle convention here
    poseTheta = (int)(100*poseThetaRad*180/3.14159f);
    if(poseTheta < 0)
        poseTheta += 36000;

    //Pose is valid, update and return whether there is little difference from previous
    if(!dotsIsAboveGround(posInfo)){
        bool xStayedSame = abs(poseX - poseXOld) < X_Y_DISTANCE_TO_WAKE_UP;
        bool yStayedSame = abs(poseY - poseYOld) < X_Y_DISTANCE_TO_WAKE_UP;
        bool thetaStayedSame = abs(poseTheta - poseThetaOld) < THETA_DISTANCE_TO_WAKE_UP;

        if(!xStayedSame)
            poseXOld = poseX;
        if(!yStayedSame)
            poseYOld = poseY;
        if(!thetaStayedSame)
            poseThetaOld = poseTheta;

        return xStayedSame && yStayedSame && thetaStayedSame;
    }

    //Pose is invalid, if were not kidnapped before, we should wake up
    else
        return kidnapped;
}

inline bool stayIdleCV(){
    return updatePose();
}

void APP_Localization_Initialize(){
    posInfo=NULL;
}

unsigned int i=0;
unsigned int emptyFrames = 0;
unsigned int lastPoseUpdate = 0;
unsigned int lastPoseBcast = 0;
bool idling = false;

inline void sendPose(){
    if(systemMillis >= lastPoseBcast + bcastPeriod){
        if(timestamping)
            bluetoothEventPoseChangedTimestamped(poseX, poseY, poseTheta);
        else
            bluetoothEventPoseChanged(poseX, poseY, poseTheta);
        lastPoseBcast = systemMillis;
    }
    reportActivity();
}

void APP_Localization_Tasks(){
    if(currentCamState == CAM_STATE_READY && !frameConsumed){
        camFixSkewedRows();
        bool poseStayedSame = updatePose();
        updateShutterWidth();

        //Pose valid
        if(!dotsIsAboveGround(posInfo) && poseX >= 0 && poseY >= 0){
            emptyFrames = 0;
            if(kidnapped)
                bluetoothEventKidnapChanged(false);
            kidnapped = false;

            //Pose changed, send pose as usual
            if(!poseStayedSame){
                lastPoseUpdate = systemMillis;
                sendPose();
                idling = false;
            }

            //Pose stayed the same
            else{

                //If idling, send pose periodically as only a heartbeat
                if(idling){
                    if(timestamping)
                        idling = false;
                    if(systemMillis >= lastPoseUpdate + max(POSE_PUBLISH_PERIOD, bcastPeriod)){
                        lastPoseUpdate = systemMillis;
                        if(timestamping)
                            bluetoothEventPoseChangedTimestamped(poseX, poseY, poseTheta);
                        else
                            bluetoothEventPoseChanged(poseX, poseY, poseTheta);
                    }
                }

                //Decide whether to start idling
                else{
                    sendPose();
                    if(systemMillis >= lastPoseUpdate + TIME_BEFORE_IDLING && !timestamping)
                        idling = true;
                }
            }
        }

        //Pose invalid this frame
        else{
            emptyFrames++;
            if(emptyFrames >= EMPTY_FRAMES_FOR_KIDNAP && !kidnapped){
                kidnapped = true;
                bluetoothEventKidnapChanged(true);
            }
        }

        frameConsumed = true;
    }
}
