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
 * @file CelluloZoneRectangle.h
 * @brief Header for the rectangular zone
 * @author Joanna Salathé
 * @date 2016-03-04
 */

#ifndef CELLULOZONERECTANGLE_H
#define CELLULOZONERECTANGLE_H

#include "CelluloZone.h"

namespace Cellulo{

/**
 * @addtogroup zone
 * @{
 */

/**
 * @brief CelluloZone Base Class for rectangular zones
 * @abstract
 */
class CelluloZoneRectangle : public CelluloZone {
    /* *INDENT-OFF* */
    Q_OBJECT
    /* *INDENT-ON* */

    /** @brief X coordinate of the top left corner (mm) */
    Q_PROPERTY(float x WRITE setX READ getX NOTIFY xChanged)

    /** @brief Y coordinate of the top left corner (mm) */
    Q_PROPERTY(float y WRITE setY READ getY NOTIFY yChanged)

    /** @brief Width of the rectangle (mm) */
    Q_PROPERTY(float height WRITE setHeight READ getHeight NOTIFY heightChanged)

    /** @brief Height of the rectangle (mm) */
    Q_PROPERTY(float width WRITE setWidth READ getWidth NOTIFY widthChanged)

public:

    /** @cond DO_NOT_DOCUMENT */

    /**
     * @brief Creates a new Cellulo rectangular zone
     */
    CelluloZoneRectangle();

    /**
     * @brief Gets the x position of upper left corner of the rectangle
     * @return X position of upper left corner of the rectangle
     */
    float getX() { return x; }

    /**
     * @brief Updates the rectangle's left edge x position
     * @param newX New x position in mm
     */
    void setX(float newX);

    /**
     * @brief Gets the y position of upper left corner of the rectangle
     * @return Y position of upper left corner of the rectangle
     */
    float getY() { return y; }

    /**
     * @brief Updates the rectangle's top edge y position
     * @param newY New y position in mm
     */
    void setY(float newY);

    /**
     * @brief Gets the height of the rectangle
     * @return Height of the rectangle
     */
    float getHeight() { return height; }

    /**
     * @brief Updates the rectangle's height
     * @param newHeight New height in mm
     */
    void setHeight(float newHeight);

    /**
     * @brief Gets the width of the rectangle
     * @return Width of the rectangle
     */
    float getWidth() { return width; }

    /**
     * @brief Updates the rectangle's width
     * @param newHeight New width in mm
     */
    void setWidth(float newWidth);

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

    /**
     * @brief Get if the point is inside the zone or not
     *
     * @param pointX x coordinate of the point in mm
     * @param pointY y coordinate of the point in mm
     *
     * @return Whether the point is inside the zone or not
     */
    bool isPointInside(float pointX, float pointY);

    float x;                                ///< x position of the upper left corner of the rectangle
    float y;                                ///< y position of the upper left corner of the rectangle
    float height;                           ///< height of the rectangle
    float width;                            ///< width of the rectangle

    /** @endcond */

signals:

    /** @cond DO_NOT_DOCUMENT */

    /**
     * @brief Emitted when the x position of the upper left corner of the rectangular zone's center changes
     */
    void xChanged();

    /**
     * @brief Emitted when the y postion of the upper left corner of the rectangular zone's center changes
     */
    void yChanged();

    /**
     * @brief Emitted when the height of the rectangular zone changes
     */
    void heightChanged();

    /**
     * @brief Emitted when the height of the rectangular zone changes
     */
    void widthChanged();

    /** @endcond */

};

/**
 * @brief CelluloZone Specific Class for rectangular zones inner determination; calculates whether the client's position is within the rectangle, value is `0.0` or `1.0`.
 */
class CelluloZoneRectangleInner : public CelluloZoneRectangle {
    /* *INDENT-OFF* */
    Q_OBJECT
    /* *INDENT-ON* */

public:

    /** @cond DO_NOT_DOCUMENT */

    CelluloZoneRectangleInner();

    /**
     * @brief Calculate whether the robot lies inside this rectangular zone.
     *
     * This function is normally called automatically, but can be manually called as well.
     *
     * @param xRobot x position of the robot
     * @param yRobot y position of the robot
     * @param thetaRobot theta position of the robot
     *
     * @return 1 if the robot is inside this rectangular zone 0 otherwise
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
 * @brief CelluloZone Specific Class for rectangular zones border determination; calculates whether the client's position is within **borderThickness** of the zone's border, value is `0.0` or `1.0`.
 */
class CelluloZoneRectangleBorder : public CelluloZoneRectangle {
    /* *INDENT-OFF* */
    Q_OBJECT
    /* *INDENT-ON* */

    /** @brief Total thickness of the border (mm) */
    Q_PROPERTY(qreal borderThickness WRITE setBorderThickness READ getBorderThickness NOTIFY borderThicknessChanged)

public:

    /** @cond DO_NOT_DOCUMENT */

    CelluloZoneRectangleBorder();

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
     * @brief Calculate whether the robot lies on the border of this rectangular zone (given the zone's margin)
     *
     * This function is normally called automatically, but can be manually called as well.
     *
     * @param xRobot x position of the robot
     * @param yRobot y position of the robot
     * @param thetaRobot theta position of the robot
     *
     * @return 1 if the robot is on the border of this rectangular zone (given the zone's margin) 0 otherwise
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
 * @brief CelluloZone Specific Class for rectangular zones distance determination; calculates the client's distance to the zone's border.
 */
class CelluloZoneRectangleDistance : public CelluloZoneRectangle {
    /* *INDENT-OFF* */
    Q_OBJECT
    /* *INDENT-ON* */

public:

    /** @cond DO_NOT_DOCUMENT */

    CelluloZoneRectangleDistance();

    /**
     * @brief Calculate the distance between the robot pose and the border of this rectangular zone.
     *
     * This function is normally called automatically, but can be manually called as well.
     *
     * @param xRobot x position of the robot
     * @param yRobot y position of the robot
     * @param thetaRobot theta position of the robot
     *
     * @return the distance between the robot pose and the border of this rectangular zone
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

#endif // CELLULOZONERECTANGLE_H
