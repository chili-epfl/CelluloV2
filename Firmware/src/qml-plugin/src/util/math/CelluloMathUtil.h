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
 * @file CelluloMathUtil.h
 * @brief Header for mathematical and geometrical utilities
 * @author Joanna Salathé
 * @author Ayberk Özgür
 * @date 2016-06-30
 */

#ifndef CELLULOMATHUTIL_H
#define CELLULOMATHUTIL_H

#include <QObject>
#include <QVector2D>
#include <QVariantList>

#if defined(Q_OS_WIN)
	#define _USE_MATH_DEFINES
	#include <math.h>
#else
	#include <cmath>
#endif

namespace Cellulo{

/**
 * @addtogroup util
 * @{
 */

/**
 * @brief Mathematical utilities used throughout Celullo activities.
 * @singleton
 */
class CelluloMathUtil : public QObject {
    /* *INDENT-OFF* */
    Q_OBJECT
    /* *INDENT-ON* */

public:

    /** @cond DO_NOT_DOCUMENT */

    CelluloMathUtil(QObject* parent = 0);
    ~CelluloMathUtil();

    /** @endcond */

    /**
     * @brief Calculates the distance of a point to a line segment in 2D
     *
     * @param p The point
     * @param seg1 The first endpoint of the line segment
     * @param seg2 The second endpoint of the line segment
     * @return Distance between the point and the line segment
     */
    Q_INVOKABLE static qreal pointToSegmentDist(const QVector2D& p, const QVector2D& seg1, const QVector2D& seg2);

    /**
     * @brief Calculates the signed distance of a point to a line in 2D
     *
     * @param p The point
     * @param lp1 First point the line passes through
     * @param lp2 Second point the line passes through
     * @return Distance between the point and line segment, negative if point is below the line
     */
    Q_INVOKABLE static qreal pointToLineDistSigned(const QVector2D& p, const QVector2D& lp1, const QVector2D& lp2);

    /**
     * @brief Calculates whether the point is above a line in 2D
     *
     * @param p The point
     * @param lp1 First point the line passes through
     * @param lp2 Second point the line passes through
     * @return Whether the point is above the line
     */
    Q_INVOKABLE static bool pointAboveLine(const QVector2D& p, const QVector2D& lp1, const QVector2D& lp2);

    /**
     * @brief Calculates whether the given point is in the given polygon
     *
     * @param p The point
     * @param v The vertices of the polygon
     * @return Whether the point lies in the given polygon
     */
    Q_INVOKABLE static bool pointInPoly(const QVector2D& p, const QList<QVector2D>& v);

    /**
     * @brief Calculates the distance of the point to the polygon border
     *
     * @param p The point (may be in the polygon)
     * @param v The vertices of the polygon
     * @return The distance of the point to the polygon border
     */
    Q_INVOKABLE static qreal pointToPolyBorderDist(const QVector2D& p, const QList<QVector2D>& v);

    /** @cond DO_NOT_DOCUMENT */

    /**
     * @brief Calculates the roots of ax + b = 0
     *
     * @param a 1st order coefficient
     * @param b Constant
     * @param x [out] Root if exists
     * @return Number of unique real roots, i.e 0 or 1
     */
    static int solveLinearEq(qreal a, qreal b, qreal& x);

    /**
     * @brief Calculates the roots of ax^2 + bx + c = 0
     *
     * @param a 2nd order coefficient
     * @param b 1st order coefficient
     * @param c Constant
     * @param x1 [out] First root if real roots exist
     * @param x2 [out] Second root if real roots exist
     * @return Number of unique real roots, i.e 0, 1 or 2
     */
    static int solveQuadEq(qreal a, qreal b, qreal c, qreal& x1, qreal& x2);

    /**
     * @brief Calculates the roots of ax^3 + bx^2 + cx + d = 0
     *
     * @param a 3rd order coefficient, must be nonzero
     * @param b 2nd order coefficient
     * @param c 1st order coefficient
     * @param d Constant
     * @param x1 [out] First unique root if real roots exist
     * @param x2 [out] Second unique root if real roots exist (will be equal to x3 if there are two roots, equal to x1 if there is only one root)
     * @param x3 [out] Third unique root if real roots exist (will be equal to x2 if there are two roots, equal to x1 if there is only one root)
     * @return Number of unique real roots, i.e 1, 2 or 3
     */
    static int solveCubicEq(qreal a, qreal b, qreal c, qreal d, qreal& x1, qreal& x2, qreal& x3);

    /**
     * @brief Checks whether the value is within -epsilon,+epsilon
     *
     * @param val The value
     * @param epsilon The tolerance
     * @return Whether the number is within -epsilon,+epsilon
     */
    static bool isZero(qreal val, qreal epsilon);

    /** @endcond */

    /**
     * @brief Calculates whether a horizontal ray starting from r and continuing towards +infinity crosses the line segment
     *
     * @param r Ray origin
     * @param seg1 Line segment first endpoint
     * @param seg2 Line segment second endpoint
     * @return Whether the ray crosses the line segment
     */
    Q_INVOKABLE static bool hRayCrossesLineSeg(const QVector2D& r, const QVector2D& seg1, const QVector2D& seg2);

    /**
     * @brief Calculates whether the ray crosses the line segment
     *
     * @param origin Ray origin
     * @param dir Ray direction
     * @param seg1 Line segment first endpoint
     * @param seg2 Line segment second endpoint
     * @return Whether the ray crosses the line segment
     */
    Q_INVOKABLE static bool rayCrossesLineSeg(QVector2D const& origin, QVector2D const& dir, QVector2D const& seg1, QVector2D const& seg2);

    /**
     * @brief Initializes c's random number generator seed
     *
     * This will be called with time(NULL) at CelluloMathUtil creation time.
     *
     * @param seed The seed
     */
    Q_INVOKABLE static void c_srand(unsigned int seed);

    /**
     * @brief Calls c's random number generator
     *
     * @param max Interval size
     * @return Random integer in [0, max-1]
     */
    Q_INVOKABLE static int c_rand(unsigned int max);

    /**
     * @brief Gets whether the given points are collinear enough
     *
     * @param a Point 1
     * @param b Point 2
     * @param c Point 3
     * @return Whether the three points are collinear enough
     */
    Q_INVOKABLE static bool collinear(const QVector2D& a, const QVector2D& b, const QVector2D& c);

    /**
     * @brief std::random_shuffles a list
     *
     * @param list The list to shuffle
     * @return Shuffled list
     */
    Q_INVOKABLE static QVariantList shuffle(const QVariantList& list);

    /**
     * @brief Returns the given vector rotated by the given angle
     *
     * @param vector Vector to rotate
     * @param angleRad Angle to rotate by
     * @return Rotated vector
     */
    Q_INVOKABLE static QVector2D rotateVector(QVector2D const& vector, qreal angleRad);

    /**
     * @brief Converts degrees to radians
     *
     * @param deg Angle in degrees
     * @return Corresponding angle in radians
     */
    Q_INVOKABLE static constexpr qreal degToRad(qreal deg){ return deg*M_PI/180.0; }

    /**
     * @brief Converts radians to degrees
     *
     * @param rad Angle in radians
     * @return Corresponding angle in degrees
     */
    Q_INVOKABLE static constexpr qreal radToDeg(qreal rad){ return rad*180.0/M_PI; }

    /**
     * @brief Calculates a parametric sigmoid function value
     *
     * @param L Magnitude parameter
     * @param beta Steepness parameter
     * @param r_0 Horizontal offset parameter
     * @param r Input
     * @return L/(1 + exp(beta*(r - r_0)))
     */
    Q_INVOKABLE static qreal sigmoid(qreal L, qreal beta, qreal r_0, qreal r);

    /**
     * @brief Clamps value in an interval
     *
     * @param val Value to clamp
     * @param min Minimum allowed value
     * @param max Maximum allowed value
     * @return Value in [min, max]
     */
    Q_INVOKABLE static qreal clamp(qreal val, qreal min, qreal max);

    /**
     * @brief Returns the angle between two vectors
     *
     * @param  vec1 First vector, must have positive length
     * @param  vec2 Second vector, must have positive length
     * @return      Angle between in rad
     */
    Q_INVOKABLE static qreal angleBetween(QVector2D const& vec1, QVector2D const& vec2);

    /**
     * @brief Returns the signed cross product length of two 2D vectors
     *
     * @param  vec1 First vector, must have positive length
     * @param  vec2 Second vector, must have positive length
     * @return      vec1.x*vec2.y - vec1.y*vec2.x
     */
    Q_INVOKABLE static qreal crossProduct(QVector2D const& vec1, QVector2D const& vec2);

private:

    static constexpr qreal COLLINEAR_EPSILON = 1e-2;   ///< Epsilon to use during collinear check
    static constexpr qreal SOLVE_CUBIC_EPSILON = 1e-9; ///< Epsilon to use during isZero by the cubic equation solver
    static constexpr qreal SOLVE_QUAD_EPSILON = 1e-4;  ///< Epsilon to use during isZero by the quadratic equation solver

};

/** @} */

}

#endif // CELLULOMATHUTIL_H
