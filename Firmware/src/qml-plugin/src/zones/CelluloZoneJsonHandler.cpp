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
 * @file CelluloZoneJsonHandler.cpp
 * @brief Source for zone JSON tools
 * @author Joanna Salathé
 * @date 2016-03-04
 */

#include "CelluloZoneJsonHandler.h"

#include "CelluloZoneCircle.h"
#include "CelluloZoneRectangle.h"
#include "CelluloZoneLine.h"
#include "CelluloZonePoint.h"
#include "CelluloZonePolygon.h"

namespace Cellulo{

CelluloZoneJsonHandler::CelluloZoneJsonHandler(QObject* parent) : QObject(parent){
}

CelluloZoneJsonHandler::~CelluloZoneJsonHandler(){
}

void CelluloZoneJsonHandler::saveZones(const QVariantList& zones, QString path){
    QList<CelluloZone*> typedZones;
    for(const QVariant& zone : zones)
        typedZones.append(zone.value<CelluloZone*>());
    saveZones(typedZones, path);
}

void CelluloZoneJsonHandler::saveZones(const QList<CelluloZone*>& zones, QString path){
    QFile saveFile(path);
    if(!saveFile.open(QIODevice::WriteOnly)){
        qWarning() << "CelluloZoneJsonHandler::saveZones(): Couldn't write into file.";
        return;
    }

    QJsonArray jsonZones;
    write(jsonZones, zones);
    saveFile.write(QJsonDocument(jsonZones).toJson());
}

QVariantList CelluloZoneJsonHandler::loadZonesQML(QString path){
    QList<CelluloZone*> zonesList = loadZonesCPP(path);
    QVariantList zonesListVariant;
    for(const auto& zone : zonesList)
        zonesListVariant.append(QVariant::fromValue(zone));
    return zonesListVariant;
}

QList<CelluloZone*> CelluloZoneJsonHandler::loadZonesCPP(QString path){
    QFile loadFile(path);
    if(!loadFile.open(QIODevice::ReadOnly)){
        qWarning("CelluloZoneJsonHandler::loadZones(): Couldn't open file.");
        return QList<CelluloZone*>();
    }

    QJsonArray jsonZones = QJsonDocument::fromJson(loadFile.readAll()).array();
    return read(jsonZones);
}

void CelluloZoneJsonHandler::write(QJsonArray& jsonZones, QList<CelluloZone*> zones){
    for(CelluloZone* zone : zones){
        QJsonObject obj;
        zone->write(obj);
        jsonZones.append(obj);
    }
}

QList<CelluloZone*> CelluloZoneJsonHandler::read(const QJsonArray& jsonZones){
    QList<CelluloZone*> zones;
    for(const QJsonValue& val : jsonZones){
        QJsonObject jsonZone = val.toObject();
        auto type = CelluloZoneTypes::typeFromString(jsonZone["type"].toString());
        CelluloZone* newZone = CelluloZoneTypes::newZoneFromType(type);
        if(newZone){
            newZone->read(jsonZone);
            zones.append(newZone);
        }
        else
            qWarning() << "CelluloZoneJsonHandler::read(): Could not create zone, unknown or undefined type.";
    }
    return zones;
}

}
