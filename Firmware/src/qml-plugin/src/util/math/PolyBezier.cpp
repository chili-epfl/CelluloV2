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
 * @file PolyBezier.cpp
 * @brief A composite cubic Bézier curve
 * @author Ayberk Özgür
 * @date 2018-07-10
 */

#include "PolyBezier.h"

#include "CelluloMathUtil.h"

#include <QPointF>
#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonObject>

namespace Cellulo{

PolyBezier::PolyBezier(QQuickItem* parent) : QQuickItem(parent) {
    lastLookupBeginIndex = 0;
}

QVariantList PolyBezier::getControlPoints() const {
    QVariantList points;
    if(segments.size() > 0){
        points.push_back(QVariant(segments[0].getControlPoint(0)));
        for(auto& segment : segments){
            points.push_back(QVariant(segment.getControlPoint(1)));
            points.push_back(QVariant(segment.getControlPoint(2)));
            points.push_back(QVariant(segment.getControlPoint(3)));
        }
    }
    return points;
}

void PolyBezier::setControlPoints(QVariantList const& newControlPoints){
    int newSize = newControlPoints.size();
    if(newSize < 4){
        qWarning() << "PolyBezier::setControlPoints(): At least 4 points must be provided, not initializing.";
        return;
    }
    int newSizeExtra = (newSize - 1) % 3;
    if(newSizeExtra > 0){
        qWarning() << "PolyBezier::setControlPoints(): New control points contain " << newSizeExtra << " extra points, they will be discarded.";
        newSize -= newSizeExtra;
    }

    segments.clear();
    for(int i=0; i+3<newSize; i+=3)
        segments.push_back(
            CubicBezier(
                newControlPoints[i].value<QVector2D>(),
                newControlPoints[i + 1].value<QVector2D>(),
                newControlPoints[i + 2].value<QVector2D>(),
                newControlPoints[i + 3].value<QVector2D>(),
                false)
            );

    invalidateCalc();
    emit controlPointsChanged();
}

void PolyBezier::loadFromFile(QString const& path){
    QFile loadFile(path);
    if(!loadFile.open(QIODevice::ReadOnly)){
        qWarning("PolyBezier::loadZones(): Couldn't open file.");
        return;
    }

    QJsonArray controlPointsJson = QJsonDocument::fromJson(loadFile.readAll()).array();
    QVariantList newControlPoints;
    for(const QJsonValue& controlPointJson : controlPointsJson){
        const QJsonObject& controlPointObj = controlPointJson.toObject();
        newControlPoints.append(QVariant(QVector2D(controlPointObj["x"].toDouble(), controlPointObj["y"].toDouble())));
    }
    setControlPoints(newControlPoints);
}

void PolyBezier::removeSegmentsFromStart(int num){
    if(segments.size() >= num){
        if(num > 0){
            segments.remove(0, num);
            invalidateCalc();
            emit controlPointsChanged();
        }
    }
    else
        qWarning() << "PolyBezier::removeSegmentsFromStart(): Not enough segments to remove, doing nothing.";
}

bool PolyBezier::isEmpty() const {
    return segments.isEmpty();
}

void PolyBezier::clear(){
    if(!segments.isEmpty()){
        segments.clear();
        invalidateCalc();
        emit controlPointsChanged();
    }
}

void PolyBezier::start(QVector2D const& firstPoint, QVector2D const& secondPoint, qreal smoothness){
    if(!segments.isEmpty()){
        qWarning() << "PolyBezier::start(): Called on a nonempty curve, clearing.";
        segments.clear();
    }

    segments.push_back(
        CubicBezier(
            firstPoint,
            (1 - smoothness)*firstPoint     + smoothness*secondPoint,
            (1 - smoothness)*secondPoint    + smoothness*firstPoint,
            secondPoint,
            false)
        );

    invalidateCalc();
    emit controlPointsChanged();
}

void PolyBezier::appendPoint(QVector2D const& targetPoint, qreal entrySmoothness, qreal exitSmoothness){
    if(segments.isEmpty()){
        qWarning() << "PolyBezier::appendPoint(): Called on an empty curve, doing nothing.";
        return;
    }

    QVector2D lastPoint = segments.last().getControlPoint(3);

    //Maximum entry smoothness (C1 continuous) implies equal derivative in previous segment's exit and this segment's
    //entry i.e vector connecting last two control points of the previous segment must be equal to the one connecting
    //the first two control points of this segment. Lower smoothness ensures the same derivative direction (G1
    //continuous) but takes lower magnitude to get the new segment closer to a line:
    QVector2D controlPoint1 = lastPoint + entrySmoothness*(lastPoint - segments.last().getControlPoint(2));

    //We take the second control point on the line segment connecting the first control point to the target point,
    //thus ensuring shortest segment length (by not being outside the triangle made by the starting point, first control
    //point and target point) while having no inflection point (by not being inside the aforementioned triangle),
    //i.e no turning direction change while traveling on the curve. Maximum smoothness makes the second control point
    //equal to the first, minimizing the sharpness. Lower smoothness ensures shorter curve length and quicker arrival
    //to the target:
    QVector2D controlPoint2 = exitSmoothness*controlPoint1 + (1 - exitSmoothness)*targetPoint;

    segments.push_back(CubicBezier(lastPoint, controlPoint1, controlPoint2, targetPoint, false));

    invalidateCalc();
    emit controlPointsChanged();
}

int PolyBezier::getSegmentIndex(qreal& t){
    if(segments.isEmpty()){
        qCritical() << "PolyBezier::getSegmentIndex(): Curve empty, returning invalid index.";
        return -1;
    }
    else if(t >= segments.size()){
        t = 1;
        return segments.size() - 1;
    }
    else if(t < 0){
        t = 0;
        return 0;
    }
    else{
        int i = (int)t;
        t = t - i;
        return i;
    }
}

qreal PolyBezier::getArcLength(){
    calculateCumulativeArcLengths();
    return cumulativeArcLengths.last();
}

qreal PolyBezier::getMaxCurvature(){
    calculateMaxCurvature();
    return maxKappa;
}

qreal PolyBezier::getTByArcLengthRatio(qreal r){
    if(r >= 1.0)
        return segments.size();
    else if(r <= 0.0)
        return 0.0;

    //Calculate cumulative arc lengths of all segments
    calculateCumulativeArcLengths();

    if(cumulativeArcLengths.size() <= 1){
        qCritical() << "PolyBezier::getTByArcLengthRatio(): No segments present, invalid result will be returned!";
        return 0;
    }

    //Binary search over cumulative arc lengths of segments
    //Cache last found index and check it, as well as one next to it before search
    qreal rReal = r*cumulativeArcLengths.last();
    int beginIndex;
    if(lastLookupBeginIndex < segments.size() &&
        cumulativeArcLengths[lastLookupBeginIndex + 0] < rReal && rReal < cumulativeArcLengths[lastLookupBeginIndex + 1])
            beginIndex = lastLookupBeginIndex;
    else if(lastLookupBeginIndex + 1 < segments.size() &&
        cumulativeArcLengths[lastLookupBeginIndex + 1] < rReal && rReal < cumulativeArcLengths[lastLookupBeginIndex + 2])
            beginIndex = lastLookupBeginIndex + 1;
    else{
        beginIndex = 0;
        int endIndex = cumulativeArcLengths.size() - 1;
        while(beginIndex + 1 < endIndex){
            int midIndex = (beginIndex + endIndex)/2;
            if(cumulativeArcLengths[midIndex] < rReal)
                beginIndex = midIndex;
            else
                endIndex = midIndex;
        }
    }
    lastLookupBeginIndex = beginIndex;

    return beginIndex + segments[beginIndex].getTByArcLengthRatio((rReal - cumulativeArcLengths[beginIndex])/segments[beginIndex].getArcLength());
}

qreal PolyBezier::getArcLengthRatioByT(qreal t){

    //Calculate cumulative arc lengths of all segments
    calculateCumulativeArcLengths();

    if(segments.isEmpty()){
        qCritical() << "PolyBezier::getArcLengthRatioByT(): Curve empty, returning invalid t.";
        return 0.0;
    }
    else{
        int segIndex = getSegmentIndex(t); //Pulls t into local [0,1]
        CubicBezier& segment = segments[segIndex];
        return (cumulativeArcLengths[segIndex] + segment.getArcLength()*segment.getArcLengthRatioByT(t))/getArcLength();
    }
}

qreal PolyBezier::getCurvatureByArcLengthRatio(qreal r){
    if(r > 1.0)
        r = 1.0;
    else if(r < 0.0)
        r = 0.0;

    //Calculate cumulative arc lengths of all segments
    calculateCumulativeArcLengths();

    if(cumulativeArcLengths.size() <= 1){
        qCritical() << "PolyBezier::getCurvatureByArcLengthRatio(): No segments present, invalid result will be returned!";
        return 0;
    }

    //Binary search over cumulative arc lengths of segments
    //Cache last found index and check it, as well as one next to it before search
    qreal rReal = r*cumulativeArcLengths.last();
    int beginIndex;
    if(lastLookupBeginIndex < segments.size() &&
        cumulativeArcLengths[lastLookupBeginIndex + 0] < rReal && rReal < cumulativeArcLengths[lastLookupBeginIndex + 1])
            beginIndex = lastLookupBeginIndex;
    else if(lastLookupBeginIndex + 1 < segments.size() &&
        cumulativeArcLengths[lastLookupBeginIndex + 1] < rReal && rReal < cumulativeArcLengths[lastLookupBeginIndex + 2])
            beginIndex = lastLookupBeginIndex + 1;
    else{
        beginIndex = 0;
        int endIndex = cumulativeArcLengths.size() - 1;
        while(beginIndex + 1 < endIndex){
            int midIndex = (beginIndex + endIndex)/2;
            if(cumulativeArcLengths[midIndex] < rReal)
                beginIndex = midIndex;
            else
                endIndex = midIndex;
        }
    }
    lastLookupBeginIndex = beginIndex;
    return segments[beginIndex].getCurvatureByArcLengthRatio((rReal - cumulativeArcLengths[beginIndex])/segments[beginIndex].getArcLength());
}

qreal PolyBezier::getClosest(const QVector2D& m, QVector2D& closestPoint, qreal& closestDist){
    if(segments.isEmpty()){
        qCritical() << "PolyBezier::getClosest(): Curve empty, returning invalid t.";
        return 0;
    }
    else{
        qreal dist, t, closestT = 0;
        QVector2D point;
        closestDist = std::numeric_limits<qreal>::max();
        int closestSegment = 0;
        for(int i=0;i<segments.size();i++){
            CubicBezier& segment = segments[i];

            //First, check distance to bounding box, if this rough distance is not good enough, actual curve distance won't be
            if(segment.getDistToBoundingBox(m) < closestDist){
                t = segment.getClosest(m, point, dist);
                if(dist < closestDist){
                    closestPoint = point;
                    closestDist = dist;
                    closestT = t;
                    closestSegment = i;
                }
            }
        }
        return closestT + closestSegment;
    }
}

qreal PolyBezier::getClosestT(QVector2D const& point){
    QVector2D dummyV;
    qreal dummyD;
    return getClosest(point, dummyV, dummyD);
}

QVector2D PolyBezier::getPoint(qreal t){
    if(segments.isEmpty()){
        qCritical() << "PolyBezier::getPoint(): Curve empty, returning invalid point.";
        return QVector2D(0,0);
    }
    else{
        int i = getSegmentIndex(t);
        return segments[i].getPoint(t);
    }
}

QVector2D PolyBezier::getTangent(qreal t){
    if(segments.isEmpty()){
        qCritical() << "PolyBezier::getTangent(): Curve empty, returning invalid tangent.";
        return QVector2D(0,0);
    }
    else{
        int i = getSegmentIndex(t);
        return segments[i].getDerivative(t);
    }
}

QVector2D PolyBezier::getNormal(qreal t){
    QVector2D tangent = getTangent(t);
    return QVector2D(-tangent.y(), tangent.x());
}

bool PolyBezier::isInside(QVector2D const& point){

    //Check if point is outside the bounding box
    if(!isInBoundingBox(point))
        return 0.0f;

    //Number of crossings on the Bézier segments
    int numCrossings = 0;
    for(auto segment : segments)
        numCrossings += segment.getNumCrossings(point);

    //Check crossing with line segment that closes the curve
    if(!segments.isEmpty())
        if(CelluloMathUtil::hRayCrossesLineSeg(point, segments.first().getControlPoint(0), segments.last().getControlPoint(3)))
            numCrossings++;

    return numCrossings % 2 == 1;
}

void PolyBezier::calculateBoundingBox(){
    if(boundingBoxCalculated)
        return;

    minX = std::numeric_limits<qreal>::max();
    maxX = std::numeric_limits<qreal>::min();
    minY = std::numeric_limits<qreal>::max();
    maxY = std::numeric_limits<qreal>::min();

    qreal minXSeg, maxXSeg, minYSeg, maxYSeg;
    for(auto segment : segments){
        segment.getBoundingBox(minXSeg, maxXSeg, minYSeg, maxYSeg);
        if(minXSeg < minX)
            minX = minXSeg;
        if(maxXSeg > maxX)
            maxX = maxXSeg;
        if(minYSeg < minY)
            minY = minYSeg;
        if(maxYSeg > maxY)
            maxY = maxYSeg;
    }

    boundingBoxCalculated = true;
}

bool PolyBezier::isInBoundingBox(QVector2D const& point){
    calculateBoundingBox();
    return minX <= point.x() && point.x() <= maxX && minY <= point.y() && point.y() <= maxY;
}

void PolyBezier::calculateCumulativeArcLengths(){
    if(cumulativeArcLengthsCalculated)
        return;

    cumulativeArcLengths.clear();

    qreal cumulativeArcLength = 0;
    cumulativeArcLengths.push_back(0);
    for(CubicBezier& segment : segments){
        cumulativeArcLength += segment.getArcLength();
        cumulativeArcLengths.push_back(cumulativeArcLength);
    }

    cumulativeArcLengthsCalculated = true;
}

void PolyBezier::calculateMaxCurvature(){
    if(maxKappaCalculated)
        return;

    maxKappa = 0;
    for(CubicBezier& segment : segments){
        qreal localMaxKappa = segment.getMaxCurvature();
        if(localMaxKappa > maxKappa)
            maxKappa = localMaxKappa;
    }

    maxKappaCalculated = true;
}

void PolyBezier::invalidateCalc(){
    boundingBoxCalculated = false;
    cumulativeArcLengthsCalculated = false;
    maxKappaCalculated = false;
    lastLookupBeginIndex = 0;
}

}
