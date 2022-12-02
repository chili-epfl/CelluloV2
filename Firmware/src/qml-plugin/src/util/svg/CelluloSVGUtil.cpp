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
 * @file CelluloSVGUtil.h
 * @brief Source for SVG file parsing and dumping utils
 * @author Ayberk Özgür
 * @date 2016-07-26
 */

#include "CelluloSVGUtil.h"

#include <QDebug>
#include <QUrl>
#include <QFile>
#include <cstdio>
#include <cstring>
#include <cmath>
#include <QVector2D>
#define NANOSVG_IMPLEMENTATION
#include "nanosvg/src/nanosvg.h"
#include "../math/CelluloMathUtil.h"
#include "../../zones/CelluloZoneJsonHandler.h"

namespace Cellulo{

CelluloSVGUtil::CelluloSVGUtil(QObject* parent) : QObject(parent){}

CelluloSVGUtil::~CelluloSVGUtil(){}

QString CelluloSVGUtil::dumpAllPathsToJSON(const QString& inSVGFile, const QString& outJSONFile, const QString& type, const QString& name, float dpi, bool optimize){
    Q_UNUSED(type);

    //Get Zones from SVG
    QList<CelluloZone*> zones = loadZonesCPP(inSVGFile, name, dpi, optimize);

    //Output file
    QString outFilePath = QUrl(outJSONFile).toLocalFile();
    if(outFilePath.isEmpty())
        outFilePath = outJSONFile;

    //Write to output
    CelluloZoneJsonHandler::saveZones(zones, outFilePath);

    return "CelluloSVGUtil::dumpAllPathsToJSON(): Dumped " + QString::number(zones.size()) + " zones.";
}

QVariantList CelluloSVGUtil::loadZonesQML(const QString& inSVGFile, const QString& name, float dpi, bool optimize){
    QList<CelluloZone*> zonesList = loadZonesCPP(inSVGFile, name, dpi, optimize);
    QVariantList zonesListVariant;
    for(const auto& zone : zonesList)
        zonesListVariant.append(QVariant::fromValue(zone));
    return zonesListVariant;
}

QList<CelluloZone*> CelluloSVGUtil::loadZonesCPP(const QString& inSVGFile, const QString& name, float dpi, bool optimize){
    //Input file
    QString inFilePath = QUrl(inSVGFile).toLocalFile();
    if(inFilePath.isEmpty())
        inFilePath = inSVGFile;

    struct NSVGimage* image = nsvgParseFromFile(inFilePath.toLatin1(), "mm", dpi);
    QList<CelluloZone*> zones;

    if(!image)
        return zones;

    int numZone = 0;

    for(NSVGshape* shape = image->shapes; shape != NULL; shape = shape->next)
        for(NSVGpath* path = shape->paths; path != NULL; path = path->next){

            //Dump each path as one zone
            CelluloZone* newZone;

            QList<QVector2D> controlPoints;

            //Get controlPoints
            for(int i=0; i<2*path->npts; i+=2){
                if(!path->closed || (path->closed && i != (2*path->npts)-2)){
                    controlPoints.append(QVector2D(path->pts[i], path->pts[i + 1]));
                }
            }

            //Get zone
            if(optimize)
                newZone = getMoreAccurateTypeFromBezier(controlPoints);
            else
                newZone = getBezierFromControlPoints(controlPoints);

            //Name zone
            if(newZone){
                newZone->setName(name +  QString::number(numZone));
                numZone++;
                zones.append(newZone);
                qDebug() << "Extracted zone of type " << newZone->getType() << " named " << newZone->getName();
            }
            else
                qDebug() << "Failed extracting zone";
        }

    nsvgDelete(image);

    return zones;
}

CelluloZone* CelluloSVGUtil::getMoreAccurateTypeFromBezier(QList<QVector2D> controlPoints){
    CelluloZone* zone;
    if(bezierIsPoly(controlPoints)){
        QList<QVector2D> polyPoints = getPolyPointsFromBezier(controlPoints);
        // TODO update zone architecture to have a constructor with attribute arg
        if(polyIsPoint(polyPoints)) {
            zone = getPointFromPolyPoints(polyPoints);
        } else if(polyIsLine(polyPoints)) {
            zone = getLineFromPolyPoints(polyPoints);
        } else if(polyIsRectangle(polyPoints)) {
            zone = getRectangleFromPolyPoints(polyPoints);
        } else if(polyIsCircle(polyPoints)) {
            zone = getCircleFromPolyPoints(polyPoints);
        } else if(polyIsRegular(polyPoints)) {
            zone = getRegularPolygonFromPolyPoints(polyPoints);
        } else {
            zone = getIrregularPolygonFromPolyPoints(polyPoints);
        }
    } else {
        zone = getBezierFromControlPoints(controlPoints);
    }
    return zone;
}
QList<QVector2D> CelluloSVGUtil::getPolyPointsFromBezier(QList<QVector2D> controlPoints){
    QList<QVector2D> polyPoints;
    for (int var = 0; var < controlPoints.length(); var = var + 3) {
        polyPoints.append(QVector2D(controlPoints[var].x(), controlPoints[var].y()));
    }
    return polyPoints;
}

bool CelluloSVGUtil::bezierIsPoly(QList<QVector2D> controlPoints){
    bool isPoly = true;
    if((controlPoints.length()%3 == 0 || controlPoints.length() == 4)){
        for (int var = 0; var+2 < controlPoints.length(); var = var + 3) {
            bool vectCollinear = CelluloMathUtil::collinear(controlPoints[var+2],controlPoints[var+1], controlPoints[var]);
            if(!vectCollinear){
                isPoly = false;
            }
        }
    } else {
        isPoly = false;
    }
    return isPoly;
}

bool CelluloSVGUtil::polyIsSmallEnoughToBeAPoint(QList<QVector2D> polyPoints){
    Q_UNUSED(polyPoints)
    //TODO get outbounding rect value and if < than thresolf then it is a point
    return false;
}

bool CelluloSVGUtil::polyIsPoint(QList<QVector2D> polyPoints){
    return (polyPoints.length() == 1 || polyIsSmallEnoughToBeAPoint(polyPoints));
}

bool CelluloSVGUtil::polyIsLine(QList<QVector2D> polyPoints){
    return (polyPoints.length() == 2);
}

bool CelluloSVGUtil::polyIsRectangle(QList<QVector2D> polyPoints){
    if(polyPoints.length()==4){
        //TODO compute better way to tell if rectangle -> e.g closed polygon, 4 lines, parallel to x/y with tolerance of 1/100mm
        return (polyPoints[0].y() == polyPoints[1].y()) && (polyPoints[1].x() == polyPoints[2].x()) && (polyPoints[2].y() == polyPoints[3].y()) && (polyPoints[3].x() == polyPoints[0].x());
    } else {
        return false;
    }
}

bool CelluloSVGUtil::polyIsCircle(QList<QVector2D> polyPoints){
    Q_UNUSED(polyPoints)
    //TODO
    return false;
}

bool CelluloSVGUtil::polyIsRegular(QList<QVector2D> polyPoints){
    Q_UNUSED(polyPoints)
    //TODO
    return false;
}

CelluloZonePointDistance* CelluloSVGUtil::getPointFromPolyPoints(QList<QVector2D> polyPoints){
    CelluloZonePointDistance* newZonePoint = new CelluloZonePointDistance();
    newZonePoint->setX(polyPoints[0].x());
    newZonePoint->setY(polyPoints[0].y());
    return newZonePoint;
}

CelluloZoneLineSegmentDistance* CelluloSVGUtil::getLineFromPolyPoints(QList<QVector2D> polyPoints){
    CelluloZoneLineSegmentDistance* newZoneLine = new CelluloZoneLineSegmentDistance();
    newZoneLine->setX1(polyPoints[0].x());
    newZoneLine->setY1(polyPoints[0].y());
    newZoneLine->setX2(polyPoints[1].x());
    newZoneLine->setY2(polyPoints[1].y());
    return newZoneLine;
}

CelluloZoneRectangleDistance* CelluloSVGUtil::getRectangleFromPolyPoints(QList<QVector2D> polyPoints){
    CelluloZoneRectangleDistance* newZoneRectangle = new CelluloZoneRectangleDistance();
    newZoneRectangle->setX(polyPoints[0].x());
    newZoneRectangle->setY(polyPoints[0].y());
    newZoneRectangle->setWidth(polyPoints[1].x()-polyPoints[0].x());
    newZoneRectangle->setHeight(polyPoints[2].y()-polyPoints[0].y());
    return newZoneRectangle;
}

CelluloZoneCircleDistance* CelluloSVGUtil::getCircleFromPolyPoints(QList<QVector2D> polyPoints){
    Q_UNUSED(polyPoints)

    CelluloZoneCircleDistance* newZoneCircle = new CelluloZoneCircleDistance();
    //TODO
    return newZoneCircle;
}

CelluloZoneRegularPolygonDistance* CelluloSVGUtil::getRegularPolygonFromPolyPoints(QList<QVector2D> polyPoints){
    Q_UNUSED(polyPoints)

    CelluloZoneRegularPolygonDistance* newZoneRegularPoly = new CelluloZoneRegularPolygonDistance();
    //TODO
    return newZoneRegularPoly;
}

CelluloZoneIrregularPolygonDistance* CelluloSVGUtil::getIrregularPolygonFromPolyPoints(QList<QVector2D> polyPoints){
    CelluloZoneIrregularPolygonDistance* newZoneIrPolygon = new CelluloZoneIrregularPolygonDistance();
    newZoneIrPolygon->setVertices(polyPoints);
    return newZoneIrPolygon;
}

CelluloZonePolyBezierClosestT* CelluloSVGUtil::getBezierFromControlPoints(QList<QVector2D> controlPoints){
    CelluloZonePolyBezierClosestT* newZonePolyBezier = new CelluloZonePolyBezierClosestT();
    QList<QVariant> variantControlPoints;
    for(QVector2D controlPoint: controlPoints){
        variantControlPoints.append(QVariant::fromValue(controlPoint));
    }
    newZonePolyBezier->setControlPoints(variantControlPoints);
    return newZonePolyBezier;
}

}
