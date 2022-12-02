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
 * @file PolyBezierTrackerConstAccel.cpp
 * @brief Makes CelluloRobots track PolyBezier curves with constant velocity and finite constant acceleration
 * @author Ayberk Özgür
 * @date 2018-07-13
 */

#include "PolyBezierTrackerConstAccel.h"

namespace Cellulo {

PolyBezierTrackerConstAccel::PolyBezierTrackerConstAccel(QQuickItem* parent) : PolyBezierTracker(parent) {}

PolyBezierTrackerConstAccel::~PolyBezierTrackerConstAccel(){}

void PolyBezierTrackerConstAccel::setTrackingVelocity(qreal trackingVelocity){
    if(0.0 < trackingVelocity && trackingVelocity <= 185.0){
        this->trackingVelocity = trackingVelocity;
        emit trackingVelocityChanged();
    }
    else
        qCritical() << "PolyBezierTrackerConstAccel::setTrackingVelocity(): Tracking velocity must be between 0 and 185 mm/s, doing nothing.";
}

void PolyBezierTrackerConstAccel::setTrackingAcceleration(qreal trackingAcceleration){
    if(0.0 < trackingAcceleration){
        this->trackingAcceleration = trackingAcceleration;
        emit trackingAccelerationChanged();
    }
    else
        qCritical() << "PolyBezierTrackerConstAccel::setTrackingAcceleration(): Tracking acceleration must be larger than 0 mm/s^2, doing nothing.";
}

void PolyBezierTrackerConstAccel::spinLoop(qreal deltaTime){
    if(deltaTime < 0.5*controlPeriod)
        deltaTime = 0.5*controlPeriod;
    else if(deltaTime > 1.5*controlPeriod)
        deltaTime = 1.5*controlPeriod;

    QVector2D currentRobotPos(robot->getX(), robot->getY());
    if(goingToStart){
        currentP = curve->getPoint(0.0).toVector3D();
        QVector2D goalDiff = currentP.toVector2D() - currentRobotPos;
        QVector2D goalDir = goalDiff/(10*GOAL_EPSILON);
        if(goalDir.length() > 1.0)
            goalDir.normalize();
        currentV = (goalDir*trackingVelocity).toVector3D();

        robot->setGoalPoseAndVelocity(currentP.x(), currentP.y(), 0, currentV.x(), currentV.y(), 0, true, true, false);

        if(goalDiff.length() <= GOAL_EPSILON){
            emit startReached();
            goingToStart = false;
        }
    }
    else{
        currentT = curve->getTByArcLengthRatio(currentR);
        currentP = curve->getPoint(currentT).toVector3D();
        if(currentR < 1.0)
            currentV = (curve->getTangent(currentT).normalized()*currentVelocity).toVector3D();
        else{
            QVector2D goalDir = (curve->getPoint(curve->getTByArcLengthRatio(1.0)) - currentRobotPos)/(10*GOAL_EPSILON);
            if(goalDir.length() > 1.0)
                goalDir.normalize();
            currentV = (goalDir*trackingVelocity).toVector3D();
        }

        robot->setGoalPoseAndVelocity(currentP.x(), currentP.y(), 0, currentV.x(), currentV.y(), 0, true, true, false);

        if(currentR < 1.0){
            qreal curveLength = curve->getArcLength();
            qreal lengthLeft = (1.0 - currentR)*curveLength;
            qreal timeNeededToStop = currentVelocity/trackingAcceleration;
            qreal lengthTraveledWhenStopping = currentVelocity*timeNeededToStop - 0.5*trackingAcceleration*timeNeededToStop*timeNeededToStop;

            //Should decelerate
            if(lengthTraveledWhenStopping >= lengthLeft){
                currentVelocity -= deltaTime/1000.0*trackingAcceleration;
                if(currentVelocity < 0.0)
                    currentVelocity = 0.0;
            }

            //Should accelerate
            else if(currentVelocity < trackingVelocity){
                currentVelocity += deltaTime/1000.0*trackingAcceleration;
                if(currentVelocity > trackingVelocity)
                    currentVelocity = trackingVelocity;
            }

            currentR += (currentVelocity*deltaTime/1000.0)/curveLength;
            if(currentR > 1.0)
                currentR = 1.0;
            emit trackingPercentageChanged();
        }
        else{
            if(currentRobotPos.distanceToPoint(curve->getPoint(curve->getTByArcLengthRatio(1.0))) <= GOAL_EPSILON){
                if(!endReachedFlag)
                    emit endReached();
                endReachedFlag = true;
                if(stopWhenGoalReached)
                    setEnabled(false);
            }
            else
                endReachedFlag = false;
        }
    }
}

}
