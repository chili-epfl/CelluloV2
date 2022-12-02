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
 * @file bezier.c
 * @brief Header for composite cubic Bézier curve functionality
 * @author Ayberk Özgür
 * @date 2016-07-24
 */

#include "bezier.h"

#include <math.h>
#include <float.h>

#include "qml-plugin/include/CelluloBluetoothSharedDefs.h"
#include "localization.h"

#pragma GCC push_options
#pragma GCC optimize ("O1")

#define NEXT_DIST_MULTIPLIER 0.15f   ///< Curve tracking speed multiplied by this
#define INIT_T_INCREMENT 0.01f      ///< Initial t increment for next velocity estimate
#define MIN_T_INCREMENT 0.00001f    ///< Minimum t increment for next velocity estimate

POLYBEZIER_TRACKER polyBezierTrackerState = POLYBEZIER_TRACKER_FINISHED;

float currentT = 0.0f;
float currentX = 0.0f;
float currentY = 0.0f;
float currentDist = 0.0f;
int numSegments = 0;
float bezierControlPointsX[BEZIER_MAX_SEGMENTS*3 + 1];
float bezierControlPointsY[BEZIER_MAX_SEGMENTS*3 + 1];

inline float square(register float val){
    return val*val;
}

inline float distSquared(float p1x, float p1y, float p2x, float p2y){
    register float xdiff = p1x - p2x;
    register float ydiff = p1y - p2y;
    return xdiff*xdiff + ydiff*ydiff;
}

inline float calcBezier(float externalT, register float* controlPoints){
    if(externalT < 0.0f)
        externalT = 0.0f;
    float numSegmentF;
    register float internalT = modff(externalT, &numSegmentF);
    controlPoints = &controlPoints[3*((int)numSegmentF)];

    if(internalT == 0.0f)
        return controlPoints[0];
    else if(internalT == 1.0f)
        return controlPoints[3];
    else{
        register float one_minus_t = 1 - internalT;
        register float one_minus_t_squared = one_minus_t*one_minus_t;
        register float t_squared = internalT*internalT;
        return one_minus_t_squared*(one_minus_t*controlPoints[0] + 3*controlPoints[1]*internalT) + t_squared*(3*controlPoints[2]*one_minus_t + internalT*controlPoints[3]);
    }
}

void polyBezierInit(float x0, float y0){
    bezierControlPointsX[0] = x0;
    bezierControlPointsY[0] = y0;
    numSegments = 0;
}

void polyBezierAppend(float x1, float y1, float x2, float y2, float x3, float y3){
    if(numSegments >= BEZIER_MAX_SEGMENTS)
        return;

    bezierControlPointsX[numSegments*3 + 1] = x1;
    bezierControlPointsY[numSegments*3 + 1] = y1;

    bezierControlPointsX[numSegments*3 + 2] = x2;
    bezierControlPointsY[numSegments*3 + 2] = y2;

    bezierControlPointsX[numSegments*3 + 3] = x3;
    bezierControlPointsY[numSegments*3 + 3] = y3;

    numSegments++;
}

void polyBezierTrackingReset(){
    currentT = 0.0f;
}

void getPolyBezierInit(int* x, int* y){
    if(bezierControlPointsX[0] <= 0)
        *x = 0;
    else
        *x = (int)(bezierControlPointsX[0]*GOAL_POSE_FACTOR_SHARED/DOTS_GRID_SPACING);
    if(bezierControlPointsY[0] <= 0)
        *y = 0;
    else
        *y = (int)(bezierControlPointsY[0]*GOAL_POSE_FACTOR_SHARED/DOTS_GRID_SPACING);
}

void getPolyBezierEnd(int* x, int* y){
    if(bezierControlPointsX[numSegments*3] <= 0)
        *x = 0;
    else
        *x = (int)(bezierControlPointsX[numSegments*3]*GOAL_POSE_FACTOR_SHARED/DOTS_GRID_SPACING);
    if(bezierControlPointsY[numSegments*3] <= 0)
        *y = 0;
    else
        *y = (int)(bezierControlPointsY[numSegments*3]*GOAL_POSE_FACTOR_SHARED/DOTS_GRID_SPACING);
}

bool getPolyBezierNextVelocities(float desiredV, float* vxGlobalGoal, float* vyGlobalGoal){
    float robotX = poseX/GOAL_POSE_FACTOR_SHARED*DOTS_GRID_SPACING;
    float robotY = poseY/GOAL_POSE_FACTOR_SHARED*DOTS_GRID_SPACING;
    float nextX = calcBezier(currentT, bezierControlPointsX);
    float nextY = calcBezier(currentT, bezierControlPointsY);

    //If we are falling behind, do not attempt to move t forward
    float distLim = square(NEXT_DIST_MULTIPLIER*desiredV);
    if(distLim > distSquared(nextX, nextY, robotX, robotY)){
        float tIncrement = min(INIT_T_INCREMENT, (float)numSegments - currentT);

        //Decrease t until we have desired distance to next
        nextX = calcBezier(currentT + tIncrement, bezierControlPointsX);
        nextY = calcBezier(currentT + tIncrement, bezierControlPointsY);
        while(distSquared(nextX, nextY, robotX, robotY) > distLim && tIncrement > MIN_T_INCREMENT){
            tIncrement /= 2;
            nextX = calcBezier(currentT + tIncrement, bezierControlPointsX);
            nextY = calcBezier(currentT + tIncrement, bezierControlPointsY);
        }

        //Increase t until we have desired distance to next
        distLim /= 2;
        while(distLim > distSquared(nextX, nextY, robotX, robotY)){
            tIncrement *= 2;
            if(tIncrement >= (float)numSegments - currentT){
                currentT = (float)numSegments;
                nextX = calcBezier(currentT, bezierControlPointsX);
                nextY = calcBezier(currentT, bezierControlPointsY);
                break;
            }
            nextX = calcBezier(currentT + tIncrement, bezierControlPointsX);
            nextY = calcBezier(currentT + tIncrement, bezierControlPointsY);
        }

        currentT += tIncrement;
    }

    if(currentT >= (float)numSegments)
        return true;
    else{
        float nextXVec = nextX - robotX;
        float nextYVec = nextY - robotY;
        float mag = sqrtf(nextXVec*nextXVec + nextYVec*nextYVec);
        *vxGlobalGoal = nextXVec/mag*desiredV;
        *vyGlobalGoal = nextYVec/mag*desiredV;
        return false;
    }
}

#pragma GCC pop_options
