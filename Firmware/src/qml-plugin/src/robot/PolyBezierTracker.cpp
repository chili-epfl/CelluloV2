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
 * @file PolyBezierTracker.h
 * @brief Makes CelluloRobots track PolyBezier curves
 * @author Ayberk Özgür
 * @date 2018-07-13
 */

#include "PolyBezierTracker.h"

#include <cmath>

namespace Cellulo{

PolyBezierTracker::PolyBezierTracker(QQuickItem* parent) : QQuickItem(parent) {
    curve = nullptr;
    robot = nullptr;
    setEnabled(false);
}

PolyBezierTracker::~PolyBezierTracker(){
}

void PolyBezierTracker::itemChange(ItemChange change, ItemChangeData const& value){
    if(change == ItemChange::ItemChildRemovedChange){
        PolyBezier* removedChild = qobject_cast<PolyBezier*>(value.item);
        if(removedChild == curve)
            setCurve(nullptr);
    }
    else if(change == ItemChange::ItemChildAddedChange){
        PolyBezier* addedChild = qobject_cast<PolyBezier*>(value.item);
        if(addedChild)
            setCurve(addedChild);
    }
    else if(change == ItemChange::ItemParentHasChanged){
        CelluloRobot* newParent = qobject_cast<CelluloRobot*>(value.item);
        if(newParent)
            setRobot(newParent);
    }

    QQuickItem::itemChange(change, value);
}

void PolyBezierTracker::setCurve(PolyBezier* newCurve){
    if(curve)
        disconnect(curve, SIGNAL(controlPointsChanged()), this, SLOT(updateCurve()));
    curve = newCurve;
    if(curve)
        connect(curve, SIGNAL(controlPointsChanged()), this, SLOT(updateCurve()));
    emit curveChanged();
}

void PolyBezierTracker::setRobot(CelluloRobot* newRobot){
    if(robot)
        disconnect(robot, SIGNAL(nextGoalPoseVelRequested(qreal)), this, SLOT(robotControlLoop(qreal)));
    robot = newRobot;
    if(robot){
        connect(robot, SIGNAL(nextGoalPoseVelRequested(qreal)), this, SLOT(robotControlLoop(qreal)));
        robot->setPoseVelControlPeriod(controlPeriod);
        robot->setPoseVelControlEnabled(true);
    }
    emit robotChanged();
}

void PolyBezierTracker::setControlPeriod(qreal newControlPeriod){
    if(newControlPeriod > 0.0){
        controlPeriod = newControlPeriod;
        if(robot)
            robot->setPoseVelControlPeriod(controlPeriod);
        emit controlPeriodChanged();
    }
    else
        qCritical() << "PolyBezierTracker::setControlPeriod(): Period must be larger than 0, doing nothing!";
}

void PolyBezierTracker::startTracking(){
    if(robot && curve){
        goingToStart = goToStartFirst;
        if(goToStartFirst){
            currentT = 0.0;
            currentR = 0.0;
        }
        else{
            currentT = curve->getClosestT(QVector2D(robot->getX(), robot->getY()));
            currentR = curve->getArcLengthRatioByT(currentT);
        }
        emit trackingPercentageChanged();
        endReachedFlag = false;
        setEnabled(true);
    }
    else
        qCritical() << "PolyBezierTracker::track(): Robot or curve not yet set, doing nothing.";
}

void PolyBezierTracker::updateCurve(){
    endReachedFlag = false;
    QVector2D newP;

    //Check segment addition to the end, currentT should be unchanged
    newP = curve->getPoint(currentT);
    if(newP.x() == currentP.x() && newP.y() == currentP.y()){
        currentR = curve->getArcLengthRatioByT(currentT);
        emit trackingPercentageChanged();
        return;
    }

    //Check segment removal from the beginning, currentT - 1 should correspond to the new t
    newP = curve->getPoint(currentT - 1);
    if(newP.x() == currentP.x() && newP.y() == currentP.y()){
        currentT = currentT - 1;
        currentR = curve->getArcLengthRatioByT(currentT);
        emit trackingPercentageChanged();
        return;
    }

    //Curve changed more than just one removed from the beginning or one added to the end; get closest point and continue from there
    qreal dummyR;
    currentT = curve->getClosest(currentP.toVector2D(), newP, dummyR);
    currentP.setX(newP.x());
    currentP.setY(newP.y());
    currentR = curve->getArcLengthRatioByT(currentT);
    emit trackingPercentageChanged();
}

void PolyBezierTracker::robotControlLoop(qreal deltaTime){
    if(robot && curve){
        if(isEnabled()){
            if(curve->isEmpty()){
                qWarning() << "PolyBezierTracker::robotControlLoop(): Curve is empty, stopping tracker.";
                setEnabled(false);
            }
            else{
                spinLoop(deltaTime);
                emit trackedPoseChanged();
                emit trackedVelocityChanged();

                if(cleanCurve){
                    int numComplete = (int)currentT;
                    if(std::floor(currentT) == currentT && numComplete > 0)
                        numComplete--;
                    curve->removeSegmentsFromStart(numComplete);
                }
            }
        }
        else
            robot->setGoalPoseAndVelocity(0, 0, 0, 0, 0, 0, false, false, false);
    }
    else
        qCritical() << "PolyBezierTracker::robotControlLoop(): Robot or curve not yet set, exiting.";
}

}
