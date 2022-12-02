/*
 * Copyright (C) 2017 EPFL
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
 * @file CelluloRobot.h
 * @brief Object that directly represents a Cellulo robot
 * @author Ayberk Özgür
 * @date 2015-05-20
 */

#include "CelluloRobot.h"

#if defined(Q_OS_WIN)
	#define _USE_MATH_DEFINES
	#include <math.h>
#else
	#include <cmath>
#endif

#include "../comm/CelluloBluetoothEnums.h"

namespace Cellulo{

const qreal CelluloRobot::maxEstimatedXYVel = 1000.0;
const qreal CelluloRobot::maxEstimatedW = 50.0;
const qreal CelluloRobot::vMu = 0.0;

CelluloRobot::CelluloRobot(QQuickItem* parent) : CelluloBluetooth(parent){
    keysLongTouched << false << false << false << false << false << false;
    keysTouched << false << false << false << false << false << false;

    connect(this, SIGNAL(touchBegan(int)), this, SLOT(touchKey(int)));
    connect(this, SIGNAL(longTouch(int)), this, SLOT(longTouchKey(int)));
    connect(this, SIGNAL(touchReleased(int)), this, SLOT(releaseKey(int)));

    poseVelControlEnabled = false;
    poseVelControlPeriod = 20;
    poseVelControlEnabledComponents = QVector3D(1, 1, 1);

    poseVelControlMaxLinearVel = 185.0;
    poseVelControlMaxAngularVel = 7.5;

    vxyw = QVector3D(0,0,0);

    velEstimateNeedsReset = true;

    lastPose = QVector3D(0,0,0);
    lastLastTimestamp = 0;
    deltaTime = 0;

    poseVelControlKGoalVel = QVector3D(0.9, 0.9, 0.9);
    poseVelControlKGoalVelErr = QVector3D(0.2, 0.2, 0.2);
    poseVelControlKGoalPoseErr = QVector3D(2.0, 2.0, 2.3);

    connect(this, SIGNAL(kidnappedChanged()), this, SLOT(resetVelEstimate()));
    connect(this, SIGNAL(bootCompleted()), this, SLOT(initialize()));
    connect(this, SIGNAL(connectionStatusChanged()), this, SLOT(initialize()));
    connect(this, SIGNAL(poseChanged(qreal, qreal, qreal)), this, SLOT(spinControllers()));
}

CelluloRobot::~CelluloRobot(){}

void CelluloRobot::setPoseVelControlEnabled(bool enabled){
    if(enabled != poseVelControlEnabled){
        initialize();
        poseVelControlEnabled = enabled;
        emit poseVelControlEnabledChanged();
        emit nextGoalPoseVelRequested(deltaTime);
    }
}

void CelluloRobot::setPoseVelControlPeriod(int period){
    if(period < 0){
        qWarning() << "CelluloRobot::setPoseVelControlPeriod(): Period must be greater than zero.";
        return;
    }

    if(period != poseVelControlPeriod){
        poseVelControlPeriod = period;
        initialize();
        emit poseVelControlPeriodChanged();
    }
}

void CelluloRobot::setGoalPoseAndVelocity(qreal x, qreal y, qreal theta, qreal Vx, qreal Vy, qreal w, bool xEnabled, bool yEnabled, bool thetaEnabled){
    poseVelControlGoalPose = QVector3D(x, y, theta);
    poseVelControlGoalVel = QVector3D(Vx, Vy, w);
    poseVelControlEnabledComponents = QVector3D(xEnabled ? 1.0 : 0.0, yEnabled ? 1.0 : 0.0, thetaEnabled ? 1.0 : 0.0);
    if(!poseVelControlEnabled)
        qWarning() << "CelluloRobot::setGoalPoseAndVelocity(): Warning, poseVelControlEnabled is not true, nothing will happen.";
}

void CelluloRobot::initialize(){
    for(int i=0;i<6;i++)
        releaseKey(i);

    if(getConnectionStatus() == CelluloBluetoothEnums::ConnectionStatusConnected){
        setPoseBcastPeriod(poseVelControlPeriod);
        setTimestampingEnabled(true);

        velEstimateNeedsReset = true;
        lastPose = QVector3D(0,0,0);
        lastLastTimestamp = 0;
        deltaTime = 0;

        emit nextGoalPoseVelRequested(deltaTime);
    }
}

void CelluloRobot::resetVelEstimate(){
    velEstimateNeedsReset = true;
}

void CelluloRobot::spinControllers(){
    estimateVelocities();
    if(poseVelControlEnabled)
        poseVelControlCommandVelocities();
}

void CelluloRobot::estimateVelocities(){
    qreal newTime = getLastTimestamp();
    deltaTime = newTime - lastLastTimestamp;
    QVector3D newPose = QVector3D(getX(), getY(), getTheta());

    QVector3D newVxyw = newPose - lastPose;
    while(newVxyw.z() <= -180)
        newVxyw.setZ(newVxyw.z() + 360);
    while(newVxyw.z() > 180)
        newVxyw.setZ(newVxyw.z() - 360);
    newVxyw.setZ(newVxyw.z()*M_PI/180);

    newVxyw *= 1000.0/deltaTime;
    if(getBcastPeriodMin() < deltaTime && deltaTime < getBcastPeriodMax()){
        if(velEstimateNeedsReset){
            velEstimateNeedsReset = false;
            vxyw = newVxyw;
        }
        else
            vxyw = vMu*vxyw + (1 - vMu)*newVxyw;
    }
    else if(getBcastPeriodMax() <= deltaTime){
        velEstimateNeedsReset = true;
        vxyw = newVxyw;
    }
    else
        velEstimateNeedsReset = true;

    if(vxyw.x() > maxEstimatedXYVel)
        vxyw.setX(maxEstimatedXYVel);
    else if(vxyw.x() < -maxEstimatedXYVel)
        vxyw.setX(-maxEstimatedXYVel);

    if(vxyw.y() > maxEstimatedXYVel)
        vxyw.setY(maxEstimatedXYVel);
    else if(vxyw.y() < -maxEstimatedXYVel)
        vxyw.setY(-maxEstimatedXYVel);

    if(vxyw.z() > maxEstimatedW)
        vxyw.setZ(maxEstimatedW);
    else if(vxyw.z() < -maxEstimatedW)
        vxyw.setZ(-maxEstimatedW);

    lastPose = newPose;
    lastLastTimestamp = newTime;

    emit vxywChanged();
}

void CelluloRobot::poseVelControlCommandVelocities(){
    emit nextGoalPoseVelRequested(deltaTime);

    //Goal velocity component
    QVector3D commandVel = poseVelControlKGoalVel*poseVelControlGoalVel;

    //Goal velocity error component
    commandVel += poseVelControlKGoalVelErr*(poseVelControlGoalVel - vxyw);

    //Goal pose error component
    QVector3D poseErr = poseVelControlGoalPose - QVector3D(getX(), getY(), getTheta());
    while(poseErr.z() > 180.0)
        poseErr.setZ(poseErr.z() - 360.0);
    while(poseErr.z() <= -180.0)
        poseErr.setZ(poseErr.z() + 360.0);
    poseErr.setZ(poseErr.z()/180.0*M_PI);
    commandVel += poseVelControlKGoalPoseErr*poseErr;

    //Clamp goal velocity
    QVector2D linearVel = commandVel.toVector2D();
    qreal linearVelMag = linearVel.length();
    if(linearVelMag > poseVelControlMaxLinearVel){
        linearVel = linearVel/linearVelMag*poseVelControlMaxLinearVel;
        commandVel.setX(linearVel.x());
        commandVel.setY(linearVel.y());
    }
    if(commandVel.z() > poseVelControlMaxAngularVel)
        commandVel.setZ(poseVelControlMaxAngularVel);
    else if(commandVel.z() < -poseVelControlMaxAngularVel)
        commandVel.setZ(-poseVelControlMaxAngularVel);

    commandVel *= poseVelControlEnabledComponents;

    setGoalVelocity(commandVel.x(), commandVel.y(), commandVel.z());
}

void CelluloRobot::touchKey(int key){
    if(!keysTouched[key]){
        keysTouched[key] = true;
        emit keysTouchedChanged();
    }
}

void CelluloRobot::releaseKey(int key){
    if(keysTouched[key]){
        keysTouched[key] = false;
        emit keysTouchedChanged();
    }
    if(keysLongTouched[key]){
        keysLongTouched[key] = false;
        emit keysLongTouchedChanged();
    }
}

void CelluloRobot::longTouchKey(int key){
    if(!keysLongTouched[key]){
        keysLongTouched[key] = true;
        emit keysLongTouchedChanged();
    }
}

}
