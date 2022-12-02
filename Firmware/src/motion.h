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
 * @file motion.h
 * @brief Header for motion control
 * @author Ayberk Özgür
 * @date 2015-12-14
 */

#ifndef MOTION_H
#define MOTION_H

#include "localization.h"
#include "motor.h"
#include "touch.h"
#include "math.h"
#include "pid.h"

#ifdef  __cplusplus
extern "C" {
#endif

typedef LOCOMOTION_INTERACTIVITY_MODE_ENUM_SHARED LocomotionInteractivityMode;

#define W_MAX 25.0f

#define MAX_DELTA_T_FOR_VEL_ESTIMATE 1000          ///< Maximum time difference between two localization frames to do velocity estimation

#define ROBOT_RADIUS 28.0f                         ///< Distance to one wheel from robot's center in millimeters

#define VEL_ESTIMATE_SMOOTHING_XY 0.5f             ///< Smoothing amount for temporally smoothed x,y velocity measurement
#define VEL_ESTIMATE_SMOOTHING_W 0.8f              ///< Smoothing amount for temporally smoothed w velocity measurement

#define VXY_MAX_INCREMENT 20.0f                    ///< Maximum increment for Vx,Vy in pose/position tracking
#define W_MAX_INCREMENT 0.5f                       ///< Maximum increment for w in pose tracking

#define VX_TRACKING_COEFF 30.0f                    ///< Open loop coeff that links Vx goal to motor outputs
#define VY_TRACKING_COEFF 30.0f                    ///< Open loop coeff that links Vy goal to motor outputs
#define WR_TRACKING_COEFF 40.0f                    ///< Open loop coeff that links w goal to motor outputs

extern float vxGlobalMeasured;                     ///< Measured x velocity in world frame in mm/s
extern float vyGlobalMeasured;                     ///< Measured y velocity in world frame in mm/s
extern float vxLocalMeasured;                      ///< Measured x velocity in robot frame in mm/s
extern float vyLocalMeasured;                      ///< Measured y velocity in robot frame in mm/s
extern float wrMeasured;                           ///< Measured angular velocity times robot's radius in mm*rad/s
extern float wMeasured;                            ///< Measured angular velocity in rad/s
extern float v1Measured;                           ///< Measured angular velocity of motor 1 in rad/s
extern float v2Measured;                           ///< Measured angular velocity of motor 2 in rad/s
extern float v3Measured;                           ///< Measured angular velocity of motor 3 in rad/s
extern float vxGlobalMeasuredSmoothed;             ///< Smoothed version of vxGlobalMeasured
extern float vyGlobalMeasuredSmoothed;             ///< Smoothed version of vyGlobalMeasured
extern float wMeasuredSmoothed;                    ///< Smoothed version of wMeasured

/**
 * @brief List of motion controllers
 */
typedef enum {
    MOTION_TRACKER_NONE = 0,           ///< No motion controller is active
    MOTION_TRACKER_VELOCITY,           ///< Tracking global velocity
    MOTION_TRACKER_POSE,               ///< Tracking pose
    MOTION_TRACKER_POSITION,           ///< Tracking position
    MOTION_TRACKER_X_COORDINATE,       ///< Tracking just the X coordinate
    MOTION_TRACKER_Y_COORDINATE,       ///< Tracking just the Y coordinate
    MOTION_TRACKER_ORIENTATION,        ///< Tracking orientation
    MOTION_TRACKER_X_THETA_COORDINATE, ///< Tracking X and Theta coordinates
    MOTION_TRACKER_Y_THETA_COORDINATE, ///< Tracking Y and Theta coordinates
    MOTION_TRACKER_POLYBEZIER,         ///< Tracking a composite Bézier curve
    MOTION_TRACKER_POLYBEZIER_ALIGNED, ///< Tracking a composite Bézier curve with tangent orientation
    MOTION_TRACKER_NUM_TRACKERS,
    MOTION_TRACKER_INVALID = -1
} MOTION_TRACKER;

/**
 * @brief Sets the locomotion interactivity mode, i.e whether/how the locomotion system is bound to user input
 *
 * @param newMode The new mode
 */
void setLocomotionInteractivityMode(LocomotionInteractivityMode newMode);

/**
 * @brief Sets a goal pose to track
 *
 * @param x X position in grid coordinates times 100
 * @param y Y position in grid coordinates times 100
 * @param theta Orientation in degrees times 100
 * @param v Maximum linear velocity to track pose
 * @param w Maximum angular velocity to track pose
 */
void setGoalPose(int x, int y, int theta, float v, float w);

/**
 * @brief Sets a goal position to track
 *
 * @param x X position in grid coordinates times 100
 * @param y Y position in grid coordinates times 100
 * @param v Maximum linear velocity to track position
 */
void setGoalPosition(int x, int y, float v);

/**
 * @brief Sets a goal X coordinate to track
 *
 * @param x X position in grid coordinates times 100
 * @param v Maximum linear velocity to track position
 */
void setGoalXCoordinate(int x, float v);

/**
 * @brief Sets a goal Y coordinate to track
 *
 * @param y Y position in grid coordinates times 100
 * @param v Maximum linear velocity to track position
 */
void setGoalYCoordinate(int y, float v);

/**
 * @brief Sets a goal velocity
 *
 * @param vx X velocity in world frame in mm/s
 * @param vy Y velocity in world frame in mm/s
 * @param w Angular velocity in world frame in rad/s
 */
void setGoalVelocity(float vx, float vy, float w);

/**
 * @brief Sets a goal orientation to track
 *
 * @param theta Orientation in degrees times 100
 * @param w Maximum angular velocity to track pose
 */
void setGoalOrientation(int theta, float w);

/**
 * @brief Sets a goal X and Theta coordinate to track
 *
 * @param x X position in grid coordinates times 100
 * @param theta Orientation in degrees times 100
 * @param v Maximum linear velocity to track pose
 * @param w Maximum angular velocity to track pose
 */
void setGoalXThetaCoordinate(int x, int theta, float v, float w);

/**
 * @brief Sets a goal Y and Theta coordinate to track
 *
 * @param y Y position in grid coordinates times 100
 * @param theta Orientation in degrees times 100
 * @param v Maximum linear velocity to track pose
 * @param w Maximum angular velocity to track pose
 */
void setGoalYThetaCoordinate(int y, int theta, float v, float w);

/**
 * @brief Starts tracking the previously set composite Bézier curve
 *
 * @param v Linear velocity in world frame in mm/s
 * @param w Angular velocity throughout the traversal
 */
void setGoalPolyBezier(float v, float w);

/**
 * @brief Starts tracking the previously set composite Bézier curve while keeping the orientation aligned to the curve
 *
 * @param v Linear velocity in world frame in mm/s
 * @param theta Orientation to track with respect to the curve tangent orientation, in degrees times 100
 * @param w Maximum angular velocity while tracking orientation, in rad/s
 */
void setGoalPolyBezierAligned(float v, int theta, float w);

/**
 * @brief Clears pose/position/velocity goals
 */
void clearTracking();

/**
 * @brief Initializes motion control
 */
void APP_Motion_Initialize();

/**
 * @brief Runs regular motion control tasks
 */
void APP_Motion_Tasks();


#ifdef  __cplusplus
}
#endif

#endif //MOTION_H
