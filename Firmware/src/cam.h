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
 * @file cam.h
 * @brief Header for camera functionality
 * @author Ayberk Özgür
 * @date 2015-04-30
 */

#ifndef CAM_H
#define CAM_H

#include "mt9v034.h"
#include "config.h"

#define PIXEL_WR_PORT_CHANNEL PORT_CHANNEL_D
#define PIXEL_WR_PORT_BIT_POS PORTS_BIT_POS_4

#define DCHCON_CHEN_MASK 0b00000000000000000000000010000000

#if defined(EXPOSURE_NEW_HOUSING)
    #define CAM_ANALOG_GAIN 16
#elif defined(EXPOSURE_OLD_HOUSING)
    #define CAM_ANALOG_GAIN 64
#endif

#define CAM_AUTOEXPOSURE_GOAL_MEAN 0x87               ///< Target image mean
#define CAM_AUTOEXPOSURE_NUM_PIXELS_IN_ROW (94 + 752) ///< Number of pixels in a row used in shutter width = Horizontal blanking default + 751
#define CAM_AUTOEXPOSURE_MIN_PIXELS 300               ///< Minimum number of pixels to use in shutter width
#define CAM_AUTOEXPOSURE_INIT_PIXELS 700              ///< Initial number of pixels to use in shutter width
#define CAM_AUTOEXPOSURE_MAX_PIXELS 2200              ///< Maximum number of pixels to use in shutter width
#define CAM_AUTOEXPOSURE_P 1                          ///< Coefficient to link goal mean error to exposure timing increment/decrement

/**
 * @brief Camera frame states
 */
typedef enum {
    CAM_STATE_GRABBING = 0, //New frame is being grabbed
    CAM_STATE_READY,        //New frame is ready
    CAM_STATE_NUM_STATES,
    CAM_STATE_INVALID = -1
} CAM_STATE;

extern unsigned char pixels[];
extern bool frameRequest;
extern bool frameConsumed;
extern bool camSleepReady;
extern CAM_STATE currentCamState;
extern unsigned char camImgMean;

/**
 * @brief Initializes camera functionality
 */
void APP_Cam_Initialize();

/**
 * @brief Performs regular camera tasks
 */
void APP_Cam_Tasks();

/**
 * @brief Sets the exposure time or enables autoexposure
 *
 * @param time Exposure time in number of pixels, 0 enables autoexposure
 */
void setExposureTime(unsigned int time);

/**
 * @brief Fixes the row skew problem due to manual latching of image sensor signals
 */
void camFixSkewedRows();

/**
 * @brief Resumes camera operation (after waking up from sleep or idle mode)
 */
void resumeCam();

/**
 * @brief Suspends camera operation for sleep or idle mode
 */
void suspendCam();

/**
 * @brief Sends the new exposure time to the sensor
 */
void updateShutterWidth();

/**
 * @brief Wakes the camera up, grabs one frame and suspends it back
 *
 * @return Whether the frame was successfully grabbed
 */
bool grabOneFrame();

#endif //CAM_H
