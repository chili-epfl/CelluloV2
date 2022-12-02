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
 * @file CelluloZone.cpp
 * @brief Source for base class zone architecture
 * @author Joanna Salathé
 * @date 2016-03-04
 */

#include "CelluloZone.h"
#include "CelluloZoneClient.h"

namespace Cellulo{

CelluloZone::CelluloZone(QQuickItem* parent) : QQuickItem(parent){
    active = true;
    name = "anonymousZone";

    paintedItem = NULL;
}

CelluloZone::~CelluloZone(){
    delete paintedItem;
}

void CelluloZone::onClientPoseChanged(qreal x, qreal y, qreal theta){
    CelluloZoneClient* client = qobject_cast<CelluloZoneClient*>(QObject::sender());

    if(client){

        //Calculate the new value associated with the client whose pose has changed
        qreal newVal = calculate(x, y, theta);

        //We already have a previous value for this client
        if(clientsLastValues.contains(client)){

            //If the value of this zone changed, alert the client
            qreal& oldValRef = clientsLastValues[client];
            if(oldValRef != newVal){
                emit client->zoneValueChanged(this, newVal);
                oldValRef = newVal;
            }
        }

        //We don't have a previous value for this client, simply create it and alert the client
        else{
            client->zoneValueChanged(this, newVal);
            clientsLastValues[client] = newVal;
        }
    }
    else
        qDebug() << "CelluloZone::onClientPoseChanged(): Zone can only connect to a CelluloZoneClient-derived object.";
}

void CelluloZone::write(QJsonObject& json){
    json["type"] = CelluloZoneTypes::ZoneTypeString(type);
    json["name"] = name;
}

void CelluloZone::read(const QJsonObject& json){
    type = CelluloZoneTypes::typeFromString(json["type"].toString());
    name = json["name"].toString();
}

void CelluloZone::setName(QString newName){
    if(newName != name){
        name = newName;
        emit(nameChanged());
    }
}

void CelluloZone::setActive(float newActive){
    if(newActive != active){
        active = newActive;
        emit(activeChanged());
    }
}

void CelluloZone::updatePaintedItem(){
    if(paintedItem)
        paintedItem->update();
    else
        qWarning() << "CelluloZone::updatePaintedItem(): Called when there is no paintedItem.";
}

CelluloZonePaintedItem* CelluloZone::createPaintedItem(QQuickItem* parent, QColor color, qreal physicalPlaygroundWidth, qreal physicalPlaygroundHeight){
    delete paintedItem;
    paintedItem = new CelluloZonePaintedItem(parent);
    paintedItem->setColor(color);
    paintedItem->setAssociatedZone(this);
    paintedItem->setPhysicalPlaygroundWidth(physicalPlaygroundWidth);
    paintedItem->setPhysicalPlaygroundHeight(physicalPlaygroundHeight);
    return paintedItem;
}

void CelluloZone::setPaintedItem(CelluloZonePaintedItem* newPaintedItem){
    paintedItem = newPaintedItem;
}

}
