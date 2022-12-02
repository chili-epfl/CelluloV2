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
 * @file CelluloZoneAngleInterval.h
 * @brief Header for angular interval zones
 * @author Ayberk Özgür
 * @date 2018-07-18
 */

#ifndef CELLULOZONEANGLEINTERVAL_H
#define CELLULOZONEANGLEINTERVAL_H

#include "CelluloZone.h"

namespace Cellulo{

/**
 * @addtogroup zone
 * @{
 */

/**
 * @brief CelluloZone Base for angular interval zones
 *
 * The zone is described by the limits fromAngle to toAngle, in that order; e.g if 330 and 30 are provided, the zone
 * interval includes 0 degrees and not 180 degrees.
 *
 * @abstract
 */
class CelluloZoneAngleInterval : public CelluloZone {
    /* *INDENT-OFF* */
    Q_OBJECT
    /* *INDENT-ON* */

    /** @brief Beginning of this zone (deg) */
    Q_PROPERTY(float fromAngle WRITE setFromAngle READ getFromAngle NOTIFY fromAngleChanged)

    /** @brief End of this zone (deg) */
    Q_PROPERTY(float toAngle WRITE setToAngle READ getToAngle NOTIFY toAngleChanged)

public:

    /** @cond DO_NOT_DOCUMENT */

    /**
     * @brief Creates a new Cellulo angular zone
     */
    CelluloZoneAngleInterval();

    /**
     * @brief Gets the first limit angle
     * @return First limit angle
     */
    float getFromAngle(){ return fromAngle; }

    /**
     * @brief Sets the first limit angle
     * @param newAngle1 First limit angle
     */
    void setFromAngle(float newFromAngle);

    /**
     * @brief Gets the second limit angle
     * @return Second limit angle
     */
    float getToAngle(){ return toAngle; }

    /**
     * @brief Sets the second limit angle
     * @param newAngle1 Second limit angle
     */
    void setToAngle(float newToAngle);

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

protected:

    /** @cond DO_NOT_DOCUMENT */

    qreal fromAngle;    ///< First limit angle
    qreal toAngle;      ///< Second limit angle

    /** @endcond */

signals:

    /** @cond DO_NOT_DOCUMENT */

    /**
     * @brief Emitted when the first limit angle changes
     */
    void fromAngleChanged();

    /**
     * @brief Emitted when the second limit angle changes
     */
    void toAngleChanged();

    /** @endcond */

};

/**
 * @brief Describes an angular zone sensitive to the client angle being in/out of the interval
 *
 * Calculates whether the client's orientation is within the zone's angle interval, value is `0.0` or `1.0`. The zone is
 * described by the limits **fromAngle** to **toAngle**, in that order; e.g if `330` and `30` are provided, the zone
 * interval includes `0` degrees and not `180` degrees.
 */
class CelluloZoneAngleIntervalInner : public CelluloZoneAngleInterval {
    /* *INDENT-OFF* */
    Q_OBJECT
    /* *INDENT-ON* */

public:

    /** @cond DO_NOT_DOCUMENT */

    CelluloZoneAngleIntervalInner();

    /**
     * @brief Calculate whether the robot lies inside this angle zone
     *
     * @param xRobot UNUSED
     * @param yRobot UNUSED
     * @param thetaRobot Orientation of the robot
     *
     * @return 1 if the robot is between the zone's angles, 0 otherwise
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
 * @brief Describes an angular zone sensitive to the client angle being on the border of the interval
 *
 * Calculates whether the client's orientation is within **borderThickness** of the zone's angle interval limits, value
 * is `0.0` or `1.0`
 */
class CelluloZoneAngleIntervalBorder : public CelluloZoneAngleInterval {
    /* *INDENT-OFF* */
    Q_OBJECT
    /* *INDENT-ON* */

    /** @brief Total thickness of the border (deg) */
    Q_PROPERTY(qreal borderThickness WRITE setBorderThickness READ getBorderThickness NOTIFY borderThicknessChanged)

public:

    /** @cond DO_NOT_DOCUMENT */

    CelluloZoneAngleIntervalBorder();

    /**
     * @brief Gets the border thickness
     *
     * @return Border thickness in degrees
     */
    qreal getBorderThickness(){ return borderThickness; }

    /**
     * @brief Sets the new border thickness
     *
     * @param newThickness New thickness in degrees
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
     * @brief Calculate whether the robot lies on the border of this angular zone
     *
     * @param xRobot UNUSED
     * @param yRobot UNUSED
     * @param thetaRobot Orientation of the robot in degrees
     *
     * @return 1 if the robot is on the border of this angular zone, 0 otherwise
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

    qreal borderThickness;  ///< The border thickness in degrees

};

/**
 * @brief Describes an angular zone sensitive to the client orientation distance to the interval
 *
 * Calculates the client's angular distance to the zone's angle interval. The zone is described by the limits
 * **fromAngle** to **toAngle**, in that order; e.g if `330` and `30` are provided, the zone interval includes `0`
 * degrees and not `180` degrees. Always calculates the shortest distance to the interval.
 */
class CelluloZoneAngleIntervalDistance : public CelluloZoneAngleInterval {
    /* *INDENT-OFF* */
    Q_OBJECT
    /* *INDENT-ON* */

public:

    /** @cond DO_NOT_DOCUMENT */

    CelluloZoneAngleIntervalDistance();

    /**
     * @brief Calculate the distance between the robot orientation and one of the zone limits
     *
     * @param xRobot UNUSED
     * @param yRobot UNUSED
     * @param thetaRobot Orientation of the robot
     *
     * @return The distance between the robot orientation and the limits of this zone; 0 if inside the zone
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

#endif // CELLULOZONEANGLEINTERVAL_H
