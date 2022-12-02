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
 * @file CelluloMathUtil.cpp
 * @brief Header for mathematical and geometrical utilities
 * @author Joanna Salathé
 * @author Ayberk Özgür
 * @date 2016-06-30
 */

#include "CelluloMathUtil.h"

#include <algorithm>
#include <cstdlib>
#include <ctime>

#include <QtMath>

namespace Cellulo{

CelluloMathUtil::CelluloMathUtil(QObject* parent) : QObject(parent){
    c_srand(time(NULL));
}

CelluloMathUtil::~CelluloMathUtil(){}

qreal CelluloMathUtil::pointToSegmentDist(const QVector2D& p, const QVector2D& seg1, const QVector2D& seg2){
    //Taken from http://stackoverflow.com/questions/849211/shortest-distance-between-a-point-and-a-line-segment

    const qreal segLenSquared = (seg1 - seg2).lengthSquared();
    if(segLenSquared == 0.0)
        return seg1.distanceToPoint(p);

    //Consider the line extending the segment, parameterized as v + t (w - v).
    //We find projection of point p onto the line.
    //It falls where t = [(p-v) . (w-v)] / |w-v|^2
    //We clamp t from [0,1] to handle points outside the segment vw.
    const qreal t = fmax(0, fmin(1, QVector2D::dotProduct(p - seg1, seg2 - seg1)/segLenSquared));
    const QVector2D projection = seg1 + t*(seg2 - seg1); //Projection falls on the segment
    return projection.distanceToPoint(p);
}

qreal CelluloMathUtil::pointToLineDistSigned(const QVector2D& p, const QVector2D& lp1, const QVector2D& lp2){
    //Taken from https://en.wikipedia.org/wiki/Distance_from_a_point_to_a_line

    QVector2D diff = lp2 - lp1;
    return ((diff.y()*p.x() - diff.x()*p.y()) + (lp2.x()*lp1.y() - lp2.y()*lp1.x()))/diff.length();
}

bool CelluloMathUtil::pointAboveLine(const QVector2D& p, const QVector2D& lp1, const QVector2D& lp2){
    QVector2D diff = lp2 - lp1;
    return ((diff.y()*p.x() - diff.x()*p.y()) + (lp2.x()*lp1.y() - lp2.y()*lp1.x())) > 0;
}

bool CelluloMathUtil::pointInPoly(const QVector2D& p, const QList<QVector2D>& v){
    //Taken from: https://www.ecse.rpi.edu/Homepages/wrf/Research/Short_Notes/pnpoly.html

    bool result = false;
    for(int i = 0, j = v.size() - 1; i < v.size(); j = i++){
        if((v[i].y() > p.y()) != (v[j].y() > p.y()) &&
           (p.x() < (v[j].x() - v[i].x())*(p.y() - v[i].y())/(v[j].y() - v[i].y()) + v[i].x()))
            result = !result;
    }
    return result;
}

qreal CelluloMathUtil::pointToPolyBorderDist(const QVector2D& p, const QList<QVector2D>& v){
    qreal minDist = std::numeric_limits<qreal>::max();
    qreal tempDist;
    for(int i=0;i<v.size()-1;i++){
        tempDist = pointToSegmentDist(p, v[i], v[i + 1]);
        if(tempDist < minDist)
            minDist = tempDist;
    }
    tempDist = pointToSegmentDist(p, v[v.size() - 1], v[0]);
    if(tempDist < minDist)
        minDist = tempDist;
    return minDist;
}

int CelluloMathUtil::solveLinearEq(qreal a, qreal b, qreal& x){

    //Treat constant line as having zero root even if b = 0
    if(a == 0)
        return 0;
    else{
        x = -b/a;
        return 1;
    }
}

int CelluloMathUtil::solveQuadEq(qreal a, qreal b, qreal c, qreal& x1, qreal& x2){
    //Taken from http://read.pudn.com/downloads21/sourcecode/graph/71499/gems/Roots3And4.c__.htm

    if(isZero(a, SOLVE_QUAD_EPSILON)){
        int numRoots = solveLinearEq(b, c, x1);
        x2 = x1;
        return numRoots;
    }

    //Normal form: x^2 + px + q = 0
    qreal p = b/(2*a);
    qreal q = c/a;
    qreal D = p*p - q;

    if(isZero(D, SOLVE_CUBIC_EPSILON)){
        x1 = -p;
        x2 = x1;
        return 1;
    }
    else if(D < 0)
        return 0;
    else{
        qreal sqrt_D = sqrtf(D);
        x1 = sqrt_D - p;
        x2 = -sqrt_D - p;
        return 2;
    }
}

inline bool CelluloMathUtil::isZero(qreal val, qreal epsilon){
    return -epsilon <= val && val <= epsilon;
}

int CelluloMathUtil::solveCubicEq(qreal a, qreal b, qreal c, qreal d, qreal& x1, qreal& x2, qreal& x3){
    //Taken from http://read.pudn.com/downloads21/sourcecode/graph/71499/gems/Roots3And4.c__.htm

    //Most significant coeff is zero, treat as a quadratic equation
    if(isZero(a, SOLVE_CUBIC_EPSILON)){
        int numRoots = solveQuadEq(b, c, d, x1, x2);
        x3 = x2;
        return numRoots;
    }

    //Normal form: x^3 + Ax^2 + Bx + C = 0
    qreal A = b/a;
    qreal B = c/a;
    qreal C = d/a;

    //Substitute x = y - A/3 to eliminate quadric term: x^3 +px + q = 0 */
    qreal Asquared = A*A;
    qreal p = (-Asquared/3 + B)/3;
    qreal q = (2*A*Asquared/27 - A*B/3 + C)/2;

    //Use Cardano's formula
    qreal pcubed = p*p*p;
    qreal D = q*q + pcubed;

    qreal sub = A/3;

    if(isZero(D, SOLVE_CUBIC_EPSILON)){

        //One triple solution
        if(isZero(q, SOLVE_CUBIC_EPSILON)){
            x1 = -sub;
            x2 = x1;
            x3 = x1;
            return 1;
        }

        //One single and one double solution
        else{
            qreal u = cbrt(-q);
            x1 = 2*u - sub;
            x2 = -u - sub;
            x3 = x2;
            return 2;
        }
    }

    //Casus irreducibilis: three real solutions
    else if(D < 0){
        qreal phi = acos(-q/sqrt(-pcubed))/3;
        qreal t = 2*sqrt(-p);
        x1 =  t*cos(phi) - sub;
        x2 = -t*cos(phi + M_PI/3) - sub;
        x3 = -t*cos(phi - M_PI/3) - sub;
        return 3;
    }

    //One real solution
    else{
        qreal sqrt_D = sqrt(D);

        x1 = cbrt(sqrt_D - q) - cbrt(sqrt_D + q) - sub;
        x2 = x1;
        x3 = x1;
        return 1;
    }
}

bool CelluloMathUtil::hRayCrossesLineSeg(const QVector2D& r, const QVector2D& seg1, const QVector2D& seg2){

    //Segment is degenerate
    if(seg1.y() == seg2.y())
        return false;

    //Ray origin is to the right, bottom or top of the segment
    if(
        (r.x() > seg1.x() && r.x() > seg2.x()) ||
        (r.y() > seg1.y() && r.y() > seg2.y()) ||
        (r.y() < seg1.y() && r.y() < seg2.y())
    )
        return false;

    //Ray origin is to the left of the segment, definitely crosses
    if(r.x() < seg1.x() && r.x() < seg2.x())
        return true;

    //Ray origin is within the bounding box of the segment at this point, find intersection of ray
    qreal q = (r.y() - seg2.y())/(seg1.y() - seg2.y());
    return r.x() < q*seg1.x() + (1 - q)*seg2.x();
}

bool CelluloMathUtil::rayCrossesLineSeg(QVector2D const& origin, QVector2D const& dir, QVector2D const& seg1, QVector2D const& seg2){

    /* https://rootllama.wordpress.com/2014/06/20/ray-line-segment-intersection-test-in-2d/ */

    QVector2D v1 = origin - seg1;
    QVector2D v2 = seg2 - seg1;
    QVector2D v3 = QVector2D(-dir.y(), dir.x()).normalized();

    qreal denom = QVector2D::dotProduct(v2, v3);

    //Check whether segment is degenerate, direction is degenerate or segment and direction are parallel
    if(denom == 0.0)
        return false;

    //Intersection parameter on the ray
    qreal t1 = crossProduct(v2, v1)/denom;
    if(t1 < 0.0)
        return false;

    //Intersection parameter on the segment
    qreal t2 = QVector2D::dotProduct(v1, v3)/denom;
    return 0.0 <= t2 && t2 <= 1.0;
}

bool CelluloMathUtil::collinear(const QVector2D& a, const QVector2D& b, const QVector2D& c){
    return fabsf(a.x()*(b.y() - c.y()) + b.x()*(c.y() - a.y()) + c.x()*(a.y() - b.y())) < COLLINEAR_EPSILON;
}

void CelluloMathUtil::c_srand(unsigned int seed){
    srand(seed);
}

int CelluloMathUtil::c_rand(unsigned int max){
    return rand() % max;
}

QVariantList CelluloMathUtil::shuffle(const QVariantList& list){
    QVariantList shuffled = list;
    std::random_shuffle(shuffled.begin(), shuffled.end());
    return shuffled;
}

QVector2D CelluloMathUtil::rotateVector(QVector2D const& vector, qreal angleRad){
    return QVector2D(qCos(angleRad)*vector.x() - qSin(angleRad)*vector.y(), qSin(angleRad)*vector.x() + qCos(angleRad)*vector.y());
}

qreal CelluloMathUtil::sigmoid(qreal L, qreal beta, qreal r_0, qreal r){
    return L/(1 + qExp(beta*(r - r_0)));
}

qreal CelluloMathUtil::clamp(qreal val, qreal min, qreal max){
    if(val < min)
        return min;
    else if(val > max)
        return max;
    else
        return val;
}

qreal CelluloMathUtil::angleBetween(QVector2D const& vec1, QVector2D const& vec2){
    qreal lens = vec1.length()*vec2.length();
    if(lens > 0)
        return qAcos(QVector2D::dotProduct(vec1, vec2)/lens);
    else
        return 0;
}

qreal CelluloMathUtil::crossProduct(QVector2D const& vec1, QVector2D const& vec2){
    return vec1.x()*vec2.y() - vec1.y()*vec2.x();
}

}
