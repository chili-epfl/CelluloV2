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
 * @file CelluloRobotPoolClient.cpp
 * @brief Uses the robots at a cellulo-robot-pool
 * @author Ayberk Özgür
 * @date 2017-03-21
 */

#include "CelluloRobotPoolClient.h"

#include <QMetaObject>
#include <QQmlEngine>
#include <QVariant>

namespace Cellulo {

CelluloRobotPoolClient::CelluloRobotPoolClient(QQuickItem* parent) : CelluloLocalRelayClient(parent){
    robotComponent = nullptr;
    createRobotFunction = QJSValue(QJSValue::NullValue);

    connect(this, SIGNAL(unknownRobotAtServer(QString)), this, SLOT(createAddSelectRobot(QString)));
    connectToServer();
}

CelluloRobotPoolClient::~CelluloRobotPoolClient(){}

void CelluloRobotPoolClient::setRobotComponent(QQmlComponent* robotComponent){
    if(this->robotComponent != robotComponent){
        if(this->robotComponent)
            robotComponent->deleteLater();
        this->robotComponent = robotComponent;
        if(robotComponent){
            QQmlEngine::setObjectOwnership(robotComponent, QQmlEngine::CppOwnership);
            robotComponent->setParent(this);
        }
        emit robotComponentChanged();
    }
}

void CelluloRobotPoolClient::setCreateRobot(QJSValue const& function){
    if(!function.isCallable()){
        qWarning() << "CelluloRobotPoolClient::setCreateRobot(): Passed object must be a callable function.";
        return;
    }

    createRobotFunction = function;
    emit createRobotChanged();
}

void CelluloRobotPoolClient::createAddSelectRobot(QString macAddr){
    CelluloBluetooth* newRobot = nullptr;

    //Try robotComponent first
    if(robotComponent){
        if(!createRobotFunction.isNull())
            qWarning() << "CelluloRobotPoolClient::createAddSelectRobot(): robotComponent object and createRobot function are both provided, ignoring createRobot!";

        newRobot = qobject_cast<CelluloBluetooth*>(robotComponent->create());
        if(!newRobot)
            qCritical() << "CelluloRobotPoolClient::createAddSelectRobot(): robotComponent must encapsulate a CelluloBluetooth-derived object!";
        else
            QQmlEngine::setObjectOwnership(newRobot, QQmlEngine::JavaScriptOwnership);
    }

    //Try createRobot() next
    else if(!createRobotFunction.isNull()){
        QJSValue retval = createRobotFunction.call(QJSValueList() << QJSValue(macAddr));
        if(retval.isError())
            qCritical() << "CelluloRobotPoolClient::createAddSelectRobot(): Error while calling function at createRobot: " << retval.toString();
        else{
            QObject* obj = retval.toQObject();
            if(obj == nullptr)
                qWarning() << "CelluloRobotPoolClient::createAddSelectRobot(): Function at createRobot must return a non-null CelluloBluetooth or CelluloBluetooth-derived object.";
            else{
                newRobot = qobject_cast<CelluloBluetooth*>(obj);
                if(newRobot == nullptr)
                    qWarning() << "CelluloRobotPoolClient::createAddSelectRobot(): Function at createRobot must create and return a CelluloBluetooth or CelluloBluetooth-derived object (such as CelluloRobot).";
            }
        }
    }

    //Neither are there, go with a standard CelluloBluetooth
    else{
        newRobot = new CelluloBluetooth();
        QQmlEngine::setObjectOwnership(newRobot, QQmlEngine::JavaScriptOwnership);
    }

    if(newRobot){
        newRobot->setAutoConnect(false);
        newRobot->setMacAddr(macAddr);
        newRobot->setParent(this);
        newRobot->setParentItem(this);
        addRobot(newRobot, true);
        emit newRobotFound(newRobot);
    }
    else
        qCritical() << "CelluloRobotPoolClient::createAddSelectRobot(): Failed to add robot!";
}

}
