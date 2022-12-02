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
 * @file motion.c
 * @brief Source for motion control
 * @author Ayberk Özgür
 * @date 2015-12-14
 */

#include "motion.h"

#include <math.h>

#include "haptics.h"
#include "bluetooth.h"
#include "bezier.h"


#define abs(X) ((X) >= 0 ? (X) : -(X))
#define max(X,Y) ((X) > (Y) ? (X) : (Y))
#define PI 3.14159265359f

MOTION_TRACKER currentMotionTracker = MOTION_TRACKER_NONE; ///< Currently active tracker

LocomotionInteractivityMode locomotionInteractivityMode = LocomotionInteractivityModeNormal;

int goalPoseX = 0;
int goalPoseY = 0;
int goalPoseTheta = 0;
float goalPoseMaxV = 0.0;
float goalPoseMaxW = 0.0;

int polyBezierAlignedTheta = 0;

const float pPoseXY = 2.0f;             ///< Proportional component of goal pose controller for x and y
const float pPoseTheta = 0.1f;          ///< Proportional component of goal pose controller for theta
const float pPoseThetaGoalless = 0.08f; ///< Proportional component of goal pose controller for theta, when there is no fixed goal

const float marginXY = 5.0f;            ///< Within this bound of x and y, we consider we reached the goal
const float marginTheta = 2.0f;         ///< Within this bound of theta, we consider we reached the goal

bool xGoalReached = false;              ///< Whether we are within marginXY of the x goal
bool yGoalReached = false;              ///< Whether we are within marginXY of the y goal
bool thetaGoalReached = false;          ///< Whether we are within marginTheta of the theta goal
bool polyBezierGoalReached = false;     ///< Whether the composite Bézier curve is entirely tracked
bool relevantGoalsReached = false;      ///< Whether all coordinate components relevant to the current tracker are within margins

const short motorOutputMax = 0xFFF;     ///< Maximum motor output, absolute value
const short motorOutputMin = 0x420;     ///< Minimum motor output, absolute value

short motor1Target = 0;                 ///< Target output for motor 1
short motor2Target = 0;                 ///< Target output for motor 2
short motor3Target = 0;                 ///< Target output for motor 3

float vxGlobalMeasured = 0.0;
float vyGlobalMeasured = 0.0;
float vxLocalMeasured = 0.0;
float vyLocalMeasured = 0.0;
float wrMeasured = 0.0;
float wMeasured = 0.0;
float v1Measured = 0.0;
float v2Measured = 0.0;
float v3Measured = 0.0;
float vxGlobalMeasuredSmoothed = 0.0;
float vyGlobalMeasuredSmoothed = 0.0;
float wMeasuredSmoothed = 0.0;

float vxGlobalGoalTracker = 0.0; ///< Global Vx goal dictated by the active tracker
float vyGlobalGoalTracker = 0.0; ///< Global Vy goal dictated by the active tracker
float wGlobalGoalTracker = 0.0;  ///< Global w goal dictated by the active tracker

int prevPoseX = 0;
int prevPoseY = 0;
float prevPoseThetaRad = 0;
unsigned int prevPoseTimestamp = 0;

float deltaT_secs = 0;
bool use_adaptive_ctrl = true;

float k_inv[9] = {
    0.666666666666667f,     0.0f,                   0.333333333333333f,
    -0.333333333333333f,    0.577350269189626f,     0.333333333333333f,
    -0.333333333333333f,    -0.577350269189626f,    0.333333333333333f
}; ///< Inverse kinematic transfer matrix: Maps x, y, theta velocities to wheel velocities

void setLocomotionInteractivityMode(LocomotionInteractivityMode newMode){
    locomotionInteractivityMode = newMode;
}

void setGoalPose(int x, int y, int theta, float v, float w){
    goalPoseX = x;
    goalPoseY = y;
    goalPoseTheta = theta;
    goalPoseMaxV = v;
    goalPoseMaxW = w;
    currentMotionTracker = MOTION_TRACKER_POSE;
    relevantGoalsReached = false;
    //***************************** CLEAR D AND I
}

void setGoalPosition(int x, int y, float v){
    goalPoseX = x;
    goalPoseY = y;
    goalPoseMaxV = v;
    goalPoseMaxW = 0;
    currentMotionTracker = MOTION_TRACKER_POSITION;
    relevantGoalsReached = false;
    //***************************** CLEAR D AND I
}

void setGoalXCoordinate(int x, float v){
    goalPoseX = x;
    goalPoseMaxV = v;
    goalPoseMaxW = 0;
    currentMotionTracker = MOTION_TRACKER_X_COORDINATE;
    relevantGoalsReached = false;
    //***************************** CLEAR D AND I
}

void setGoalYCoordinate(int y, float v){
    goalPoseY = y;
    goalPoseMaxV = v;
    goalPoseMaxW = 0;
    currentMotionTracker = MOTION_TRACKER_Y_COORDINATE;
    relevantGoalsReached = false;
    //***************************** CLEAR D AND I
}

void setGoalOrientation(int theta, float w){
    goalPoseTheta = theta;
    goalPoseMaxV = 0;
    goalPoseMaxW = w;
    currentMotionTracker = MOTION_TRACKER_ORIENTATION;
    relevantGoalsReached = false;
    //***************************** CLEAR D AND I
}

void setGoalXThetaCoordinate(int x, int theta, float v, float w){
    goalPoseX = x;
    goalPoseTheta = theta;
    goalPoseMaxV = v;
    goalPoseMaxW = w;
    currentMotionTracker = MOTION_TRACKER_X_THETA_COORDINATE;
    relevantGoalsReached = false;
    //***************************** CLEAR D AND I
}

void setGoalYThetaCoordinate(int y, int theta, float v, float w){
    goalPoseY = y;
    goalPoseTheta = theta;
    goalPoseMaxV = v;
    goalPoseMaxW = w;
    currentMotionTracker = MOTION_TRACKER_Y_THETA_COORDINATE;
    relevantGoalsReached = false;
    //***************************** CLEAR D AND I
}

void setGoalVelocity(float vx, float vy, float w){
    vxGlobalGoalTracker = vx;
    vyGlobalGoalTracker = vy;
    wGlobalGoalTracker = w;
    currentMotionTracker = MOTION_TRACKER_VELOCITY;
    //***************************** CLEAR D AND I
}

void setGoalPolyBezier(float v, float w){
    goalPoseMaxV = v;
    goalPoseMaxW = w;
    currentMotionTracker = MOTION_TRACKER_POLYBEZIER;
    relevantGoalsReached = false;
    //***************************** CLEAR D AND I
    polyBezierTrackerState = POLYBEZIER_TRACKER_GOING_TO_START;
    getPolyBezierInit(&goalPoseX, &goalPoseY);
}

void setGoalPolyBezierAligned(float v, int theta, float w){
    goalPoseMaxV = v;
    goalPoseMaxW = w;
    polyBezierAlignedTheta = theta;
    currentMotionTracker = MOTION_TRACKER_POLYBEZIER_ALIGNED;
    relevantGoalsReached = false;
    //***************************** CLEAR D AND I
    polyBezierTrackerState = POLYBEZIER_TRACKER_GOING_TO_START;
    getPolyBezierInit(&goalPoseX, &goalPoseY);
}

void clearTracking(){
    goalPoseX = 0;
    goalPoseY = 0;
    goalPoseTheta = 0;
    goalPoseMaxV = 0;
    goalPoseMaxW = 0;
    currentMotionTracker = MOTION_TRACKER_NONE;
}

/**
 * @brief Sets all measured velocities to zero
 */
inline void zeroMeasuredVelocities(){
    vxGlobalMeasured = 0.0;
    vyGlobalMeasured = 0.0;
    vxLocalMeasured = 0.0;
    vyLocalMeasured = 0.0;
    wrMeasured = 0.0;
    wMeasured = 0.0;
    v1Measured = 0.0;
    v2Measured = 0.0;
    v3Measured = 0.0;
}

/**
 * @brief Sets all motor outputs to zero
 */
inline void zeroMotorOutputs(){
    setMotorOutput(1, 0);
    setMotorOutput(2, 0);
    setMotorOutput(3, 0);
}

void Send_ACtrl_Params(){
    //float p_out[18];
    //ACtrl_Get_Params(&actrl, p_out);
    bluetoothEventACtrlParameters();
}

void Set_adaptive_ctrl(bool enable){
    use_adaptive_ctrl = enable;
}
/**
 * @brief Sets motor outputs in a calibrated open loop from global goal velocities
 */
/*inline void setMotorOutputsFromGlobalVelGoals(){
    float vxGlobalGoal = vxGlobalGoalTracker + vxGlobalGoalHaptics;
    float vyGlobalGoal = vyGlobalGoalTracker + vyGlobalGoalHaptics;
    float wGlobalGoal = wGlobalGoalTracker + wGlobalGoalHaptics;

    if(vxGlobalGoal == 0 && vyGlobalGoal == 0 && wGlobalGoal == 0)
        zeroMotorOutputs();
    else{

        //Calculate linear velocity goals in local frame
        float vxLocalGoal = cosf(-poseThetaRad)*vxGlobalGoal - sinf(-poseThetaRad)*vyGlobalGoal;
        float vyLocalGoal = sinf(-poseThetaRad)*vxGlobalGoal + cosf(-poseThetaRad)*vyGlobalGoal;

        //Calcualte motor target outputs
        motor1Target = k_inv[0]*VX_TRACKING_COEFF*vxLocalGoal + k_inv[1]*VY_TRACKING_COEFF*vyLocalGoal + k_inv[2]*WR_TRACKING_COEFF*ROBOT_RADIUS*wGlobalGoal;
        motor2Target = k_inv[3]*VX_TRACKING_COEFF*vxLocalGoal + k_inv[4]*VY_TRACKING_COEFF*vyLocalGoal + k_inv[5]*WR_TRACKING_COEFF*ROBOT_RADIUS*wGlobalGoal;
        motor3Target = k_inv[6]*VX_TRACKING_COEFF*vxLocalGoal + k_inv[7]*VY_TRACKING_COEFF*vyLocalGoal + k_inv[8]*WR_TRACKING_COEFF*ROBOT_RADIUS*wGlobalGoal;

        //Ensure output does not go below minimum
        if(motor1Target > 0)
            motor1Target += motorOutputMin;
        else if(motor1Target < 0)
            motor1Target -= motorOutputMin;

        if(motor2Target > 0)
            motor2Target += motorOutputMin;
        else if(motor2Target < 0)
            motor2Target -= motorOutputMin;

        if(motor3Target > 0)
            motor3Target += motorOutputMin;
        else if(motor3Target < 0)
            motor3Target -= motorOutputMin;

        //Ensure output does not go above maximum, conserve scales among motor outputs
        float motorTargetMax = max(max(abs(motor1Target), abs(motor2Target)), abs(motor3Target));
        if(motorTargetMax > motorOutputMax){
            motor1Target = motor1Target*motorOutputMax/motorTargetMax;
            motor2Target = motor2Target*motorOutputMax/motorTargetMax;
            motor3Target = motor3Target*motorOutputMax/motorTargetMax;
        }

        //Set motor outputs
        setMotorOutput(1, motor1Target);
        setMotorOutput(2, motor2Target);
        setMotorOutput(3, motor3Target);
    }
}
*/
inline void setMotorOutputsFromGlobalVelGoals(){
    float vxGlobalGoal = vxGlobalGoalTracker + vxGlobalGoalHaptics;
    float vyGlobalGoal = vyGlobalGoalTracker + vyGlobalGoalHaptics;
    float wGlobalGoal = wGlobalGoalTracker + wGlobalGoalHaptics;
    
    
    if(vxGlobalGoal == 0 && vyGlobalGoal == 0 && wGlobalGoal == 0)
        zeroMotorOutputs();
    else{
        if(use_adaptive_ctrl){
            
            float ref[3] = {-vxGlobalGoal/1000., -vyGlobalGoal/1000., wGlobalGoal};
            float state[6] = {poseX, poseY, poseThetaRad, -vxGlobalMeasuredSmoothed/1000., -vyGlobalMeasuredSmoothed/1000., wMeasuredSmoothed};
            float u[3] = {0., 0., 0.};

            updateACtrl(&actrl, ref, state, deltaT_secs, u);
            
            if(abs(u[0])>=6 || abs(u[1])>=6 || abs(u[2])>=6){
                ACtrl_Set_saturation(true);
            }else{
                ACtrl_Set_saturation(false);
            }
            
            //Calculate motor target outputs
            motor1Target = u[0]/6.*4095;
            motor2Target = u[1]/6.*4095;
            motor3Target = u[2]/6.*4095;
            
        }else{

            //Calculate linear velocity goals in local frame
            float vxLocalGoal = cosf(-poseThetaRad)*vxGlobalGoal - sinf(-poseThetaRad)*vyGlobalGoal;
            float vyLocalGoal = sinf(-poseThetaRad)*vxGlobalGoal + cosf(-poseThetaRad)*vyGlobalGoal;

            //Calculate motor target outputs
            motor1Target = k_inv[0]*VX_TRACKING_COEFF*vxLocalGoal + k_inv[1]*VY_TRACKING_COEFF*vyLocalGoal + k_inv[2]*WR_TRACKING_COEFF*ROBOT_RADIUS*wGlobalGoal;
            motor2Target = k_inv[3]*VX_TRACKING_COEFF*vxLocalGoal + k_inv[4]*VY_TRACKING_COEFF*vyLocalGoal + k_inv[5]*WR_TRACKING_COEFF*ROBOT_RADIUS*wGlobalGoal;
            motor3Target = k_inv[6]*VX_TRACKING_COEFF*vxLocalGoal + k_inv[7]*VY_TRACKING_COEFF*vyLocalGoal + k_inv[8]*WR_TRACKING_COEFF*ROBOT_RADIUS*wGlobalGoal;

        }
                
        //Ensure output does not go below minimum
        if(motor1Target > 0)
            motor1Target += motorOutputMin;
        else if(motor1Target < 0)
            motor1Target -= motorOutputMin;

        if(motor2Target > 0)
            motor2Target += motorOutputMin;
        else if(motor2Target < 0)
            motor2Target -= motorOutputMin;

        if(motor3Target > 0)
            motor3Target += motorOutputMin;
        else if(motor3Target < 0)
            motor3Target -= motorOutputMin;

        //Ensure output does not go above maximum, conserve scales among motor outputs
        float motorTargetMax = max(max(abs(motor1Target), abs(motor2Target)), abs(motor3Target));
        if(motorTargetMax > motorOutputMax){
            motor1Target = motor1Target*motorOutputMax/motorTargetMax;
            motor2Target = motor2Target*motorOutputMax/motorTargetMax;
            motor3Target = motor3Target*motorOutputMax/motorTargetMax;
        }
                
        //Set motor outputs
        setMotorOutput(1, motor1Target);
        setMotorOutput(2, motor2Target);
        setMotorOutput(3, motor3Target);
    }
}

/**
 * @brief Estimates global & local robot velocities and wheel velocities from localization frames
 *
 * @return Whether the velocities were sucessfully estimated
 */
inline bool estimateVelocities(){
    int deltaT = poseTimestamp - prevPoseTimestamp;
    deltaT_secs = ((float)(poseTimestamp - prevPoseTimestamp))/1000.;

    //Same frame
    if(deltaT <= 0)
        return false;

    //Regular timestamp difference
    else if(deltaT < MAX_DELTA_T_FOR_VEL_ESTIMATE){

        //Calculate raw values
        vxGlobalMeasured = ((poseX - prevPoseX)/100.0f*DOTS_GRID_SPACING)*1000/deltaT;
        vyGlobalMeasured = ((poseY - prevPoseY)/100.0f*DOTS_GRID_SPACING)*1000/deltaT;
        wMeasured = poseThetaRad - prevPoseThetaRad;
        if(wMeasured < -PI)
            wMeasured += 2*PI;
        else if(wMeasured > PI)
            wMeasured -= 2*PI;
        wMeasured = wMeasured*1000/deltaT;

        //Calculate local velocities from global velocities
        vxLocalMeasured = cosf(-poseThetaRad)*vxGlobalMeasured - sinf(-poseThetaRad)*vyGlobalMeasured;
        vyLocalMeasured = sinf(-poseThetaRad)*vxGlobalMeasured + cosf(-poseThetaRad)*vyGlobalMeasured;
        wrMeasured = wMeasured*ROBOT_RADIUS;

        //Calculate smoothed values
        vxGlobalMeasuredSmoothed = VEL_ESTIMATE_SMOOTHING_XY*vxGlobalMeasuredSmoothed + (1 - VEL_ESTIMATE_SMOOTHING_XY)*vxGlobalMeasured;
        vyGlobalMeasuredSmoothed = VEL_ESTIMATE_SMOOTHING_XY*vyGlobalMeasuredSmoothed + (1 - VEL_ESTIMATE_SMOOTHING_XY)*vyGlobalMeasured;
        wMeasuredSmoothed = VEL_ESTIMATE_SMOOTHING_W*wMeasuredSmoothed + (1 - VEL_ESTIMATE_SMOOTHING_W)*wMeasured;

        //Calculate wheel velocities
        v1Measured = (k_inv[0]*vxLocalMeasured + k_inv[1]*vyLocalMeasured + k_inv[2]*wrMeasured);
        v2Measured = (k_inv[3]*vxLocalMeasured + k_inv[4]*vyLocalMeasured + k_inv[5]*wrMeasured);
        v3Measured = (k_inv[6]*vxLocalMeasured + k_inv[7]*vyLocalMeasured + k_inv[8]*wrMeasured);

        prevPoseX = poseX;
        prevPoseY = poseY;
        prevPoseThetaRad = poseThetaRad;
        prevPoseTimestamp = poseTimestamp;
        return true;
    }

    //Too much timestamp difference
    else{
        zeroMeasuredVelocities();
        prevPoseX = poseX;
        prevPoseY = poseY;
        prevPoseThetaRad = poseThetaRad;
        prevPoseTimestamp = poseTimestamp;
        return false;
    }
}

/**
 * @brief Calculates necessary velocities to reach given position
 */
inline void trackPosition(){

    //Get difference of goal and actual x, y in grid coordinates
    float xDiff = (goalPoseX - poseX)/100.0f;
    float yDiff = (goalPoseY - poseY)/100.0f;

    xGoalReached = abs(xDiff) < marginXY;
    yGoalReached = abs(yDiff) < marginXY;

    float vxGlobalGoalNew;
    float vyGlobalGoalNew;

    //Maximum linear velocity is zero, do not track linear position
    if(goalPoseMaxV == 0){
        vxGlobalGoalNew = 0;
        vyGlobalGoalNew = 0;
    }

    //Only x goal remains
    else if(!xGoalReached && yGoalReached){
        vyGlobalGoalNew = 0;
        vxGlobalGoalNew = pPoseXY*xDiff;
        if(vxGlobalGoalNew > goalPoseMaxV)
            vxGlobalGoalNew = goalPoseMaxV;
        else if(vxGlobalGoalNew < -goalPoseMaxV)
            vxGlobalGoalNew = -goalPoseMaxV;
    }

    //Only y goal remains
    else if(xGoalReached && !yGoalReached){
        vxGlobalGoalNew = 0;
        vyGlobalGoalNew = pPoseXY*yDiff;
        if(vyGlobalGoalNew > goalPoseMaxV)
            vyGlobalGoalNew = goalPoseMaxV;
        else if(vyGlobalGoalNew < -goalPoseMaxV)
            vyGlobalGoalNew = -goalPoseMaxV;
    }

    //Both goals reached
    else if(xGoalReached && yGoalReached){
        vxGlobalGoalNew = 0;
        vyGlobalGoalNew = 0;
    }

    //Both goals remain
    else{
        vxGlobalGoalNew = pPoseXY*xDiff;
        vyGlobalGoalNew = pPoseXY*yDiff;

        //Scale down both x and y to have given max. linear velocity
        float vxyGlobalGoalNew = sqrtf(vxGlobalGoalNew*vxGlobalGoalNew + vyGlobalGoalNew*vyGlobalGoalNew);
        if(vxyGlobalGoalNew > goalPoseMaxV){
            vxGlobalGoalNew = vxGlobalGoalNew*goalPoseMaxV/vxyGlobalGoalNew;
            vyGlobalGoalNew = vyGlobalGoalNew*goalPoseMaxV/vxyGlobalGoalNew;
        }
    }

    //Update goal velocities slowly over time
    if(vxGlobalGoalNew > vxGlobalGoalTracker + VXY_MAX_INCREMENT)
        vxGlobalGoalTracker = vxGlobalGoalTracker + VXY_MAX_INCREMENT;
    else if(vxGlobalGoalNew < vxGlobalGoalTracker - VXY_MAX_INCREMENT)
        vxGlobalGoalTracker = vxGlobalGoalTracker - VXY_MAX_INCREMENT;
    else
        vxGlobalGoalTracker = vxGlobalGoalNew;

    if(vyGlobalGoalNew > vyGlobalGoalTracker + VXY_MAX_INCREMENT)
        vyGlobalGoalTracker = vyGlobalGoalTracker + VXY_MAX_INCREMENT;
    else if(vyGlobalGoalNew < vyGlobalGoalTracker - VXY_MAX_INCREMENT)
        vyGlobalGoalTracker = vyGlobalGoalTracker - VXY_MAX_INCREMENT;
    else
        vyGlobalGoalTracker = vyGlobalGoalNew;
}

/**
 * @brief Calculates necessary velocities to reach given X coordindate
 */
inline void trackXCoordinate(){

    //Get difference of goal and actual x in grid coordinates
    float xDiff = (goalPoseX - poseX)/100.0f;

    xGoalReached = abs(xDiff) < marginXY;

    float vxGlobalGoalNew;

    //Maximum linear velocity is zero or goal is reached, do not track linear position
    if(goalPoseMaxV == 0 || xGoalReached)
        vxGlobalGoalNew = 0;

    //X goal remains
    else{
        vxGlobalGoalNew = pPoseXY*xDiff;
        if(vxGlobalGoalNew > goalPoseMaxV)
            vxGlobalGoalNew = goalPoseMaxV;
        else if(vxGlobalGoalNew < -goalPoseMaxV)
            vxGlobalGoalNew = -goalPoseMaxV;
    }

    //Update goal velocities slowly over time
    if(vxGlobalGoalNew > vxGlobalGoalTracker + VXY_MAX_INCREMENT)
        vxGlobalGoalTracker = vxGlobalGoalTracker + VXY_MAX_INCREMENT;
    else if(vxGlobalGoalNew < vxGlobalGoalTracker - VXY_MAX_INCREMENT)
        vxGlobalGoalTracker = vxGlobalGoalTracker - VXY_MAX_INCREMENT;
    else
        vxGlobalGoalTracker = vxGlobalGoalNew;
}

/**
 * @brief Calculates necessary velocities to reach given Y coordindate
 */
inline void trackYCoordinate(){

    //Get difference of goal and actual y in grid coordinates
    float yDiff = (goalPoseY - poseY)/100.0f;

    yGoalReached = abs(yDiff) < marginXY;

    float vyGlobalGoalNew;

    //Maximum linear velocity is zero or goal is reached, do not track linear position
    if(goalPoseMaxV == 0 || yGoalReached)
        vyGlobalGoalNew = 0;

    //Y goal remains
    else{
        vyGlobalGoalNew = pPoseXY*yDiff;
        if(vyGlobalGoalNew > goalPoseMaxV)
            vyGlobalGoalNew = goalPoseMaxV;
        else if(vyGlobalGoalNew < -goalPoseMaxV)
            vyGlobalGoalNew = -goalPoseMaxV;
    }

    //Update goal velocities slowly over time
    if(vyGlobalGoalNew > vyGlobalGoalTracker + VXY_MAX_INCREMENT)
        vyGlobalGoalTracker = vyGlobalGoalTracker + VXY_MAX_INCREMENT;
    else if(vyGlobalGoalNew < vyGlobalGoalTracker - VXY_MAX_INCREMENT)
        vyGlobalGoalTracker = vyGlobalGoalTracker - VXY_MAX_INCREMENT;
    else
        vyGlobalGoalTracker = vyGlobalGoalNew;
}

/**
 * @brief Calculates necessary angular velocity to reach given orientation
 *
 * @param goalless Will not turn off tracking when within marginTheta, thetaGoalReached will always be false
 */
inline void trackOrientation(bool goalless){

    //Get difference of goal and actual orientation between -PI and PI in radians
    float thetaDiff = (goalPoseTheta - poseTheta)/100.0f;
    while(thetaDiff < -180)
        thetaDiff += 360;
    while(thetaDiff > 180)
        thetaDiff -= 360;

    thetaGoalReached = goalless ? false : abs(thetaDiff) < marginTheta;

    float wGlobalGoalNew;

    //If maximum angular velocity is zero, do not track orientation
    if(goalPoseMaxW == 0 || thetaGoalReached)
        wGlobalGoalNew = 0;
    else{
        wGlobalGoalNew = (goalless ? pPoseThetaGoalless : pPoseTheta)*thetaDiff;
        if(wGlobalGoalNew > goalPoseMaxW)
            wGlobalGoalNew = goalPoseMaxW;
        else if(wGlobalGoalNew < -goalPoseMaxW)
            wGlobalGoalNew = -goalPoseMaxW;
    }

    //Update goal velocity slowly over time
    if(wGlobalGoalNew > wGlobalGoalTracker + W_MAX_INCREMENT)
        wGlobalGoalTracker = wGlobalGoalTracker + W_MAX_INCREMENT;
    else if(wGlobalGoalNew < wGlobalGoalTracker - W_MAX_INCREMENT)
        wGlobalGoalTracker = wGlobalGoalTracker - W_MAX_INCREMENT;
    else
        wGlobalGoalTracker = wGlobalGoalNew;
}

/**
 * @brief Calculates the orientation of given vector
 *
 * @param x X component of vector
 * @param y Y component of vector
 * @return Orientation in degrees times 100
 */
inline int getVectorOrientation(float x, float y){
    return (int)(atan2f(y, x)*180*100/M_PI);
}

/**
 * @brief Calculates necessary global velocities to track the composite Bézier curve
 *
 * @param aligned Whether to keep the orientation aligned with respect to the curve tangent orientation
 */
inline void trackPolyBezier(bool aligned){
    switch(polyBezierTrackerState){
        case POLYBEZIER_TRACKER_GOING_TO_START:
            polyBezierGoalReached = false;
            trackPosition();
            wGlobalGoalTracker = 0;
            if(xGoalReached && yGoalReached){
                polyBezierTrackingReset();
                polyBezierTrackerState = POLYBEZIER_TRACKER_TRACKING;
            }
            break;

        case POLYBEZIER_TRACKER_TRACKING:
            polyBezierGoalReached = false;

            //Linear velocity
            if(getPolyBezierNextVelocities(goalPoseMaxV, &vxGlobalGoalTracker, &vyGlobalGoalTracker)){
                polyBezierTrackerState = POLYBEZIER_TRACKER_GOING_TO_FINISH;
                getPolyBezierEnd(&goalPoseX, &goalPoseY);
            }

            //Angular velocity
            if(aligned){
                goalPoseTheta = polyBezierAlignedTheta + getVectorOrientation(vxGlobalGoalTracker, vyGlobalGoalTracker);
                trackOrientation(true);
            }
            else
                wGlobalGoalTracker = goalPoseMaxW;

            break;

        case POLYBEZIER_TRACKER_GOING_TO_FINISH:
            polyBezierGoalReached = false;
            trackPosition();
            if(aligned)
                wGlobalGoalTracker = 0.0f;
            else
                wGlobalGoalTracker = goalPoseMaxW;
            if(xGoalReached && yGoalReached)
                polyBezierTrackerState = POLYBEZIER_TRACKER_FINISHED;
            break;

        case POLYBEZIER_TRACKER_FINISHED:
            polyBezierGoalReached = true;
            vxGlobalGoalTracker = 0;
            vyGlobalGoalTracker = 0;
            wGlobalGoalTracker = 0;
            break;

        default:
            break;
    }
}

void APP_Motion_Initialize(){
    initACtrl(&actrl);
    use_adaptive_ctrl = true;
}

void APP_Motion_Tasks(){

     if(paramRequest){
            bluetoothEventACtrlParameters();                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 
            paramRequest = false; 
        }
    //Turn off everything if kidnapped
    if(kidnapped){
        zeroMeasuredVelocities();
        zeroMotorOutputs();
        return;
    }

    //Estimate velocities
    if(!estimateVelocities())
        return;

    //Do not move if requires hold and is not held
    if(locomotionInteractivityMode == LocomotionInteractivityModeRequiresHold && currentGesture != GestureHold){
        zeroMotorOutputs();
        return;
    }

    //Calculate desired goal velocities depending on the current motion tracker; and calculate whether goals are reached
    bool relevantGoalsReachedNow = false;
    switch(currentMotionTracker){
        case MOTION_TRACKER_POSITION:
            trackPosition();
            wGlobalGoalTracker = 0;
            relevantGoalsReachedNow = xGoalReached && yGoalReached;
            break;

        case MOTION_TRACKER_X_COORDINATE:
            trackXCoordinate();
            vyGlobalGoalTracker = 0;
            wGlobalGoalTracker = 0;
            relevantGoalsReachedNow = xGoalReached;
            break;

        case MOTION_TRACKER_Y_COORDINATE:
            trackYCoordinate();
            vxGlobalGoalTracker = 0;
            wGlobalGoalTracker = 0;
            relevantGoalsReachedNow = yGoalReached;
            break;

        case MOTION_TRACKER_POSE:
            trackPosition();
            trackOrientation(false);
            relevantGoalsReachedNow = xGoalReached && yGoalReached && thetaGoalReached;
            break;

        case MOTION_TRACKER_ORIENTATION:
            vxGlobalGoalTracker = 0;
            vyGlobalGoalTracker = 0;
            trackOrientation(false);
            relevantGoalsReachedNow = thetaGoalReached;
            break;

        case MOTION_TRACKER_X_THETA_COORDINATE:
            trackXCoordinate();
            vyGlobalGoalTracker = 0;
            trackOrientation(false);
            relevantGoalsReachedNow = xGoalReached && thetaGoalReached;
            break;

        case MOTION_TRACKER_Y_THETA_COORDINATE:
            trackYCoordinate();
            vxGlobalGoalTracker = 0;
            trackOrientation(false);
            relevantGoalsReachedNow = yGoalReached && thetaGoalReached;
            break;

        case MOTION_TRACKER_POLYBEZIER:
            trackPolyBezier(false);
            relevantGoalsReachedNow = polyBezierGoalReached;
            break;

        case MOTION_TRACKER_POLYBEZIER_ALIGNED:
            trackPolyBezier(true);
            relevantGoalsReachedNow = polyBezierGoalReached;
            break;

        case MOTION_TRACKER_NONE:
            vxGlobalGoalTracker = 0;
            vyGlobalGoalTracker = 0;
            wGlobalGoalTracker = 0;
            break;

        default:
            break;
    }

    //Broadcast if relevant tracking goals are reached
    if(relevantGoalsReachedNow && !relevantGoalsReached)
        bluetoothEventTrackingGoalReached();
    relevantGoalsReached = relevantGoalsReachedNow;

    //Calculate desired goal velocities depending on the haptics module
    calculateHaptics();

    //Set motor outputs from calculated goal velocities
    setMotorOutputsFromGlobalVelGoals();
}
