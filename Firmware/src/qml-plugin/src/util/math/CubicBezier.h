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
 * @file CubicBezier.h
 * @brief Header for a cubic Bézier curve
 * @author Ayberk Özgür
 * @date 2016-07-21
 */

#ifndef CUBICBEZIER_H
#define CUBICBEZIER_H

#include <QVector2D>
#include <QList>
#include <QVector>

namespace Cellulo{

/** @cond DO_NOT_DOCUMENT */

/**
 * @brief Describes a cubic Bézier curve, B(t) = (1 - t)^3*p0 + 3*(1 - t)^2*t*p1 + 3*(1 - t)*t^2*p2 + t^3*p3
 */
class CubicBezier {

public:

    /**
     * @brief Creates a new cubic Bézier curve with provided control points, calculates the lookup tables
     *
     * @param p0 First control point
     * @param p1 Second control point
     * @param p2 Third control point
     * @param p3 Fourth control point
     * @param calculateNow Whether to calculate the fast lookup tables and bounding box immediately
     */
    CubicBezier(const QVector2D &p0, const QVector2D &p1, const QVector2D &p2, const QVector2D &p3, bool calculateNow = true);

    /**
     * @brief Creates a new cubic Bézier curve with undefined control points
     */
    CubicBezier();

    /**
     * @brief Creates a clone of this with just the control points with all lazy calculations invalidated
     *
     * @return New clone of this object, without the lazy calculations done
     */
    CubicBezier& dumbClone();

    /**
     * @brief Sets all control points
     *
     * @param p0 First control point
     * @param p1 Second control point
     * @param p2 Third control point
     * @param p3 Fourth control point
     */
    void setControlPoints(const QVector2D& p0, const QVector2D& p1, const QVector2D& p2, const QVector2D& p3);

    /**
     * @brief Gets a control point
     *
     * @param i Index: 0, 1, 2 or 3
     */
    QVector2D getControlPoint(unsigned char i) const;

    /**
     * @brief Gets the approximate arc length of this curve
     *
     * Arc length is defined as the sum of segments where each segment's length is guaranteed to be less than ARC_LENGTH_EPSILON
     *
     * @return Approximate arc length of the curve
     */
    qreal getArcLength();

    /**
     * @brief Gets the parameter (approximate) t corresponding to the desired arc length ratio
     *
     * Takes O(1) time after building LUT.
     *
     * @param  r Arc length ratio in [0,1] where 0 corresponds to the beginning, 0.5 corresponds to exactly halfway through the length of the curve etc.
     * @return   Parameter t corresponding to r
     */
    qreal getTByArcLengthRatio(qreal r);

    /**
     * @brief Gets the (approximate) arc length ratio corresponding to the parameter t
     *
     * Takes O(logN) time after building LUT where N is the LUT size.
     *
     * @param  t Parameter t
     * @return   Arc length ratio in [0,1] where 0 corresponds to the beginning, 0.5 corresponds to exactly halfway through the length of the curve etc.
     */
    qreal getArcLengthRatioByT(qreal t);

    /**
     * @brief Gets the closest point on the curve to the given point
     *
     * @param m Given point
     * @param closestPoint [out] Returns the closest point
     * @param closestDist [out] Returns the closest distance
     * @return Returns the parameter t corresponding to the closest point
     */
    qreal getClosest(const QVector2D& m, QVector2D& closestPoint, qreal& closestDist);

    /**
     * @brief Calculates the point on the curve corresponding to the given parameter
     *
     * @param t Curve parameter in [0,1]
     * @return Point corresponding to t
     */
    QVector2D getPoint(qreal t);

    /**
     * @brief Calculates the x coordinate of the point on the curve corresponding to the given parameter
     *
     * @param t Curve parameter in [0,1]
     * @return Point x coordinate corresponding to t
     */
    qreal getPointX(qreal t);

    /**
     * @brief Calculates the y coordinate of the point on the curve corresponding to the given parameter
     *
     * @param t Curve parameter in [0,1]
     * @return Point y coordinate corresponding to t
     */
    qreal getPointY(qreal t);

    /**
     * @brief Calculates the vector of the tangent line to the curve at the given parameter
     *
     * @param t Parameter
     * @return The direction vector of the tangent line to the curve
     */
    QVector2D getDerivative(qreal t);

    /**
     * @brief Calculates the rate and direction of change of the tangent af the given parameter
     *
     * @param t Parameter
     * @return The rate and direction of change of the tangent line to the curve
     */
    QVector2D getSecondDerivative(qreal t);

    /**
     * @brief Calculates the approximate absolute curvature (kappa) at given arc length ratio
     *
     * @param r Arc length ratio
     * @return The approximate absolute curvature (kappa) at given arc length ratio
     */
    qreal getCurvatureByArcLengthRatio(qreal r);

    /**
     * @brief Gets the approximate maximum absolute curvature (kappa) of the curve
     *
     * @return Approximate maximum curvature (kappa)
     */
    qreal getMaxCurvature();

    /**
     * @brief Calculates which side of the curve the given point is located
     *
     * @param m Given point
     * @return True if the point is on the right (clockwise) false if the point is on the left (counter-clockwise)
     */
    bool side(const QVector2D& m);

    /**
     * @brief Splits the curve into two parts which make up this curve
     *
     * @param t Split point, must be in [0,1]
     * @param left [out] First part
     * @param right [out] Second part
     */
    void split(qreal t, CubicBezier& left, CubicBezier& right);

    /**
     * @brief Gets the coordinates of the bounding box
     *
     * @param minXOut [out] Left coordinate
     * @param maxXOut [out] Right coordinate
     * @param minYOut [out] Top coordinate
     * @param maxYOut [out] Bottom coordinate
     */
    void getBoundingBox(qreal& minXOut, qreal& maxXOut, qreal& minYOut, qreal& maxYOut);

    /**
     * @brief Calculates whether the given point is in the bounding box of this curve
     *
     * @param m The point to check
     * @return Whether this point is in the bounding box
     */
    bool inBoundingBox(const QVector2D& m);

    /**
     * @brief Calculates the distance to the bounding box of the given point
     *
     * @param m Given point
     * @return Distance to bounding box, 0 if inside the bounding box
     */
    qreal getDistToBoundingBox(const QVector2D& m);

    /**
     * @brief Get how many times a horizontal ray from m to +infinity crosses this curve
     *
     * @param m The origin of the ray
     * @return Number of times the ray crosses the curve
     */
    int getNumCrossings(const QVector2D& m);

private:

    /**
     * @brief Forces heavy calculations to be redone at a later time
     */
    void invalidateCalc();

    /**
     * @brief Calculates the arc length, equidistant t lookup table etc.
     */
    void calculateLUTs();

    /**
     * @brief Calculates the curvatures along the curve
     */
    void calculateKappaLUT();

    /**
     * @brief Sets the min/max X of the bounding box from the given value if value is less/greater than the min/max
     *
     * @param newX New value
     */
    void updateMinMaxX(qreal newX);

    /**
     * @brief Sets the min/max Y of the bounding box from the given value if value is less/greater than the min/max
     *
     * @param newY New value
     */
    void updateMinMaxY(qreal newY);

    /**
     * @brief Calculates the bounding box
     */
    void calculateBoundingBox();

    /**
     * @brief Calculates whether the bounding boxes of two cubic Bézier curves intersect
     *
     * @param curve1 First curve
     * @param curve2 Second curve
     * @return Whether the bounding box of the two curves intersect
     */
    static bool boundingBoxesIntersect(CubicBezier& curve1, CubicBezier& curve2);

    bool boundingBoxCalculated = false;                                       ///< Whether the bounding box is calculated for this curve
    bool LUTsCalculated = false;                                              ///< Whether the equidistant t/point lookup table is calculated
    bool kappaLUTCalculated = false;                                          ///< Whether the curvatures along the curve are calculated

    qreal maxKappa;                                                           ///< Max absolute curvature found along the curve

    bool isDegenerate = false;                                                ///< Whether this curve is an approximation of a line
    QVector2D p[4];                                                           ///< Control points

    static constexpr qreal ARC_LENGTH_FIRST_PASS_EPSILON = 0.5;               ///< Maximum subdivision length in mm for adaptive geometry approximation
    static constexpr qreal ARC_LENGTH_EPSILON = 10.0;                         ///< Desired subdivision length in mm for piecewise equidistant linear approximation
    static constexpr qreal CLOSEST_T_SEARCH_EPSILON = 0.01;                   ///< Closest point search interval limit, in mm
    qreal arcLength;                                                          ///< Approximate length of the curve

    QVector<qreal> tLUT;                                                      ///< List of t that are approximately equidistant to eachother on the curve
    QVector<QVector2D> pointLUT;                                              ///< List of points that correspond to tLUT
    QVector<qreal> kappaLUT;                                                  ///< List of curvatures that currespond to tLUT

    qreal minX = std::numeric_limits<qreal>::max();                           ///< Minimal x bound for the curve
    qreal maxX = std::numeric_limits<qreal>::min();                           ///< Maximum x bound for the curve
    qreal minY = std::numeric_limits<qreal>::max();                           ///< Minimum y bound for the curve
    qreal maxY = std::numeric_limits<qreal>::min();                           ///< Maximum y bound for the curve

};

/** @endcond */

}

#endif // CUBICBEZIER_H
