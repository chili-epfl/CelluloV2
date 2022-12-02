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
 * @file CelluloZoneLine.h
 * @brief Header for the line zone
 * @author Joanna Salathé
 * @date 2016-03-04
 */

#ifndef CELLULOZONELINE_H
#define CELLULOZONELINE_H

#include "CelluloZone.h"

namespace Cellulo{

/**
 * @addtogroup zone
 * @{
 */

/**
 * @brief CelluloZone Base Class for line zones
 * @abstract
 */
class CelluloZoneLine : public CelluloZone {
    /* *INDENT-OFF* */
    Q_OBJECT
    /* *INDENT-ON* */

    /** @brief X coordinate of the first line segment end (mm) */
    Q_PROPERTY(float x1 WRITE setX1 READ getX1 NOTIFY x1Changed)

    /** @brief Y coordinate of the first line segment end (mm) */
    Q_PROPERTY(float y1 WRITE setY1 READ getY1 NOTIFY y1Changed)

    /** @brief X coordinate of the second line segment end (mm) */
    Q_PROPERTY(float x2 WRITE setX2 READ getX2 NOTIFY x2Changed)

    /** @brief Y coordinate of the second line segment end (mm) */
    Q_PROPERTY(float y2 WRITE setY2 READ getY2 NOTIFY y2Changed)

public:

    /** @cond DO_NOT_DOCUMENT */

    /**
     * @brief Creates a new Cellulo line zone
     */
    CelluloZoneLine();

    /**
     * @brief Gets the x position of the line's first point
     * @return X position of the line's first point
     */
    float getX1(){ return x1; }

    /**
     * @brief Updates the x position of the line first point
     * @param newx1 New x position in mm
     */
    void setX1(float newX1);

    /**
     * @brief Gets the y position of the line's first point
     * @return Y position of the line's first point
     */
    float getY1(){ return y1; }

    /**
     * @brief Updates the y position of the line first point
     * @param newx1 New y position in mm
     */
    void setY1(float newY1);

    /**
     * @brief Gets the x position of the line's second point
     * @return X position of the line's second point
     */
    float getX2(){ return x2; }

    /**
     * @brief Updates the x position of the line second point
     * @param newX2 New x position in mm
     */
    void setX2(float newX2);

    /**
     * @brief Gets the y position of the line's second point
     * @return Y position of the line's second point
     */
    float getY2(){ return y2; }

    /**
     * @brief Updates the y position of the line second point
     * @param newY1 New y position in mm
     */
    void setY2(float newY2);

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

    float x1;   ///< X coordinate of the line's first point
    float y1;   ///< Y coordinate of the line's first point
    float x2;   ///< X coordinate of the line's second point
    float y2;   ///< Y coordinate of the line's second point

    /** @endcond */

signals:

    /** @cond DO_NOT_DOCUMENT */

    /**
     * @brief Emitted when the X coordinate of the line's first point changes
     */
    void x1Changed();

    /**
     * @brief Emitted when the Y coordinate of the line's first point changes
     */
    void y1Changed();

    /**
     * @brief Emitted when the X coordinate of the line's second point changes
     */
    void x2Changed();

    /**
     * @brief Emitted when the Y coordinate of the line's second point changes
     */
    void y2Changed();

    /** @endcond */

};

/**
 * @brief CelluloZone Specific Class for line segment distance determination
 *
 * Calculates the client's distance to the zone that is a line segment.
 */
class CelluloZoneLineSegmentDistance : public CelluloZoneLine {
    /* *INDENT-OFF* */
    Q_OBJECT
    /* *INDENT-ON* */

public:

    /** @cond DO_NOT_DOCUMENT */

    CelluloZoneLineSegmentDistance();

    /**
     * @brief Calculate the distance between the robot position and this line segment
     *
     * @param xRobot x position of the robot
     * @param yRobot y position of the robot
     * @param thetaRobot theta position of the robot
     *
     * @return the distance between the robot position and this line segment
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

/**
 * @brief CelluloZone Specific Class for signed line distance determination
 *
 * Calculates the client's (signed) distance to the zone that is an infinite line.
 * The two points of CelluloZoneLine are taken as the two points that this infinite line passes through.
 */
class CelluloZoneLineDistanceSigned : public CelluloZoneLine {
    /* *INDENT-OFF* */
    Q_OBJECT
    /* *INDENT-ON* */

public:

    /** @cond DO_NOT_DOCUMENT */

    CelluloZoneLineDistanceSigned();

    /**
     * @brief Calculate the distance between the robot position and line
     *
     * @param xRobot x position of the robot
     * @param yRobot y position of the robot
     * @param thetaRobot theta position of the robot
     *
     * @return The distance between the robot position and this line, negative if point is below the line
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

/**
 * @brief CelluloZone Specific Class for determining whether a point is within a distance to an infinite line
 *
 * Calculates whether the client's position is within **borderThickness** of the zone's border, value is `0.0` or `1.0`.
 * The two points of CelluloZoneLine are taken as the two points that this infinite line passes through.
 */
class CelluloZoneLineBorder : public CelluloZoneLine {
    /* *INDENT-OFF* */
    Q_OBJECT
    /* *INDENT-ON* */

    /** @brief Total thickness of the border (mm) */
    Q_PROPERTY(qreal borderThickness WRITE setBorderThickness READ getBorderThickness NOTIFY borderThicknessChanged)

public:

    /** @cond DO_NOT_DOCUMENT */

    CelluloZoneLineBorder();

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
     * @brief Calculate whether the distance between the robot position and line is below the border thickness
     *
     * @param xRobot x position of the robot
     * @param yRobot y position of the robot
     * @param thetaRobot theta position of the robot
     *
     * @return 1 if within border, 0 if outside
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

/** @} */

}

#endif // CELLULOZONELINE_H
