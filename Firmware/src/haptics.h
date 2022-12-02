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
 * @file haptics.h
 * @brief Header for haptics control
 * @author Ayberk Özgür
 * @date 2016-07-13
 */

#ifndef HAPTICS_H
#define HAPTICS_H

#include <stdbool.h>

#ifdef  __cplusplus
extern "C" {
#endif

#define CASUAL_BDRV_ASSIST_SMOOTHING_XY 0.5f       ///< Smoothing amount for x,y assist when the robot is held
#define CASUAL_BDRV_ASSIST_SMOOTHING_W 0.7f        ///< Smoothing amount for w assist when the robot is held
#define CASUAL_BDRV_ASSIST_DECAY_XY 0.6f           ///< Assist decay for x,y when the robot is not held
#define CASUAL_BDRV_ASSIST_DECAY_XY_KNEE 70.0f     ///< Below this much sqrt(x^2 + y^2), decay increases quadratically
#define CASUAL_BDRV_ASSIST_DECAY_W 0.6f            ///< Assist decay for w when the robot is not held
#define CASUAL_BDRV_ASSIST_DECAY_W_KNEE_2 0.75f    ///< Below this much w, decay increases quartically
#define CASUAL_BDRV_ASSIST_DECAY_W_KNEE 1.5f       ///< Below this much w, decay increases quadratically
#define CASUAL_BDRV_ASSIST_HYSTERESIS_FRAMES 5     ///< Number of frames to move/stay still to enable/disable assist
#define CASUAL_BDRV_ASSIST_HYSTERESIS_MIN_XY 10.0f ///< Min. amount in mm to move/stay still for sqrt(x^2 + y^2) to enable/disable assist
#define CASUAL_BDRV_ASSIST_HYSTERESIS_MIN_W 0.1f   ///< Min. amount in rad to move/stay still for abs(w) to enable/disable assist

#define HAPTIC_BDRV_ASSIST_SMOOTHING_XY 0.5f       ///< Smoothing amount for x,y assist
#define HAPTIC_BDRV_ASSIST_SMOOTHING_W 0.7f        ///< Smoothing amount for w assist
#define HAPTIC_BDRV_ASSIST_DECAY_XY_KNEE 55.0f     ///< Below this much sqrt(x^2 + y^2), decay increases linearly
#define HAPTIC_BDRV_ASSIST_DECAY_W_KNEE 1.0f       ///< Below this much abs(w), decay increases linearly
#define HAPTIC_BDRV_ASSIST_HYSTERESIS_FRAMES 5     ///< Number of frames to move/stay still to enable/disable assist
#define HAPTIC_BDRV_ASSIST_HYSTERESIS_MIN_XY 20.0f ///< Min. amount in mm to move/stay still for sqrt(x^2 + y^2) to enable/disable assist
#define HAPTIC_BDRV_ASSIST_HYSTERESIS_MIN_W 0.2f   ///< Min. amount in rad to move/stay still for abs(w) to enable/disable assist

#define VIBRATION_ON_MOTION_MIN_VXY 15.0f          ///< Min. linear velocity in mm/s for vibration on motion to be enabled
#define VIBRATION_ON_MOTION_MIN_W 0.4f             ///< Min. angular velocity in rad/s for vibration on motion to be enabled

extern float vxGlobalGoalHaptics;                  ///< Global Vx goal dictated by the haptics module
extern float vyGlobalGoalHaptics;                  ///< Global Vy goal dictated by the haptics module
extern float wGlobalGoalHaptics;                   ///< Global w goal dictated by the haptics module

/**
 * @brief Enables/disables casual backdrive assist
 *
 * @param enabled Whether enabled
 */
void casualBackdriveAssistEnable(bool enabled);

/**
 * @brief Enables backdrive assist for haptic purposes
 *
 * Disables casual backdrive assist upon enable.
 * Pass 0 to all arguments to disable.
 *
 * @param xCoeff X assist with respect to x drive velocity, can be negative
 * @param yCoeff Y assist with respect to y drive velocity, can be negative
 * @param thetaCoeff Theta assist with respect to w drive, can be negative
 */
void hapticBackdriveAssist(float xCoeff, float yCoeff, float thetaCoeff);

/**
 * @brief Starts a motor vibration
 *
 * @param iX X intensity, scale is comparable to linear velocity
 * @param iY Y intensity, scale is comparable to linear velocity
 * @param iTheta Theta intensity, scale is comparable to angular velocity
 * @param period Period of vibration in milliseconds
 * @param duration Duration of vibration in milliseconds, 0 for vibrate forever
 */
void simpleVibrate(float iX, float iY, float iTheta, unsigned short period, unsigned short duration);

/**
 * @brief Enables vibrating against motion
 *
 * @param intensityCoeff Vibration intensity rate with respect to current velocities
 * @param period Vibration period
 */
void vibrateOnMotion(float intensityCoeff, unsigned short period);

/**
 * @brief Disables all haptic feedbacks
 */
void clearHapticFeedback();

/**
 * @brief Regular calculation task to determine haptics velocity outputs
 */
void calculateHaptics();

#ifdef  __cplusplus
}
#endif

#endif //HAPTICS_H
