/*
 * Copyright (C) 2016 EPFL
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
 * @file haptics.c
 * @brief Source for haptics control
 * @author Ayberk Özgür
 * @date 2016-07-13
 */

#include "haptics.h"

#include "motion.h"
#include "gesture.h"
#include "timer.h"

#define abs(X) ((X) >= 0 ? (X) : -(X))
#define PI 3.14159265359f

#define CASUAL_BDRV_ASSIST_HYSTERESIS_MIN_XY_SQUARED (CASUAL_BDRV_ASSIST_HYSTERESIS_MIN_XY*CASUAL_BDRV_ASSIST_HYSTERESIS_MIN_XY)
#define CASUAL_BDRV_ASSIST_DECAY_XY_KNEE_SQUARED (CASUAL_BDRV_ASSIST_DECAY_XY_KNEE*CASUAL_BDRV_ASSIST_DECAY_XY_KNEE)
#define CASUAL_BDRV_ASSIST_DECAY_W_KNEE_SQUARED (CASUAL_BDRV_ASSIST_DECAY_W_KNEE*CASUAL_BDRV_ASSIST_DECAY_W_KNEE)
#define CASUAL_BDRV_ASSIST_DECAY_W_KNEE_2_SQUARED (CASUAL_BDRV_ASSIST_DECAY_W_KNEE_2*CASUAL_BDRV_ASSIST_DECAY_W_KNEE_2)

#define HAPTIC_BDRV_ASSIST_HYSTERESIS_MIN_XY_SQUARED (HAPTIC_BDRV_ASSIST_HYSTERESIS_MIN_XY*HAPTIC_BDRV_ASSIST_HYSTERESIS_MIN_XY)

float vxGlobalGoalHaptics = 0.0;
float vyGlobalGoalHaptics = 0.0;
float wGlobalGoalHaptics = 0.0;

bool casualBdrvAssistEnabled = false;         ///< Whether casual backdrive is enabled
bool hapticBdrvAssistEnabled = false;         ///< Whether haptic backdrive is enabled
int bdrvAssistVxyHysteresisFrames = 0;        ///< Number of consecutive frames spent staying still/moving in x,y
int bdrvAssistWHysteresisFrames = 0;          ///< Number of consecutive frames spent staying still/moving in w
bool bdrvAssistVxyHysteresisMobile = false;   ///< Whether moving in x,y
bool bdrvAssistWHysteresisMobile = false;     ///< Whether moving in w
float vxGlobalGoalCasualBdrvAssist = 0.0;     ///< X velocity dictated by casual assist
float vyGlobalGoalCasualBdrvAssist = 0.0;     ///< Y velocity dictated by casual assist
float wGlobalGoalCasualBdrvAssist = 0.0;      ///< Angular velocity dictated by casual assist
float hapticBdrvAssistXCoeff = 0.0;           ///< X assist with respect to x drive velocity, can be negative
float hapticBdrvAssistYCoeff = 0.0;           ///< Y assist with respect to y drive velocity, can be negative
float hapticBdrvAssistThetaCoeff = 0.0;       ///< Theta assist with respect to w drive, can be negative
float vxGlobalGoalHapticBdrvAssist = 0.0;     ///< X velocity dictated by haptics assist
float vyGlobalGoalHapticBdrvAssist = 0.0;     ///< Y velocity dictated by haptics assist
float wGlobalGoalHapticBdrvAssist = 0.0;      ///< Angular velocity dictated by haptics assist

bool simpleVibrationEnabled = false;          ///< Whether simple vibration is enabled
unsigned short simpleVibrationPeriod = 0;     ///< Vibration period in milliseconds
unsigned int simpleVibrationEndTime = 0;      ///< Ending time of vibration in milliseconds
unsigned int simpleVibrationInvertTime = 0;   ///< Last time in milliseconds when vibration was inverted
float vxGlobalGoalSimpleVibration = 0.0;      ///< X velocity dictated by vibration
float vyGlobalGoalSimpleVibration = 0.0;      ///< Y velocity dictated by vibration
float wGlobalGoalSimpleVibration = 0.0;       ///< Angular velocity dictated by vibration

bool vibrationOnMotionEnabled = false;        ///< Whether vibration on motion is enabled
unsigned short vibrationOnMotionPeriod = 0;   ///< Vibration period in milliseconds
unsigned int vibrationOnMotionInvertTime = 0; ///< Last time in milliseconds when vibration was inverted
bool vibrationOnMotionPositive = false;       ///< Whether currently positive (i.e on part of duty cycle)
float vibrationOnMotionIntensityCoeff = 0;    ///< Vibration intensity rate with respect to current velocities
float vxGlobalGoalVibrationOnMotion = 0.0;    ///< X velocity dictated by vibration
float vyGlobalGoalVibrationOnMotion = 0.0;    ///< Y velocity dictated by vibration
float wGlobalGoalVibrationOnMotion = 0.0;     ///< Angular velocity dictated by vibration

void casualBackdriveAssistEnable(bool enabled){
    if(enabled)
        hapticBdrvAssistEnabled = false;
    casualBdrvAssistEnabled = enabled;
}

void hapticBackdriveAssist(float xCoeff, float yCoeff, float thetaCoeff){
    if(xCoeff == 0.0f && yCoeff == 0.0f && thetaCoeff == 0.0f)
        hapticBdrvAssistEnabled = false;
    else{
        hapticBdrvAssistXCoeff = xCoeff;
        hapticBdrvAssistYCoeff = yCoeff;
        hapticBdrvAssistThetaCoeff = thetaCoeff;
        hapticBdrvAssistEnabled = true;
        casualBdrvAssistEnabled = false;
    }
}

void simpleVibrate(float iX, float iY, float iTheta, unsigned short period, unsigned short duration){
    if(!simpleVibrationEnabled){
        if(vxGlobalMeasuredSmoothed > 0)
            vxGlobalGoalSimpleVibration = -iX;
        else
            vxGlobalGoalSimpleVibration = iX;
        if(vyGlobalMeasuredSmoothed > 0)
            vyGlobalGoalSimpleVibration = -iY;
        else
            vyGlobalGoalSimpleVibration = iY;
        if(wMeasuredSmoothed > 0)
            wGlobalGoalSimpleVibration = -iTheta;
        else
            wGlobalGoalSimpleVibration = iTheta;
        simpleVibrationInvertTime = systemMillis;
    }
    else{
        if(vxGlobalGoalSimpleVibration > 0)
            vxGlobalGoalSimpleVibration = iX;
        else
            vxGlobalGoalSimpleVibration = -iX;
        if(vyGlobalGoalSimpleVibration > 0)
            vyGlobalGoalSimpleVibration = iY;
        else
            vyGlobalGoalSimpleVibration = -iY;
        if(wGlobalGoalSimpleVibration > 0)
            wGlobalGoalSimpleVibration = iTheta;
        else
            wGlobalGoalSimpleVibration = -iTheta;
    }

    simpleVibrationPeriod = period;
    if(duration > 0)
        simpleVibrationEndTime = systemMillis + duration;
    else
        simpleVibrationEndTime = 0;
    simpleVibrationEnabled = true;
}

void vibrateOnMotion(float intensityCoeff, unsigned short period){
    vibrationOnMotionPeriod = period;
    vibrationOnMotionIntensityCoeff = intensityCoeff;
    vibrationOnMotionInvertTime = systemMillis;
    vibrationOnMotionEnabled = true;
}

inline void calculateHapticBackdriveAssist(){
    if(!hapticBdrvAssistEnabled){
        vxGlobalGoalHapticBdrvAssist = 0.0;
        vyGlobalGoalHapticBdrvAssist = 0.0;
        wGlobalGoalHapticBdrvAssist = 0.0;
        return;
    }

    //Measure sqrt(Vx^2 + Vy^2) hysteresis, decide whether to go to other state
    float vxySquared = vxGlobalMeasuredSmoothed*vxGlobalMeasuredSmoothed + vyGlobalMeasuredSmoothed*vyGlobalMeasuredSmoothed;
    if((vxySquared < HAPTIC_BDRV_ASSIST_HYSTERESIS_MIN_XY_SQUARED) == bdrvAssistVxyHysteresisMobile)
        bdrvAssistVxyHysteresisFrames++;
    if(bdrvAssistVxyHysteresisFrames >= HAPTIC_BDRV_ASSIST_HYSTERESIS_FRAMES){
        bdrvAssistVxyHysteresisMobile = !bdrvAssistVxyHysteresisMobile;
        bdrvAssistVxyHysteresisFrames = 0;
    }

    //Measure w hysteresis, decide whether to go to other state
    if((abs(wMeasuredSmoothed) < HAPTIC_BDRV_ASSIST_HYSTERESIS_MIN_W) == bdrvAssistWHysteresisMobile)
        bdrvAssistWHysteresisFrames++;
    if(bdrvAssistWHysteresisFrames >= HAPTIC_BDRV_ASSIST_HYSTERESIS_FRAMES){
        bdrvAssistWHysteresisMobile = !bdrvAssistWHysteresisMobile;
        bdrvAssistWHysteresisFrames = 0;
    }

    //Assist Vxy or not
    float vxy = sqrtf(vxySquared);
    if(bdrvAssistVxyHysteresisMobile){
        vxGlobalGoalHapticBdrvAssist = hapticBdrvAssistXCoeff*vxGlobalMeasuredSmoothed;
        vyGlobalGoalHapticBdrvAssist = hapticBdrvAssistYCoeff*vyGlobalMeasuredSmoothed;
        if(vxy < HAPTIC_BDRV_ASSIST_DECAY_XY_KNEE){
            vxGlobalGoalHapticBdrvAssist *= vxy/HAPTIC_BDRV_ASSIST_DECAY_XY_KNEE;
            vyGlobalGoalHapticBdrvAssist *= vxy/HAPTIC_BDRV_ASSIST_DECAY_XY_KNEE;
        }
    }
    else{
        vxGlobalGoalHapticBdrvAssist = HAPTIC_BDRV_ASSIST_SMOOTHING_XY*vxGlobalGoalHapticBdrvAssist + (1 - HAPTIC_BDRV_ASSIST_SMOOTHING_XY)*0.0f;
        vyGlobalGoalHapticBdrvAssist = HAPTIC_BDRV_ASSIST_SMOOTHING_XY*vyGlobalGoalHapticBdrvAssist + (1 - HAPTIC_BDRV_ASSIST_SMOOTHING_XY)*0.0f;
    }

    //Assist w or not
    float wAbs = abs(wMeasured);
    if(bdrvAssistWHysteresisMobile){
        wGlobalGoalHapticBdrvAssist = hapticBdrvAssistThetaCoeff*wMeasuredSmoothed;
        if(wAbs < HAPTIC_BDRV_ASSIST_DECAY_W_KNEE)
            wGlobalGoalHapticBdrvAssist *= wAbs/HAPTIC_BDRV_ASSIST_DECAY_W_KNEE;
    }
    else
        wGlobalGoalHapticBdrvAssist = HAPTIC_BDRV_ASSIST_SMOOTHING_W*wGlobalGoalHapticBdrvAssist + (1 - HAPTIC_BDRV_ASSIST_SMOOTHING_W)*0.0f;
}

inline void calculateCasualBackdriveAssist(){
    if(!casualBdrvAssistEnabled){
        vxGlobalGoalCasualBdrvAssist = 0.0;
        vyGlobalGoalCasualBdrvAssist = 0.0;
        wGlobalGoalCasualBdrvAssist = 0.0;
        return;
    }

    //Measure sqrt(Vx^2 + Vy^2) hysteresis, decide whether to go to other state
    float vxySquared = vxGlobalMeasuredSmoothed*vxGlobalMeasuredSmoothed + vyGlobalMeasuredSmoothed*vyGlobalMeasuredSmoothed;
    if((vxySquared < CASUAL_BDRV_ASSIST_HYSTERESIS_MIN_XY_SQUARED) == bdrvAssistVxyHysteresisMobile)
        bdrvAssistVxyHysteresisFrames++;
    if(bdrvAssistVxyHysteresisFrames >= CASUAL_BDRV_ASSIST_HYSTERESIS_FRAMES){
        bdrvAssistVxyHysteresisMobile = !bdrvAssistVxyHysteresisMobile;
        bdrvAssistVxyHysteresisFrames = 0;
    }

    //Measure w hysteresis, decide whether to go to other state
    if((abs(wMeasuredSmoothed) < CASUAL_BDRV_ASSIST_HYSTERESIS_MIN_W) == bdrvAssistWHysteresisMobile)
        bdrvAssistWHysteresisFrames++;
    if(bdrvAssistWHysteresisFrames >= CASUAL_BDRV_ASSIST_HYSTERESIS_FRAMES){
        bdrvAssistWHysteresisMobile = !bdrvAssistWHysteresisMobile;
        bdrvAssistWHysteresisFrames = 0;
    }

    //Robot is not held, limit assist and decay
    if(currentGesture != GestureHold){

        //Assist Vxy or not
        if(bdrvAssistVxyHysteresisMobile){
            vxGlobalGoalCasualBdrvAssist = CASUAL_BDRV_ASSIST_DECAY_XY*vxGlobalMeasuredSmoothed;
            vyGlobalGoalCasualBdrvAssist = CASUAL_BDRV_ASSIST_DECAY_XY*vyGlobalMeasuredSmoothed;
            if(vxySquared < CASUAL_BDRV_ASSIST_DECAY_XY_KNEE_SQUARED){
                vxGlobalGoalCasualBdrvAssist *= vxySquared/CASUAL_BDRV_ASSIST_DECAY_XY_KNEE_SQUARED;
                vyGlobalGoalCasualBdrvAssist *= vxySquared/CASUAL_BDRV_ASSIST_DECAY_XY_KNEE_SQUARED;
            }
        }
        else{
            vxGlobalGoalCasualBdrvAssist = (1 - CASUAL_BDRV_ASSIST_SMOOTHING_XY)*0.0f + CASUAL_BDRV_ASSIST_SMOOTHING_XY*vxGlobalGoalCasualBdrvAssist;
            vyGlobalGoalCasualBdrvAssist = (1 - CASUAL_BDRV_ASSIST_SMOOTHING_XY)*0.0f + CASUAL_BDRV_ASSIST_SMOOTHING_XY*vyGlobalGoalCasualBdrvAssist;
        }

        //Assist w or not
        float wSquared = wMeasuredSmoothed*wMeasuredSmoothed;
        if(bdrvAssistWHysteresisMobile){
            wGlobalGoalCasualBdrvAssist = CASUAL_BDRV_ASSIST_DECAY_W*wMeasuredSmoothed;
            if(wSquared < CASUAL_BDRV_ASSIST_DECAY_W_KNEE_SQUARED)
                wGlobalGoalCasualBdrvAssist *= wSquared/CASUAL_BDRV_ASSIST_DECAY_W_KNEE_SQUARED;
            if(wSquared < CASUAL_BDRV_ASSIST_DECAY_W_KNEE_2_SQUARED)
                wGlobalGoalCasualBdrvAssist *= wSquared/CASUAL_BDRV_ASSIST_DECAY_W_KNEE_2_SQUARED;
        }
        else
            wGlobalGoalCasualBdrvAssist = (1 - CASUAL_BDRV_ASSIST_SMOOTHING_W)*0.0f + CASUAL_BDRV_ASSIST_SMOOTHING_W*wGlobalGoalCasualBdrvAssist;
    }

    //Robot is held, unlimited assist
    else{

        //Assist Vxy or not
        if(bdrvAssistVxyHysteresisMobile){
            vxGlobalGoalCasualBdrvAssist = CASUAL_BDRV_ASSIST_SMOOTHING_XY*vxGlobalGoalCasualBdrvAssist + (1 - CASUAL_BDRV_ASSIST_SMOOTHING_XY)*vxGlobalMeasured;
            vyGlobalGoalCasualBdrvAssist = CASUAL_BDRV_ASSIST_SMOOTHING_XY*vyGlobalGoalCasualBdrvAssist + (1 - CASUAL_BDRV_ASSIST_SMOOTHING_XY)*vyGlobalMeasured;
        }
        else{
            vxGlobalGoalCasualBdrvAssist = CASUAL_BDRV_ASSIST_SMOOTHING_XY*vxGlobalGoalCasualBdrvAssist + (1 - CASUAL_BDRV_ASSIST_SMOOTHING_XY)*0.0f;
            vyGlobalGoalCasualBdrvAssist = CASUAL_BDRV_ASSIST_SMOOTHING_XY*vyGlobalGoalCasualBdrvAssist + (1 - CASUAL_BDRV_ASSIST_SMOOTHING_XY)*0.0f;
        }

        //Assist w or not
        if(bdrvAssistWHysteresisMobile)
            wGlobalGoalCasualBdrvAssist = CASUAL_BDRV_ASSIST_SMOOTHING_W*wGlobalGoalCasualBdrvAssist + (1 - CASUAL_BDRV_ASSIST_SMOOTHING_W)*wMeasured;
        else
            wGlobalGoalCasualBdrvAssist = CASUAL_BDRV_ASSIST_SMOOTHING_W*wGlobalGoalCasualBdrvAssist + (1 - CASUAL_BDRV_ASSIST_SMOOTHING_W)*0.0f;
    }
}

inline void calculateSimpleVibration(){
    if(!simpleVibrationEnabled){
        vxGlobalGoalSimpleVibration = 0.0;
        vyGlobalGoalSimpleVibration = 0.0;
        wGlobalGoalSimpleVibration = 0.0;
        return;
    }

    if(simpleVibrationEndTime > 0 && systemMillis >= simpleVibrationEndTime)
        simpleVibrationEnabled = false;
    else{
        if(systemMillis >= simpleVibrationInvertTime + simpleVibrationPeriod){
            vxGlobalGoalSimpleVibration = -vxGlobalGoalSimpleVibration;
            vyGlobalGoalSimpleVibration = -vyGlobalGoalSimpleVibration;
            wGlobalGoalSimpleVibration = -wGlobalGoalSimpleVibration;
            simpleVibrationInvertTime += simpleVibrationPeriod;
        }
    }
}

inline void calculateVibrationOnMotion(){
    if(!vibrationOnMotionEnabled){
        vxGlobalGoalVibrationOnMotion = 0.0;
        vyGlobalGoalVibrationOnMotion = 0.0;
        wGlobalGoalVibrationOnMotion = 0.0;
        return;
    }

    if(systemMillis >= vibrationOnMotionInvertTime + vibrationOnMotionPeriod){
        vibrationOnMotionPositive = !vibrationOnMotionPositive;
        vibrationOnMotionInvertTime += vibrationOnMotionPeriod;
    }

    if(abs(vxGlobalMeasuredSmoothed) > VIBRATION_ON_MOTION_MIN_VXY)
        vxGlobalGoalVibrationOnMotion = vibrationOnMotionIntensityCoeff*abs(vxGlobalMeasuredSmoothed);
    else
        vxGlobalGoalVibrationOnMotion = 0.0f;

    if(abs(vyGlobalMeasuredSmoothed) > VIBRATION_ON_MOTION_MIN_VXY)
        vyGlobalGoalVibrationOnMotion = vibrationOnMotionIntensityCoeff*abs(vyGlobalMeasuredSmoothed);
    else
        vyGlobalGoalVibrationOnMotion = 0.0f;

    if(abs(wMeasuredSmoothed) > VIBRATION_ON_MOTION_MIN_W)
        wGlobalGoalVibrationOnMotion = vibrationOnMotionIntensityCoeff*abs(wMeasuredSmoothed);
    else
        wGlobalGoalVibrationOnMotion = 0.0f;

    if(!vibrationOnMotionPositive){
        vxGlobalGoalVibrationOnMotion = -vxGlobalGoalVibrationOnMotion;
        vyGlobalGoalVibrationOnMotion = -vyGlobalGoalVibrationOnMotion;
        wGlobalGoalVibrationOnMotion = -wGlobalGoalVibrationOnMotion;
    }
}

void calculateHaptics(){
    calculateCasualBackdriveAssist();
    calculateHapticBackdriveAssist();
    calculateSimpleVibration();
    calculateVibrationOnMotion();

    vxGlobalGoalHaptics = vxGlobalGoalCasualBdrvAssist + vxGlobalGoalHapticBdrvAssist + vxGlobalGoalSimpleVibration + vxGlobalGoalVibrationOnMotion;
    vyGlobalGoalHaptics = vyGlobalGoalCasualBdrvAssist + vyGlobalGoalHapticBdrvAssist + vyGlobalGoalSimpleVibration + vyGlobalGoalVibrationOnMotion;
    wGlobalGoalHaptics = wGlobalGoalCasualBdrvAssist + wGlobalGoalHapticBdrvAssist + wGlobalGoalSimpleVibration + wGlobalGoalVibrationOnMotion;
}

void clearHapticFeedback(){
    simpleVibrationEnabled = false;
    vibrationOnMotionEnabled = false;
}
