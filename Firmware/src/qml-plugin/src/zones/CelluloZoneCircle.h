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
 * @file CelluloZoneCircle.h
 * @brief Header for circle zone
 * @author Joanna Salathé
 * @date 2016-03-04
 */

#ifndef CELLULOZONECIRCLE_H
#define CELLULOZONECIRCLE_H

#include "CelluloZone.h"

namespace Cellulo{

/**
 * @addtogroup zone
 * @{
 */

/**
 * @brief CelluloZone Base Class for circular zones
 * @abstract
 */
class CelluloZoneCircle : public CelluloZone {
    /* *INDENT-OFF* */
    Q_OBJECT
    /* *INDENT-ON* */

    /** @brief Center x coordinate of the circle (mm) */
    Q_PROPERTY(float x WRITE setX READ getX NOTIFY xChanged)

    /** @brief Center y coordinate of the circle (mm) */
    Q_PROPERTY(float y WRITE setY READ getY NOTIFY yChanged)

    /** @brief Radius of the circle (mm) */
    Q_PROPERTY(float r WRITE setR READ getR NOTIFY rChanged)

public:

    /** @cond DO_NOT_DOCUMENT */

    /**
     * @brief Creates a new Cellulo circular zone
     */
    CelluloZoneCircle();

    /**
     * @brief Gets the x position of circle's center
     * @return X position of circle's center
     */
    float getX(){ return x; }

    /**
     * @brief Updates the x position of circle's center
     * @param newX New x position in mm
     */
    void setX(float newX);

    /**
     * @brief Gets the y position of circle's center
     * @return Y position of circle's center
     */
    float getY(){ return y; }

    /**
     * @brief Updates the y position of circle's center
     * @param newY New y position in mm
     */
    void setY(float newY);

    /**
     * @brief Gets the radius of the circle
     * @return R the radius of the circle
     */
    float getR(){ return r; }

    /**
     * @brief Updates the radius of circle
     * @param newR New radius in mm
     */
    void setR(float newR);

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

    /**
     * @brief Get if the point is inside the zone or not
     *
     * @param pointX x coordinate of the point in mm
     * @param pointY y coordinate of the point in mm
     *
     * @return Whether the point is inside the zone or not
     */
    bool isPointInside(float pointX, float pointY);

    float x;    ///< x position of circle's center
    float y;    ///< y position of circle's center
    float r;    ///< r the radius of the circle

    /** @endcond */

signals:

    /** @cond DO_NOT_DOCUMENT */

    /**
     * @brief Emitted when the x position of circular zone's center changes
     */
    void xChanged();

    /**
     * @brief Emitted when the y position of circular zone's center changes
     */
    void yChanged();

    /**
     * @brief Emitted when r the radius of the circular zone changes
     */
    void rChanged();

    /** @endcond */

};

/**
 * @brief CelluloZone Specific Class for circular zones inner determination
 *
 * Calculates whether the client's position is within the circle, value is `0.0` or `1.0`.
 */
class CelluloZoneCircleInner : public CelluloZoneCircle {
    /* *INDENT-OFF* */
    Q_OBJECT
    /* *INDENT-ON* */

public:

    /** @cond DO_NOT_DOCUMENT */

    CelluloZoneCircleInner();

    /**
     * @brief Calculate whether the robot lies inside this circular zone
     *
     * @param xRobot x position of the robot
     * @param yRobot y position of the robot
     * @param thetaRobot theta position of the robot
     *
     * @return 1 if the robot is inside this circular zone 0 otherwise
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
 * @brief CelluloZone Specific Class for circular zones border determination
 *
 * Calculates whether the client's position is within **borderThickness** of the zone's border, value is `0.0` or `1.0`.
 */
class CelluloZoneCircleBorder : public CelluloZoneCircle {
    /* *INDENT-OFF* */
    Q_OBJECT
    /* *INDENT-ON* */

    /** @brief Total thickness of the border (deg) */
    Q_PROPERTY(qreal borderThickness WRITE setBorderThickness READ getBorderThickness NOTIFY borderThicknessChanged)

public:

    /** @cond DO_NOT_DOCUMENT */

    CelluloZoneCircleBorder();

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
     * @brief Calculate whether the robot lies on the border of this circular zone (given the zone's margin)
     *
     * @param xRobot x position of the robot
     * @param yRobot y position of the robot
     * @param thetaRobot theta position of the robot
     *
     * @return 1 if the robot is on the border of this circular zone (given the zone's margin) 0 otherwise
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

    qreal borderThickness;  ///< The border thickness in mm

};

/**
 * @brief CelluloZone Specific Class for circular zones distance determination
 *
 * Calculates the client's distance to the zone's border.
 */
class CelluloZoneCircleDistance : public CelluloZoneCircle {
    /* *INDENT-OFF* */
    Q_OBJECT
    /* *INDENT-ON* */

public:

    /** @cond DO_NOT_DOCUMENT */

    CelluloZoneCircleDistance();

    /**
     * @brief Calculate the distance between the robot pose and the border of this circular zone
     *
     * @param xRobot x position of the robot
     * @param yRobot y position of the robot
     * @param thetaRobot theta position of the robot
     *
     * @return the distance between the robot pose and the border of this circular zone
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

#endif // CELLULOZONECIRCLE_H
