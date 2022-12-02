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
 * @file CelluloZoneTypes.cpp
 * @brief Source for zone type tools
 * @author Joanna Salathé
 * @date 2016-03-04
 */

#include "CelluloZoneTypes.h"

#include "CelluloZoneAngle.h"
#include "CelluloZoneAngleInterval.h"
#include "CelluloZoneCircle.h"
#include "CelluloZoneRectangle.h"
#include "CelluloZoneLine.h"
#include "CelluloZonePoint.h"
#include "CelluloZonePolygon.h"
#include "CelluloZonePolyBezier.h"

namespace Cellulo{

CelluloZoneTypes::CelluloZoneTypes(QObject* parent) : QObject(parent){}

CelluloZoneTypes::~CelluloZoneTypes(){}

//TODO: PUT THIS IN CELLULO_ENUM_DECL
CelluloZoneTypes::ZoneType CelluloZoneTypes::typeFromString(const QString &typeName){
    const QMetaObject &mo = CelluloZoneTypes::staticMetaObject;
    int index = mo.indexOfEnumerator("ZoneType");
    QMetaEnum metaEnum = mo.enumerator(index);
    int value = metaEnum.keyToValue(qPrintable(typeName));
    return static_cast<CelluloZoneTypes::ZoneType>(value);
}

CelluloZone* CelluloZoneTypes::newZoneFromType(ZoneType type){
    switch(type){
        case ANGLEDIFFERENCE:
            return (CelluloZone*)(new CelluloZoneAngleDifference());
        case ANGLEINTERVALINNER:
            return (CelluloZone*)(new CelluloZoneAngleIntervalInner());
        case ANGLEINTERVALBORDER:
            return (CelluloZone*)(new CelluloZoneAngleIntervalBorder());
        case ANGLEINTERVALDISTANCE:
            return (CelluloZone*)(new CelluloZoneAngleIntervalDistance());
        case CIRCLEINNER:
            return (CelluloZone*)(new CelluloZoneCircleInner());
        case CIRCLEBORDER:
            return (CelluloZone*)(new CelluloZoneCircleBorder());
        case CIRCLEDISTANCE:
            return (CelluloZone*)(new CelluloZoneCircleDistance());
        case RECTANGLEINNER:
            return (CelluloZone*)(new CelluloZoneRectangleInner());
        case RECTANGLEBORDER:
            return (CelluloZone*)(new CelluloZoneRectangleBorder());
        case RECTANGLEDISTANCE:
            return (CelluloZone*)(new CelluloZoneRectangleDistance());
        case LINESEGMENTDISTANCE:
            return (CelluloZone*)(new CelluloZoneLineSegmentDistance());
        case LINEDISTANCESIGNED:
            return (CelluloZone*)(new CelluloZoneLineDistanceSigned());
        case LINEBORDER:
            return (CelluloZone*)(new CelluloZoneLineBorder());
        case POINTDISTANCE:
            return (CelluloZone*)(new CelluloZonePointDistance());
        case RPOLYGONINNER:
            return (CelluloZone*)(new CelluloZoneRegularPolygonInner());
        case RPOLYGONBORDER:
            return (CelluloZone*)(new CelluloZoneRegularPolygonBorder());
        case RPOLYGONDISTANCE:
            return (CelluloZone*)(new CelluloZoneRegularPolygonDistance());
        case IRPOLYGONINNER:
            return (CelluloZone*)(new CelluloZoneIrregularPolygonInner());
        case IRPOLYGONBORDER:
            return (CelluloZone*)(new CelluloZoneIrregularPolygonBorder());
        case IRPOLYGONDISTANCE:
            return (CelluloZone*)(new CelluloZoneIrregularPolygonDistance());
        case POLYBEZIERCLOSESTT:
            return (CelluloZone*)(new CelluloZonePolyBezierClosestT());
        case POLYBEZIERXT:
            return (CelluloZone*)(new CelluloZonePolyBezierXT());
        case POLYBEZIERYT:
            return (CelluloZone*)(new CelluloZonePolyBezierYT());
        case POLYBEZIERDISTANCE:
            return (CelluloZone*)(new CelluloZonePolyBezierDistance());
        case POLYBEZIERBORDER:
            return (CelluloZone*)(new CelluloZonePolyBezierBorder());
        case POLYBEZIERINNER:
            return (CelluloZone*)(new CelluloZonePolyBezierInner());
        default:
            qWarning() << "CelluloZoneTypes::newZoneFromType(): Unknown type.";
            return NULL;
    }
}

}
