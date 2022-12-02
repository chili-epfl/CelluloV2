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
 * @file CelluloZonePolyBezier.h
 * @brief Header for composite cubic Bézier curves
 * @author Ayberk Özgür
 * @date 2016-07-21
 */

#ifndef CELLULOZONEPOLYBEZIER_H
#define CELLULOZONEPOLYBEZIER_H

#include "CelluloZone.h"
#include "../util/math/CubicBezier.h"
#include "../util/math/PolyBezier.h"
#include "../comm/CelluloBluetooth.h"

namespace Cellulo{

class CelluloBluetooth;

/**
 * @addtogroup zone
 * @{
 */

/**
 * @brief CelluloZone Base class for composite cubic Bézier curve
 *
 * The curve is composed of continuous cubic Bézier curve segments, as seen in https://en.wikipedia.org/wiki/Composite_B%C3%A9zier_curve
 * The curve is compliant with the SVG standard of continuous cubic Bézier segments.
 *
 * It is composed of 3n+1 control points where the points at indices [3k, 3k+1, 3k+2, 3k+3] correspond to a Bézier curve.
 *
 * @abstract
 */
class CelluloZonePolyBezier : public CelluloZone {
    /* *INDENT-OFF* */
    Q_OBJECT
    /* *INDENT-ON* */

    /** @brief List of consecutive control points, must contain `3N + 1` points; `N` is the number of segments */
    Q_PROPERTY(QVariantList controlPoints WRITE setControlPoints READ getControlPoints NOTIFY controlPointsChanged)

public:

    /** @cond DO_NOT_DOCUMENT */

    /**
     * @brief Creates a new Cellulo composite Bézier curve
     */
    CelluloZonePolyBezier();

    /**
     * @brief Gets the curve's control points
     *
     * @return List of curve's control points, contains 3n+1 points where points at indices [3k, 3k+1, 3k+2, 3k+3] correspond to a Bézier curve
     */
    QVariantList getControlPoints() const;

    /**
     * @brief Sets the curve's control points
     *
     * @param newVertices List of curve's control points, must contain 3n+1 points where points at indices [3k, 3k+1, 3k+2, 3k+3] correspond to a Bézier curve
     */
    void setControlPoints(const QVariantList& newControlPoints);

    /**
     * @brief Draws this zone onto the painter
     *
     * @param painter Object to draw onto
     * @param color Color of the paint
     * @param canvasWidth Screen width of the canvas in pixels
     * @param canvasHeight Screen height of the canvas in pixels
     * @param physicalWidth Physical width of the canvas in mm
     * @param physicalHeight Physical height of the canvas in mm
     */
    virtual void paint(QPainter* painter, QColor color, qreal canvasWidth, qreal canvasHeight, qreal physicalWidth, qreal physicalHeight) override;

    /**
     * @brief Write the zone infos to the given json Object
     * @param QJsonObject json object to be written
     */
    virtual void write(QJsonObject& json) override;

    /**
     * @brief Read the zone infos from the given json Object
     * @param json json object to be read
     */
    virtual void read(const QJsonObject& json) override;

    /**
     * @brief Get if the mouse position is inside the zone or not
     *
     * @param mousePosition (x,y) coordinates of the mouse in pixels
     * @param canvasWidth Screen width of the canvas in pixels
     * @param canvasHeight Screen height of the canvas in pixels
     * @param physicalWidth Physical width of the canvas in mm
     * @param physicalHeight Physical height of the canvas in mm
     *
     * @return Whether the mouse position is inside the zone or not
     */
    Q_INVOKABLE virtual bool isMouseInside(QVector2D  mousePosition, qreal canvasWidth, qreal canvasHeight, qreal physicalWidth, qreal physicalHeight) override;

    /** @endcond */

public slots:

    /**
     * @brief [DEPRECATED, USE PolyBezierTracker INSTEAD] Sends this path to the given robot in order to be followed
     *
     * @param robot The robot to send the path to
     */
    void sendPathToRobot(Cellulo::CelluloBluetooth* robot) const;

    /**
     * @brief Gets the point on the curve corresponding to the given parameter
     *
     * @param t Given parameter t in [0,numSegments]
     * @return Point on curve corresponding to t
     */
    QVector2D getPoint(qreal t);

    /**
     * @brief Gets the tangent direction of point on the curve corresponding to the given parameter
     *
     * @param t Given parameter t in [0,numSegments]
     * @return Tangent direction of the point on curve corresponding to t
     */
    QVector2D getTangent(qreal t);

    /**
     * @brief Gets the normal direction of point on the curve corresponding to the given parameter
     *
     * @param t Given parameter t in [0,numSegments]
     * @return Normal direction of the point on curve corresponding to t
     */
    QVector2D getNormal(qreal t);

signals:

    /** @cond DO_NOT_DOCUMENT */

    /**
     * @brief Emitted when the control points change
     */
    void controlPointsChanged();

    /** @endcond */

protected:

    /** @cond DO_NOT_DOCUMENT */

    PolyBezier curve;   ///< The underlying curve to this zone

    /** @endcond */

};

/**
 * @brief Calculates the parameter t of the closest point on a composite Bézier curve
 *
 * Calculates the parameter `t` of the composite Bézier curve whose corresponding point is closest to the client.
 * `t` will be in `[0, number of segments]` where every interval of size `1` corresponds to the parameter `t` on that
 * particular segment within the curve.
 */
class CelluloZonePolyBezierClosestT : public CelluloZonePolyBezier {
    /* *INDENT-OFF* */
    Q_OBJECT
    /* *INDENT-ON* */

public:

    /** @cond DO_NOT_DOCUMENT */

    CelluloZonePolyBezierClosestT();

    /**
     * @brief Calculate the parameter t of the closest point on the composite curve to the robot
     *
     * @param xRobot x position of the robot
     * @param yRobot y position of the robot
     * @param thetaRobot theta position of the robot
     *
     * @return Parameter t in [0,numSegments]
     */
    Q_INVOKABLE virtual float calculate(float xRobot, float yRobot, float thetaRobot) override;

    /**
     * @brief Draws this zone onto the painter
     *
     * @param painter Object to draw onto
     * @param color Color of the paint
     * @param canvasWidth Screen width of the canvas in pixels
     * @param canvasHeight Screen height of the canvas in pixels
     * @param physicalWidth Physical width of the canvas in mm
     * @param physicalHeight Physical height of the canvas in mm
     */
    virtual void paint(QPainter* painter, QColor color, qreal canvasWidth, qreal canvasHeight, qreal physicalWidth, qreal physicalHeight) override;

    /** @endcond */

protected:

    /** @cond DO_NOT_DOCUMENT */

    static constexpr qreal GET_T_EPSILON = 0.01; ///< Get t from x/y calculation sensitivity, in mm
    static constexpr qreal GET_T_LIM_T = 0.001;  ///< Get t from x/y calculation t interval limit

    /** @endcond */

};

/**
 * @brief Calculates the parameter t of the point on the composite Bézier curve with the same x coordinate as the client
 *
 * Calculates the parameter `t` of the composite Bézier curve whose corresponding point has the same x coordinate as
 * the client. `t` will be in `[0, number of segments]` where every interval of size `1` corresponds to the parameter
 * `t` on that particular segment within the curve. Assumes that the curve is 1-to-1 along the x axis, i.e it is of the
 * form `y = f(t)`.
 *
 * Assumes that the curve is 1-to-1 along the x axis, i.e it is of the form y = f(t).
 */
class CelluloZonePolyBezierXT : public CelluloZonePolyBezierClosestT {
    /* *INDENT-OFF* */
    Q_OBJECT
    /* *INDENT-ON* */

public:

    /** @cond DO_NOT_DOCUMENT */

    CelluloZonePolyBezierXT();

    /**
     * @brief Calculates the parameter t of the point on the composite curve with the same x coordinate as the client
     *
     * Assumes that the curve is 1-to-1 along the x axis, i.e it is of the form y = f(t).
     *
     * @param xRobot x position of the robot
     * @param yRobot UNUSED
     * @param thetaRobot UNUSED
     *
     * @return Parameter t in [0,numSegments]
     */
    Q_INVOKABLE virtual float calculate(float xRobot, float yRobot, float thetaRobot) override;

    /**
     * @brief Draws this zone onto the painter
     *
     * @param painter Object to draw onto
     * @param color Color of the paint
     * @param canvasWidth Screen width of the canvas in pixels
     * @param canvasHeight Screen height of the canvas in pixels
     * @param physicalWidth Physical width of the canvas in mm
     * @param physicalHeight Physical height of the canvas in mm
     */
    virtual void paint(QPainter* painter, QColor color, qreal canvasWidth, qreal canvasHeight, qreal physicalWidth, qreal physicalHeight) override;

    /** @endcond */

};

/**
 * @brief Calculates the parameter t of the point on the composite Bézier curve with the same y coordinate as the client
 *
 * Calculates the parameter `t` of the composite Bézier curve whose corresponding point has the same y coordinate as the
 * client. `t` will be in `[0, number of segments]` where every interval of size `1` corresponds to the parameter `t` on
 * that particular segment within the curve. Assumes that the curve is 1-to-1 along the y axis, i.e it is of the form
 * `x = f(t)`.
 *
 * Assumes that the curve is 1-to-1 along the y axis, i.e it is of the form x = f(t).
 */
class CelluloZonePolyBezierYT : public CelluloZonePolyBezierClosestT {
    /* *INDENT-OFF* */
    Q_OBJECT
    /* *INDENT-ON* */

public:

    /** @cond DO_NOT_DOCUMENT */

    CelluloZonePolyBezierYT();

    /**
     * @brief Calculates the parameter t of the point on the composite curve with the same y coordinate as the client
     *
     * Assumes that the curve is 1-to-1 along the y axis, i.e it is of the form x = f(t).
     *
     * @param xRobot UNUSED
     * @param yRobot y position of the robot
     * @param thetaRobot UNUSED
     *
     * @return Parameter t in [0,numSegments]
     */
    Q_INVOKABLE virtual float calculate(float xRobot, float yRobot, float thetaRobot) override;

    /**
     * @brief Draws this zone onto the painter
     *
     * @param painter Object to draw onto
     * @param color Color of the paint
     * @param canvasWidth Screen width of the canvas in pixels
     * @param canvasHeight Screen height of the canvas in pixels
     * @param physicalWidth Physical width of the canvas in mm
     * @param physicalHeight Physical height of the canvas in mm
     */
    virtual void paint(QPainter* painter, QColor color, qreal canvasWidth, qreal canvasHeight, qreal physicalWidth, qreal physicalHeight) override;

    /** @endcond */

};

/**
 * @brief Calculates the distance to a composite Bézier curve
 *
 * Calculates the distance of the client to the composite Bézier curve.
 */
class CelluloZonePolyBezierDistance : public CelluloZonePolyBezierClosestT {
    /* *INDENT-OFF* */
    Q_OBJECT
    /* *INDENT-ON* */

public:

    /** @cond DO_NOT_DOCUMENT */

    CelluloZonePolyBezierDistance();

    /**
     * @brief Calculate the closest distance of the composite curve to the robot
     *
     * @param xRobot x position of the robot
     * @param yRobot y position of the robot
     * @param thetaRobot theta position of the robot
     *
     * @return Closest distance to any curve among the segments
     */
    Q_INVOKABLE virtual float calculate(float xRobot, float yRobot, float thetaRobot) override;

    /**
     * @brief Draws this zone onto the painter
     *
     * @param painter Object to draw onto
     * @param color Color of the paint
     * @param canvasWidth Screen width of the canvas in pixels
     * @param canvasHeight Screen height of the canvas in pixels
     * @param physicalWidth Physical width of the canvas in mm
     * @param physicalHeight Physical height of the canvas in mm
     */
    virtual void paint(QPainter* painter, QColor color, qreal canvasWidth, qreal canvasHeight, qreal physicalWidth, qreal physicalHeight) override;

    /** @endcond */

};

/**
 * @brief Calculates whether the client is on the border of a composite Bézier curve
 *
 * Calculates whether the client's position is within **borderThickness** of the zone's border, value is `0.0` or `1.0`.
 */
class CelluloZonePolyBezierBorder : public CelluloZonePolyBezier {
    /* *INDENT-OFF* */
    Q_OBJECT
    /* *INDENT-ON* */

    /** @brief Total thickness of the border (mm) */
    Q_PROPERTY(qreal borderThickness WRITE setBorderThickness READ getBorderThickness NOTIFY borderThicknessChanged)

public:

    /** @cond DO_NOT_DOCUMENT */

    CelluloZonePolyBezierBorder();

    /**
     * @brief Gets the border thickness
     *
     * @return Border thickness in mm
     */
    qreal getBorderThickness(){ return borderThickness; }

    /**
     * @brief Sets the new border thickness
     *
     * @param newThickness New thickness in mm
     */
    void setBorderThickness(qreal newThickness);

    /**
     * @brief Write the zone infos to the given json Object
     * @param QJsonObject json object to be written
     */
    virtual void write(QJsonObject &json) override;

    /**
     * @brief Read the zone infos from the given json Object
     * @param json json object to be read
     */
    virtual void read(const QJsonObject &json) override;

    /**
     * @brief Calculate whether the robot is within the border
     *
     * @param xRobot x position of the robot
     * @param yRobot y position of the robot
     * @param thetaRobot theta position of the robot
     *
     * @return Whether the robot is within the border
     */
    Q_INVOKABLE virtual float calculate(float xRobot, float yRobot, float thetaRobot) override;

    /**
     * @brief Draws this zone onto the painter
     *
     * @param painter Object to draw onto
     * @param color Color of the paint
     * @param canvasWidth Screen width of the canvas in pixels
     * @param canvasHeight Screen height of the canvas in pixels
     * @param physicalWidth Physical width of the canvas in mm
     * @param physicalHeight Physical height of the canvas in mm
     */
    virtual void paint(QPainter* painter, QColor color, qreal canvasWidth, qreal canvasHeight, qreal physicalWidth, qreal physicalHeight) override;

    /** @endcond */

signals:

    /** @cond DO_NOT_DOCUMENT */

    /**
     * @brief Emitted when border thickness changes
     */
    void borderThicknessChanged();

    /** @endcond */

private:

    qreal borderThickness;      ///< The border thickness in mm

};

/**
 * @brief Calculates whether the client is within the composite Bézier curve
 *
 * Calculates whether the client is within the composite Bézier curve, value is `0.0` or `1.0`. The first and last
 * control points of the curve are assumed to be connected with a line segment in order to ensure the curve is closed,
 * if the curve is not already closed.
 */
class CelluloZonePolyBezierInner : public CelluloZonePolyBezier {
    /* *INDENT-OFF* */
    Q_OBJECT
    /* *INDENT-ON* */

public:

    /** @cond DO_NOT_DOCUMENT */

    CelluloZonePolyBezierInner();

    /**
     * @brief Calculate whether the robot lies inside this composite Bézier curve
     *
     * @param xRobot x position of the robot
     * @param yRobot y position of the robot
     * @param thetaRobot theta position of the robot
     *
     * @return 1 if the robot is inside this zone 0 otherwise
     */
    Q_INVOKABLE virtual float calculate(float xRobot, float yRobot, float thetaRobot) override;

    /**
     * @brief Draws this zone onto the painter
     *
     * @param painter Object to draw onto
     * @param color Color of the paint
     * @param canvasWidth Screen width of the canvas in pixels
     * @param canvasHeight Screen height of the canvas in pixels
     * @param physicalWidth Physical width of the canvas in mm
     * @param physicalHeight Physical height of the canvas in mm
     */
    virtual void paint(QPainter* painter, QColor color, qreal canvasWidth, qreal canvasHeight, qreal physicalWidth, qreal physicalHeight) override;

    /** @endcond */

};

/** @} */

}

#endif // CELLULOZONEPOLYBEZIER_H
