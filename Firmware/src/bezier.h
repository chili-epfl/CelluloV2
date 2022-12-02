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
 * @file bezier.h
 * @brief Header for composite cubic Bézier curve functionality
 * @author Ayberk Özgür
 * @date 2016-07-24
 */

#ifndef BEZIER_H
#define BEZIER_H

#include <stdbool.h>

#define BEZIER_MAX_SEGMENTS 100             ///< Maximum number of cubic Bezier curve segments that can be kept in memory

/**
 * @brief List of possible composite Bézier curve tracker states
 */
typedef enum {
    POLYBEZIER_TRACKER_GOING_TO_START = 0,  ///< Going to the first control point
    POLYBEZIER_TRACKER_TRACKING,            ///< Regular tracking
    POLYBEZIER_TRACKER_GOING_TO_FINISH,     ///< Tracking of the curve is done, going to the last control point
    POLYBEZIER_TRACKER_FINISHED,            ///< On last control point, motors are off
    POLYBEZIER_TRACKER_NUM_STATES,
    POLYBEZIER_TRACKER_INVALID = -1
} POLYBEZIER_TRACKER;

extern POLYBEZIER_TRACKER polyBezierTrackerState;   ///< Current tracker state
extern float currentT;                              ///< Current curve parameter

/**
 * @brief Sets the first control point of the first Bézier curve segment
 *
 * @param initPoint The first point
 */
void polyBezierInit(float x0, float y0);

/**
 * @brief Adds a new cubic Bézier curve segment; polyBezierInit() must be called before any polyBezierAppend() calls
 *
 * Also calculates the curve length and approximate parameter length for interval searches
 *
 * @param x1 X coordinate of the second control point
 * @param y1 Y coordinate of the second control point
 * @param x2 X coordinate of the third control point
 * @param y2 Y coordinate of the third control point
 * @param x3 X coordinate of the fourth control point
 * @param y3 Y coordinate of the fourth control point
 */
void polyBezierAppend(float x1, float y1, float x2, float y2, float x3, float y3);

/**
 * @brief Returns the first control point in int grid coordinates
 *
 * @param x [out] X coordinate of the first control point
 * @param y [out] Y coordinate of the first control point
 */
void getPolyBezierInit(int* x, int* y);

/**
 * @brief Returns the last control point in int grid coordinates
 *
 * @param x [out] X coordinate of the last control point
 * @param y [out] Y coordinate of the last control point
 */
void getPolyBezierEnd(int* x, int* y);

/**
 * @brief Sets the current parameter to zero
 */
void polyBezierTrackingReset();

/**
 * @brief Calculates the global velocities towards the next point on the curve
 *
 * @param desiredV Magnitude of the output velocity
 * @param vxGlobalGoal [out] X velocity towards the next point on the curve
 * @param vyGlobalGoal [out] Y velocity towards the next point on the curve
 * @return Whether the end of the curve was reached
 */
bool getPolyBezierNextVelocities(float desiredV, float* vxGlobalGoal, float* vyGlobalGoal);

#endif //BEZIER_H
