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
 * @file CelluloZonePoint.h
 * @brief Header for the point zone
 * @author Joanna Salathé
 * @date 2016-03-04
 */

#ifndef CELLULOZONEPOINT_H
#define CELLULOZONEPOINT_H

#include "CelluloZone.h"

namespace Cellulo{

/**
 * @addtogroup zone
 * @{
 */

/**
 * @brief CelluloZone Base Class for point zones
 * @abstract
 */
class CelluloZonePoint : public CelluloZone {
    /* *INDENT-OFF* */
    Q_OBJECT
    /* *INDENT-ON* */

    /** @brief X coordinate of the point (mm) */
    Q_PROPERTY(float x WRITE setX READ getX NOTIFY xChanged)

    /** @brief Y coordinate of the point (mm) */
    Q_PROPERTY(float y WRITE setY READ getY NOTIFY yChanged)

public:

    /** @cond DO_NOT_DOCUMENT */

    /**
     * @brief Creates a new Cellulo point zone
     */
    CelluloZonePoint();

    /**
     * @brief Gets the x position of the point
     * @return X position of the point
     */
    float getX() { return x; }

    /**
     * @brief Updates the x position of the point
     * @param newX New x position in mm
     */
    void setX(float newX);

    /**
     * @brief Gets the y position of the point
     * @return Y position of the point
     */
    float getY() { return y; }

    /**
     * @brief Updates the y position of the point
     * @param newY New y position in mm
     */
    void setY(float newY);

    /**
     * @brief Write the zone infos to the given json Object
     * @param json json object to be written
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

    float x;                               ///< X position of the point
    float y;                               ///< Y position of the point

    /** @endcond */

signals:

    /** @cond DO_NOT_DOCUMENT */

    /**
     * @brief Emitted when the X position of the point changes
     */
    void xChanged();

    /**
     * @brief Emitted when the Y position of the point changes
     */
    void yChanged();

    /** @endcond */

};

/**
 * @brief CelluloZone Specific Class for point zones distance determination
 *
 * Calculates the client's distance to the zone.
 */
class CelluloZonePointDistance : public CelluloZonePoint {
    /* *INDENT-OFF* */
    Q_OBJECT
    /* *INDENT-ON* */

public:

    /** @cond DO_NOT_DOCUMENT */

    CelluloZonePointDistance();

    /**
     * @brief Calculate the distance between the robot pose and this point zone
     *
     * @param xRobot x position of the robot
     * @param yRobot y position of the robot
     * @param thetaRobot theta position of the robot
     *
     * @return the distance between the robot pose and this point zone
     */
    Q_INVOKABLE float calculate(float xRobot, float yRobot, float thetaRobot) override;

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

#endif // CELLULOZONEPOINT_H
