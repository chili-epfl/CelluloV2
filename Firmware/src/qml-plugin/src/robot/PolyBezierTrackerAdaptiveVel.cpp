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
 * @file PolyBezierTrackerAdaptiveVel.cpp
 * @brief Makes CelluloRobots track PolyBezier curves with constant velocity
 * @author Ayberk Özgür
 * @date 2018-07-13
 */

#include "PolyBezierTrackerAdaptiveVel.h"

#include <QtMath>

namespace Cellulo {

PolyBezierTrackerAdaptiveVel::PolyBezierTrackerAdaptiveVel(QQuickItem* parent) : PolyBezierTracker(parent) {}

PolyBezierTrackerAdaptiveVel::~PolyBezierTrackerAdaptiveVel(){}

void PolyBezierTrackerAdaptiveVel::setMaxTrackingVelocity(qreal maxTrackingVelocity){
    if(0.0 < maxTrackingVelocity && maxTrackingVelocity <= 200.0){
        this->maxTrackingVelocity = maxTrackingVelocity;
        emit maxTrackingVelocityChanged();
    }
    else
        qCritical() << "PolyBezierTrackerAdaptiveVel::setMaxTrackingVelocity(): Tracking velocity must be between 0 and 200 mm/s, doing nothing.";
}

void PolyBezierTrackerAdaptiveVel::setMinTrackingVelocity(qreal minTrackingVelocity){
    if(0.0 < minTrackingVelocity && minTrackingVelocity <= 200.0){
        this->minTrackingVelocity = minTrackingVelocity;
        emit minTrackingVelocityChanged();
    }
    else
        qCritical() << "PolyBezierTrackerAdaptiveVel::setMinTrackingVelocity(): Tracking velocity must be between 0 and 200 mm/s, doing nothing.";
}

void PolyBezierTrackerAdaptiveVel::spinLoop(qreal deltaTime){
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
        currentV = (goalDir*maxTrackingVelocity).toVector3D();

        robot->setGoalPoseAndVelocity(currentP.x(), currentP.y(), 0, currentV.x(), currentV.y(), 0, true, true, false);

        if(goalDiff.length() <= GOAL_EPSILON){
            emit startReached();
            goingToStart = false;
        }
    }
    else{
        currentT = curve->getTByArcLengthRatio(currentR);
        currentP = curve->getPoint(currentT).toVector3D();

        //Adapt velocity according to local curvature
        qreal maxCurvature = curve->getMaxCurvature();
        if(maxCurvature > 0){
            qreal currentCurvatureRatio = 1 - curve->getCurvatureByArcLengthRatio(currentR)/maxCurvature;
            currentCurvatureRatio = qPow(currentCurvatureRatio, attenuationPower);
            currentTrackingVelocity = (1 - currentCurvatureRatio)*minTrackingVelocity + currentCurvatureRatio*maxTrackingVelocity;
        }
        else
            currentTrackingVelocity = maxTrackingVelocity;
        emit currentTrackingVelocityChanged();

        if(currentR < 1.0)
            currentV = (curve->getTangent(currentT).normalized()*currentTrackingVelocity).toVector3D();
        else{
            QVector2D goalDir = (curve->getPoint(curve->getTByArcLengthRatio(1.0)) - currentRobotPos)/(10*GOAL_EPSILON);
            if(goalDir.length() > 1.0)
                goalDir.normalize();
            currentV = (goalDir*maxTrackingVelocity).toVector3D();
        }

        robot->setGoalPoseAndVelocity(currentP.x(), currentP.y(), 0, currentV.x(), currentV.y(), 0, true, true, false);

        if(currentR < 1.0){
            currentR += (currentTrackingVelocity*deltaTime/1000.0)/curve->getArcLength();
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
