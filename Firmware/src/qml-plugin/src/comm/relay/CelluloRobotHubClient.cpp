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
 * @file CelluloRobotHubClient.cpp
 * @brief Uses the robots at a cellulorobothubd
 * @author Ayberk Özgür
 * @date 2018-08-30
 */

#include "CelluloRobotHubClient.h"

#include <QMetaObject>
#include <QQmlEngine>
#include <QVariant>

namespace Cellulo {

CelluloRobotHubClient::CelluloRobotHubClient(QQuickItem* parent) : CelluloTcpRelayClient(parent){
    robotComponent = nullptr;
    createRobotFunction = QJSValue(QJSValue::NullValue);

    connect(this, SIGNAL(unknownRobotAtServer(QString)), this, SLOT(createAddSelectRobot(QString)));
    connectToServer();
}

CelluloRobotHubClient::~CelluloRobotHubClient(){}

void CelluloRobotHubClient::setRobotComponent(QQmlComponent* robotComponent){
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

void CelluloRobotHubClient::setCreateRobot(QJSValue const& function){
    if(!function.isCallable()){
        qWarning() << "CelluloRobotHubClient::setCreateRobot(): Passed object must be a callable function.";
        return;
    }

    createRobotFunction = function;
    emit createRobotChanged();
}

void CelluloRobotHubClient::createAddSelectRobot(QString macAddr){
    CelluloBluetooth* newRobot = nullptr;

    //Try robotComponent first
    if(robotComponent){
        if(!createRobotFunction.isNull())
            qWarning() << "CelluloRobotHubClient::createAddSelectRobot(): robotComponent object and createRobot function are both provided, ignoring createRobot!";

        newRobot = qobject_cast<CelluloBluetooth*>(robotComponent->create());
        if(!newRobot)
            qCritical() << "CelluloRobotHubClient::createAddSelectRobot(): robotComponent must encapsulate a CelluloBluetooth-derived object!";
        else
            QQmlEngine::setObjectOwnership(newRobot, QQmlEngine::JavaScriptOwnership);
    }

    //Try createRobot() next
    else if(!createRobotFunction.isNull()){
        QJSValue retval = createRobotFunction.call(QJSValueList() << QJSValue(macAddr));
        if(retval.isError())
            qCritical() << "CelluloRobotHubClient::createAddSelectRobot(): Error while calling function at createRobot: " << retval.toString();
        else{
            QObject* obj = retval.toQObject();
            if(obj == nullptr)
                qWarning() << "CelluloRobotHubClient::createAddSelectRobot(): Function at createRobot must return a non-null CelluloBluetooth or CelluloBluetooth-derived object.";
            else{
                newRobot = qobject_cast<CelluloBluetooth*>(obj);
                if(newRobot == nullptr)
                    qWarning() << "CelluloRobotHubClient::createAddSelectRobot(): Function at createRobot must create and return a CelluloBluetooth or CelluloBluetooth-derived object (such as CelluloRobot).";
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
        qCritical() << "CelluloRobotHubClient::createAddSelectRobot(): Failed to add robot!";
}

}
