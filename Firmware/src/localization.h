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
 * @file localization.h
 * @brief Header for pattern localization
 * @author Lukas Hostettler
 * @date 2015-05-19
 */

#ifndef LOCALIZATION_H
#define LOCALIZATION_H

#include <stdio.h>
#include "cam.h"
#include "bluetooth.h"
#include "libdots/dots_localization.h"

#define POSE_PUBLISH_PERIOD 500
#define TIME_BEFORE_IDLING 500

#define EMPTY_FRAMES_FOR_KIDNAP 10      ///< After this many frames of failed decoding, we declare kidnapping

#define X_Y_DISTANCE_TO_WAKE_UP 100     ///< The robot won't exit idle state without being moved by this much in either x or y
#define THETA_DISTANCE_TO_WAKE_UP 200   ///< The robot won't exit idle state without being rotated by this much (100*degrees)

#ifdef  __cplusplus
extern "C" {
#endif

extern DecodingError poseDecodingError; ///< Decoding error if any
extern int poseX;                       ///< X component of pose, in grid coordinates, multiplied by 100
extern int poseY;                       ///< Y component of pose, in grid coordinates, multiplied by 100
extern int poseTheta;                   ///< Orientation component of pose, in degrees, multiplied by 100
extern float poseThetaRad;              ///< Orientation component of pose, in radians
extern bool kidnapped;                  ///< Whether the robot is kidnapped
extern unsigned int poseTimestamp;      ///< When this pose was recorded in milliseconds

extern bool timestamping;               ///< Whether to send the timestamp as well without idling

extern unsigned int bcastPeriod;        ///< Desired broadcast period in milliseconds

/**
 * @brief Initializes the pattern decoding
 */
void APP_Localization_Initialize();

/**
 * @brief Performs regular Pattern Decoding tasks
 */
void APP_Localization_Tasks();

/**
 * @brief Detects activity and returns whether idle mode should be preserved
 *
 * @return True if the robot is not moved enough since its last pose
 */
bool stayIdleCV();

#ifdef  __cplusplus
}
#endif

#endif //LOCALIZATION_H
