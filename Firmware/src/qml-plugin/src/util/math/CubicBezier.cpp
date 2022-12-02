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
 * @file CubicBezier.cpp
 * @brief Source for a cubic Bézier curve
 * @author Ayberk Özgür
 * @date 2016-07-21
 */

#include "CubicBezier.h"

#include "CelluloMathUtil.h"

#include <cmath>

#include <QStack>
#include <QPair>
#include <QDebug>

namespace Cellulo{

CubicBezier::CubicBezier(){
    setControlPoints(QVector2D(0,0), QVector2D(0,1), QVector2D(0,2), QVector2D(0,3));
}

CubicBezier::CubicBezier(const QVector2D& p0, const QVector2D& p1, const QVector2D& p2, const QVector2D& p3, bool calculateNow){
    setControlPoints(p0, p1, p2, p3);
    if(calculateNow){
        calculateLUTs();
        calculateBoundingBox();
    }
}

CubicBezier& CubicBezier::dumbClone(){
    CubicBezier* newCurve = new CubicBezier();
    newCurve->setControlPoints(p[0], p[1], p[2], p[3]);
    return *newCurve;
}

void CubicBezier::setControlPoints(const QVector2D& p0, const QVector2D& p1, const QVector2D& p2, const QVector2D& p3){
    p[0] = p0;
    p[1] = p1;
    p[2] = p2;
    p[3] = p3;
    isDegenerate = CelluloMathUtil::collinear(p0, p1, p3) && CelluloMathUtil::collinear(p0, p2, p3);

    invalidateCalc();
}

QVector2D CubicBezier::getControlPoint(unsigned char i) const {
    if(i > 3)
        return QVector2D();
    else
        return p[i];
}

qreal CubicBezier::getArcLength(){
    calculateLUTs();
    return arcLength;
}

qreal CubicBezier::getTByArcLengthRatio(qreal r){
    if(r > 1)
        return 1;
    else if(r < 0)
        return 0;

    //Calculate equidistant t/point lookup table if not already calculated
    calculateLUTs();

    qreal realIndex = r*(tLUT.size() - 1);
    int prevIndex = std::floor(realIndex);
    int nextIndex = std::ceil(realIndex);
    if(prevIndex == nextIndex)
        return tLUT[prevIndex];
    else
        return (realIndex - prevIndex)*tLUT[nextIndex] + (nextIndex - realIndex)*tLUT[prevIndex];
}

qreal CubicBezier::getArcLengthRatioByT(qreal t){
    if(t >= 1)
        return 1;
    else if(t <= 0)
        return 0;

    //Calculate equidistant t/point lookup table if not already calculated
    calculateLUTs();

    //Binary search over t LUT
    int beginIndex = 0;
    int endIndex = tLUT.size() - 1;
    while(beginIndex + 1 < endIndex){
        int midIndex = (beginIndex + endIndex)/2;
        if(tLUT[midIndex] < t)
            beginIndex = midIndex;
        else
            endIndex = midIndex;
    }

    if(beginIndex == endIndex)
        return ((qreal)beginIndex)/((qreal)(tLUT.size() - 1));
    else{
        qreal tBegin = tLUT[beginIndex];
        qreal tEnd = tLUT[endIndex];
        return (((t - tBegin)*((qreal)endIndex) + (tEnd - t)*((qreal)beginIndex))/(tEnd - tBegin))/((qreal)(tLUT.size() - 1));
    }
}

qreal CubicBezier::getClosest(const QVector2D& m, QVector2D& closestPoint, qreal& closestDist){

    //Calculate equidistant t/point lookup table if not already calculated
    calculateLUTs();

    qreal closestT = 0;
    closestDist = std::numeric_limits<qreal>::max();

    //Find closest point in lookup table
    for(int i=0; i<tLUT.size(); i++){
        qreal currentDist = pointLUT[i].distanceToPoint(m);
        if(currentDist < closestDist){
            closestT = tLUT[i];
            closestDist = currentDist;
            closestPoint = pointLUT[i];
        }
    }

    //Check interval around for a closer point
    qreal intervalSizeEpsilon = CLOSEST_T_SEARCH_EPSILON/arcLength;
    qreal currentIntervalSize = 0.5*(1.0/((qreal)(tLUT.size() - 1)));
    while(currentIntervalSize > intervalSizeEpsilon){
        qreal leftT = closestT - currentIntervalSize;
        qreal rightT = closestT + currentIntervalSize;
        QVector2D leftPoint = leftT >= 0 ? getPoint(leftT) : QVector2D();
        QVector2D rightPoint = rightT <= 1 ? getPoint(rightT) : QVector2D();
        qreal leftDist = leftT >= 0 ? leftPoint.distanceToPoint(m) : std::numeric_limits<qreal>::max();
        qreal rightDist = rightT <= 1 ? rightPoint.distanceToPoint(m) : std::numeric_limits<qreal>::max();

        if(leftDist < closestDist && leftDist < rightDist){
            closestT = leftT;
            closestDist = leftDist;
            closestPoint = leftPoint;
        }
        else if(rightDist < closestDist && rightDist < leftDist){
            closestT = rightT;
            closestDist = rightDist;
            closestPoint = rightPoint;
        }

        currentIntervalSize /= 2;
    }

    return closestT;
}

QVector2D CubicBezier::getPoint(qreal t){
    if(t == 0)
        return p[0];
    else if(t == 1)
        return p[3];
    else{
        qreal one_minus_t = 1 - t;
        qreal one_minus_t_squared = one_minus_t*one_minus_t;
        qreal t_squared = t*t;
        return one_minus_t_squared*(one_minus_t*p[0] + 3*t*p[1]) + t_squared*(3*one_minus_t*p[2] + t*p[3]);
    }
}

qreal CubicBezier::getPointX(qreal t){
    if(t == 0)
        return p[0].x();
    else if(t == 1)
        return p[3].x();
    else{
        qreal one_minus_t = 1 - t;
        qreal one_minus_t_squared = one_minus_t*one_minus_t;
        qreal t_squared = t*t;
        return one_minus_t_squared*(one_minus_t*p[0].x() + 3*t*p[1].x()) + t_squared*(3*one_minus_t*p[2].x() + t*p[3].x());
    }
}

qreal CubicBezier::getPointY(qreal t){
    if(t == 0)
        return p[0].y();
    else if(t == 1)
        return p[3].y();
    else{
        qreal one_minus_t = 1 - t;
        qreal one_minus_t_squared = one_minus_t*one_minus_t;
        qreal t_squared = t*t;
        return one_minus_t_squared*(one_minus_t*p[0].y() + 3*t*p[1].y()) + t_squared*(3*one_minus_t*p[2].y() + t*p[3].y());
    }
}

QVector2D CubicBezier::getDerivative(qreal t){
    if(t == 0)
        return 3*(p[1] - p[0]);
    else if(t == 1)
        return 3*(p[3] - p[2]);
    else{
        qreal one_minus_t = 1 - t;
        return 3*(one_minus_t*one_minus_t*(p[1] - p[0]) + 2*one_minus_t*t*(p[2] - p[1]) + t*t*(p[3] - p[2]));
    }
}

QVector2D CubicBezier::getSecondDerivative(qreal t){
    if(t == 0)
        return 6*(p[2] - 2*p[1] + p[0]);
    else if(t == 1)
        return 6*(p[3] - 2*p[2] + p[1]);
    else
        return 6*((1 - t)*(p[2] - 2*p[1] + p[0]) + t*(p[3] - 2*p[2] + p[1]));
}

qreal CubicBezier::getCurvatureByArcLengthRatio(qreal r){
    if(r < 0)
        return kappaLUT.first();
    else if(r > 1)
        return kappaLUT.last();

    //Calculate equidistant kappa lookup table if not already calculated
    calculateKappaLUT();

    qreal realIndex = r*(kappaLUT.size() - 1);
    int prevIndex = std::floor(realIndex);
    int nextIndex = std::ceil(realIndex);
    if(prevIndex == nextIndex)
        return kappaLUT[prevIndex];
    else
        return (realIndex - prevIndex)*kappaLUT[nextIndex] + (nextIndex - realIndex)*kappaLUT[prevIndex];
}

qreal CubicBezier::getMaxCurvature(){
    calculateKappaLUT();
    return maxKappa;
}

bool CubicBezier::side(const QVector2D& m){

    //Get the closest point on the curve
    qreal dummy;
    QVector2D curvePoint;
    qreal curveT = getClosest(m, curvePoint, dummy);

    //Get the direction vector of the tangent line to the curve on the closest point
    QVector2D curveDirection = getDerivative(curveT);

    //Get the direction vector from the closest point to the given point
    QVector2D pointDirection = m - curvePoint;

    //curveDirection and pointDirection are orthogonal by definition,
    //find whether pointDirection is located clockwise or counterclockwise with respect to curveDirection via cross product
    return curveDirection.x()*pointDirection.y() > curveDirection.y()*pointDirection.x();
}

void CubicBezier::split(qreal t, CubicBezier& left, CubicBezier& right){
    QVector2D p01 = (1 - t)*p[0] + t*p[1];
    QVector2D p12 = (1 - t)*p[1] + t*p[2];
    QVector2D p23 = (1 - t)*p[2] + t*p[3];
    QVector2D p0112 = (1 - t)*p01 + t*p12;
    QVector2D p1223 = (1 - t)*p12 + t*p23;
    QVector2D p01121223 = (1 - t)*p0112 + t*p1223;
    left.setControlPoints(p[0], p01, p0112, p01121223);
    right.setControlPoints(p01121223, p1223, p23, p[3]);
}

void CubicBezier::invalidateCalc(){
    boundingBoxCalculated = false;
    LUTsCalculated = false;
    kappaLUTCalculated = false;
}

void CubicBezier::calculateLUTs(){
    if(LUTsCalculated)
        return;

    arcLength = 0;

    //First pass, build temporary LUT of segments whose lengths are max. ARC_LENGTH_FIRST_PASS_EPSILON
    QList<qreal> tLUTFirstPass;
    QList<qreal> segLengthsFirstPass;
    QStack<QPair<qreal,qreal>> segStack;
    segStack.push(QPair<qreal,qreal>(0.0,1.0));
    while(!segStack.isEmpty()){
        auto currentSeg = segStack.pop();
        qreal begT = currentSeg.first;
        qreal endT = currentSeg.second;
        qreal midT = 0.5*(begT + endT);
        qreal segLength = getPoint(begT).distanceToPoint(getPoint(endT));
        if(segLength < ARC_LENGTH_FIRST_PASS_EPSILON){
            arcLength += segLength;
            segLengthsFirstPass.push_back(segLength);
            tLUTFirstPass.push_back(endT);
        }
        else{
            segStack.push(QPair<qreal,qreal>(midT, endT));
            segStack.push(QPair<qreal,qreal>(begT, midT));
        }
    }

    //Second pass, build final LUT of segments whose lengths are approximately equal and around ARC_LENGTH_EPSILON
    tLUT.clear();
    qreal currentT = 0.0;
    qreal currentL = 0.0;
    tLUT.push_back(currentT);
    auto t = tLUTFirstPass.begin();
    auto l = segLengthsFirstPass.begin();
    for(; t != tLUTFirstPass.end(); t++, l++){
        currentT = *t;
        currentL += *l;

        //Decide to add segment
        if(currentL >= ARC_LENGTH_EPSILON){
            tLUT.push_back(currentT);
            currentL = 0;
        }
    }

    //Check if segment is too short to benefit from first pass
    if(tLUT.size() == 1)
        tLUT.push_back(1.0);

    //Final pass, "stretch" all t's so that the final t ends up at 1.0, also calculate final point LUT
    qreal extraT = (1.0 - tLUT.last())/(tLUT.size() - 1);
    auto tFinal = tLUT.begin();
    qreal inc = 0;
    for(; tFinal != tLUT.end(); tFinal++){
        *tFinal += inc;
        inc += extraT;
        pointLUT.push_back(getPoint(*tFinal));
    }

    LUTsCalculated = true;
}

void CubicBezier::calculateKappaLUT(){
    if(kappaLUTCalculated)
        return;

    calculateLUTs();

    maxKappa = 0;

    kappaLUT.clear();
    for(qreal t : tLUT){
        QVector2D pprime = getDerivative(t);
        QVector2D pprimeprime = getSecondDerivative(t);
        qreal pprimelen = pprime.length();
        qreal kappa;
        if(pprimelen > 0)
            kappa = std::abs(pprime.x()*pprimeprime.y() - pprime.y()*pprimeprime.x())/(pprimelen*pprimelen*pprimelen);
        else
            kappa = 0.0;
        kappaLUT.push_back(kappa);
        if(kappa > maxKappa)
            maxKappa = kappa;
    }

    kappaLUTCalculated = true;
}

inline void CubicBezier::updateMinMaxX(qreal newX){
    if(newX < minX)
        minX = newX;
    if(newX > maxX)
        maxX = newX;
}

inline void CubicBezier::updateMinMaxY(qreal newY){
    if(newY < minY)
        minY = newY;
    if(newY > maxY)
        maxY = newY;
}

void CubicBezier::calculateBoundingBox(){
    if(boundingBoxCalculated)
        return;

    minX = std::numeric_limits<qreal>::max();
    maxX = std::numeric_limits<qreal>::min();
    minY = std::numeric_limits<qreal>::max();
    maxY = std::numeric_limits<qreal>::min();

    QVector2D dBdt_a = -p[0] + 3*p[1] - 3*p[2] + p[3];
    QVector2D dBdt_b = 2*p[0] - 4*p[1] + 2*p[2];
    QVector2D dBdt_c = -p[0] + p[1];

    qreal t1, t2; //Roots of dB(t)/dt = 0

    //Solve x
    if(CelluloMathUtil::solveQuadEq(dBdt_a.x(), dBdt_b.x(), dBdt_c.x(), t1, t2) > 0){
        if(0 <= t1 && t1 <= 1)
            updateMinMaxX(getPointX(t1));
        if(0 <= t2 && t2 <= 1)
            updateMinMaxX(getPointX(t2));
    }
    updateMinMaxX(p[0].x());
    updateMinMaxX(p[3].x());

    //Solve y
    if(CelluloMathUtil::solveQuadEq(dBdt_a.y(), dBdt_b.y(), dBdt_c.y(), t1, t2) > 0){
        if(0 <= t1 && t1 <= 1)
            updateMinMaxY(getPointY(t1));
        if(0 <= t2 && t2 <= 1)
            updateMinMaxY(getPointY(t2));
    }
    updateMinMaxY(p[0].y());
    updateMinMaxY(p[3].y());

    boundingBoxCalculated = true;
}

void CubicBezier::getBoundingBox(qreal& minXOut, qreal& maxXOut, qreal& minYOut, qreal& maxYOut){
    calculateBoundingBox();
    minXOut = minX;
    maxXOut = maxX;
    minYOut = minY;
    maxYOut = maxY;
}

bool CubicBezier::inBoundingBox(const QVector2D& m){
    calculateBoundingBox();
    return minX <= m.x() && m.x() <= maxX && minY <= m.y() && m.y() <= maxY;
}

qreal CubicBezier::getDistToBoundingBox(const QVector2D& m){
    calculateBoundingBox();

    //Taken from http://gamedev.stackexchange.com/a/44496
    qreal dx = fmax(fabs(m.x() - (minX + maxX)/2) - (maxX - minX)/2, 0);
    qreal dy = fmax(fabs(m.y() - (minY + maxY)/2) - (maxY - minY)/2, 0);
    return sqrt(dx*dx + dy*dy);
}

int CubicBezier::getNumCrossings(const QVector2D& m){
    if(isDegenerate)
        return CelluloMathUtil::hRayCrossesLineSeg(m, p[0], p[3]) ? 1 : 0;
    else{
        int numCrossings = 0;

        //Translate curve to m as origin
        qreal Ay = p[0].y() - m.y();
        qreal By = p[1].y() - m.y();
        qreal Cy = p[2].y() - m.y();
        qreal Dy = p[3].y() - m.y();

        //Solve B(t).y = 0 to find points where the horizontal ray is crossed
        qreal t1, t2, t3;
        int numRoots = CelluloMathUtil::solveCubicEq(-Ay + 3*By - 3*Cy + Dy, 3*Ay - 6*By + 3*Cy, -3*Ay + 3*By, Ay, t1, t2, t3);

        //One root, check t1 only
        if(0 <= t1 && t1 <= 1)
            if(getPointX(t1) - m.x() > 0) //To the right of m
                numCrossings++;

        //Two roots, check t2 too
        if(numRoots >= 2)
            if(0 <= t2 && t2 <= 1)
                if(getPointX(t2) - m.x() > 0) //To the right of m
                    numCrossings++;

        //Three roots, check t3 too
        if(numRoots >= 3)
            if(0 <= t3 && t3 <= 1)
                if(getPointX(t3) - m.x() > 0) //To the right of m
                    numCrossings++;

        return numCrossings;
    }
}

bool CubicBezier::boundingBoxesIntersect(CubicBezier& curve1, CubicBezier& curve2){

    //Calculate bounding boxes for both curves if not already calculated
    curve1.calculateBoundingBox();
    curve2.calculateBoundingBox();

    return !(curve1.maxX < curve2.minX || curve1.minX > curve2.maxX || curve1.maxY < curve2.minY || curve1.minY > curve2.maxY);
}

}
