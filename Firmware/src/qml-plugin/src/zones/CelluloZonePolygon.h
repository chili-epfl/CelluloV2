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
 * @file CelluloZonePolygon.h
 * @brief Header for the polygon zone
 * @author Joanna Salathé
 * @date 2016-03-04
 */

#ifndef CELLULOZONEPOLYGON_H
#define CELLULOZONEPOLYGON_H

#include "CelluloZone.h"
#include <math.h>

namespace Cellulo{

/**
 * @addtogroup zone
 * @{
 */

/**
 * @brief CelluloZone Base Class for polygonal zones
 * @abstract
 */
class CelluloZonePolygon : public CelluloZone {
    /* *INDENT-OFF* */
    Q_OBJECT
    /* *INDENT-ON* */

    /** @brief List of vertices, polygon has an edge between every `i`th and `i+1`th vertex */
    Q_PROPERTY(QList<QVariant> vertices WRITE setVerticesAsVariants READ getVerticesAsVariants NOTIFY verticesChanged)


public:

    /** @cond DO_NOT_DOCUMENT */

    /**
     * @brief Creates a new Cellulo polygonal zone
     */
    CelluloZonePolygon();

    /**
     * @brief Gets the polygon's vertices in their QVariant representation
     *
     * @return List of polygon's vertices in their QVariant representation
     */
    QList<QVariant> getVerticesAsVariants() {
        QList<QVariant> newVerticesAsVariants;
        for (int i = 0; i < vertices.size(); ++i) {
            newVerticesAsVariants.append(QVariant::fromValue(vertices.at(i)));
        }
        return newVerticesAsVariants;
    }

    /**
     * @brief Sets the polygon's vertices in their QVariant representation
     *
     * @param newVertices List of new vertices in their QVariant representation
     */
    Q_INVOKABLE void setVerticesAsVariants(QList<QVariant> newVerticesAsVariants) {
        setVertices(convertQVariantToQVector2D(newVerticesAsVariants));
    }

    /**
     * @brief Gets the polygon's vertices
     *
     * @return List of polygon's vertices
     */
    const QList<QVector2D>& getVertices(){ return vertices; }

    /**
     * @brief Sets the polygon's vertices
     *
     * @param newVertices List of new vertices
     */
    virtual void setVertices(const QList<QVector2D>& newVertices);

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

signals:

    /** @cond DO_NOT_DOCUMENT */

    /**
     * @brief Emitted when the vertices change
     */
    void verticesChanged();

    /** @endcond */

protected:

    /** @cond DO_NOT_DOCUMENT */

    /**
     * @brief Updates the bounding rectangle from the new list of vertices
     */
    void updateBounds();

    /**
     * @brief Convert QVariant vertices to QVector2D vertices
     */
    QList<QVector2D> convertQVariantToQVector2D(QList<QVariant> newVerticesAsVariants);

    /**
     * @brief Get if the point is inside the zone or not
     *
     * @param pointX x coordinate of the point in mm
     * @param pointY y coordinate of the point in mm
     *
     * @return Whether the point is inside the zone or not
     */
    bool isPointInside(float pointX, float pointY);

    QList<QVector2D> vertices; ///< Vertices of the polygon
    float minX;                ///< Minimal x bound for the polygon
    float maxX;                ///< Maximum x bound for the polygon
    float minY;                ///< Minimum y bound for the polygon
    float maxY;                ///< Maximum y bound for the polygon

    /** @endcond */

};


/**
 * @brief CelluloZone Base Class for irregular polygonal zones
 * @abstract
 */
class CelluloZoneIrregularPolygon : public CelluloZonePolygon {
    /* *INDENT-OFF* */
    Q_OBJECT
    /* *INDENT-ON* */

public:

    /** @cond DO_NOT_DOCUMENT */

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

    /** @endcond */

};

/**
 * @brief CelluloZone Specific Class for irregular polygonal zones inner determination
 *
 * Calculates whether the client's position is within the irregular polygon, value is `0.0` or `1.0`.
 */
class CelluloZoneIrregularPolygonInner : public CelluloZoneIrregularPolygon {
    /* *INDENT-OFF* */
    Q_OBJECT
    /* *INDENT-ON* */

public:

    /** @cond DO_NOT_DOCUMENT */

    CelluloZoneIrregularPolygonInner();

    /**
     * @brief Calculate whether the robot lies inside this irregular polygonal zone
     *
     * @param xRobot x position of the robot
     * @param yRobot y position of the robot
     * @param thetaRobot theta position of the robot
     *
     * @return 1 if the robot is inside this irregular polygonal zone 0 otherwise
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
 * @brief CelluloZone Specific Class for irregular polygonal zones border determination
 *
 * Calculates whether the client's position is within **borderThickness** of the zone's border, value is `0.0` or `1.0`.
 */
class CelluloZoneIrregularPolygonBorder : public CelluloZoneIrregularPolygon {
    /* *INDENT-OFF* */
    Q_OBJECT
    /* *INDENT-ON* */

    /** @brief Total thickness of the border (mm) */
    Q_PROPERTY(qreal borderThickness WRITE setBorderThickness READ getBorderThickness NOTIFY borderThicknessChanged)

public:

    /** @cond DO_NOT_DOCUMENT */

    CelluloZoneIrregularPolygonBorder();

    /**
     * @brief Gets the border thickness
     *
     * @return Border thickness in mm
     */
    qreal getBorderThickness(){
        return borderThickness;
    }

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
     * @brief Calculate whether the robot lies on the border of this irregular polygonal zone (given the zone's margin)
     *
     * @param xRobot x position of the robot
     * @param yRobot y position of the robot
     * @param thetaRobot theta position of the robot
     *
     * @return 1 if the robot is on the border of this irregular polygonal zone (given the zone's margin) 0 otherwise
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
 * @brief CelluloZone Specific Class for irregular polygonal zones distance determination
 *
 * Calculates the client's distance to the zone's border.
 */
class CelluloZoneIrregularPolygonDistance : public CelluloZoneIrregularPolygon {
    /* *INDENT-OFF* */
    Q_OBJECT
    /* *INDENT-ON* */

public:

    /** @cond DO_NOT_DOCUMENT */

    CelluloZoneIrregularPolygonDistance();

    /**
     * @brief Calculate the distance between the robot pose and the border of this irregular polygonal zone
     *
     * @param xRobot x position of the robot
     * @param yRobot y position of the robot
     * @param thetaRobot theta position of the robot
     *
     * @return the distance between the robot pose and the border of this irregular polygonal zone
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
 * @brief Base class for regular polygon zones
 * @abstract
 */
class CelluloZoneRegularPolygon : public CelluloZonePolygon {
    /* *INDENT-OFF* */
    Q_OBJECT
    /* *INDENT-ON* */

    /** @brief Number of edges */
    Q_PROPERTY(int numEdges WRITE setNumEdges READ getNumEdges NOTIFY numEdgesChanged)

    /** @brief X coordinate of the center (mm) */
    Q_PROPERTY(float x WRITE setX READ getX NOTIFY xChanged)

    /** @brief Y coordinate of the center (mm) */
    Q_PROPERTY(float y WRITE setY READ getY NOTIFY yChanged)

    /** @brief Radius of the enclosing circle (mm) */
    Q_PROPERTY(float r WRITE setR READ getR NOTIFY rChanged)

    /** @brief Extra rotation from the initial orientation (deg) */
    Q_PROPERTY(float rotAngle WRITE setRotAngle READ getRotAngle NOTIFY rotAngleChanged)

public:

    /** @cond DO_NOT_DOCUMENT */

    /**
     * @brief Creates a new Cellulo regular polygonal zone
     */
    CelluloZoneRegularPolygon();

    /**
     * @brief Disables modifying vertices directly
     *
     * @param newVertices Don't care
     */
    virtual void setVertices(const QList<QVector2D>& newVertices) override;

    /**
     * @brief Gets the number of edges of the regular polygon
     *
     * @return The number of edges of the regular polygon
     */
    int getNumEdges(){ return numEdges; }

    /**
     * @brief Sets the number of edges
     *
     * @param newNumEdge New number of edges, must be greater than 2
     */
    void setNumEdges(int newNumEdge);

    /**
     * @brief Gets the x position of regular polygon definer circle's center
     *
     * @return X position of regular polygon definer circle's center
     */
    float getX(){ return x; }

    /**
     * @brief Sets the x position of the center of the regular polygon
     *
     * @param newX Center x position in mm
     */
    void setX(float newX);

    /**
     * @brief Gets the y position of regular polygon definer circle's center
     *
     * @return Y position of regular polygon definer circle's center
     */
    float getY(){ return y; }

    /**
     * @brief Sets the y position of the center of the regular polygon
     *
     * @param newY Center y position in mm
     */
    void setY(float newY);

    /**
     * @brief Gets the radius of the regular polygon definer circle
     *
     * @return R the radius of the regular polygon definer circle
     */
    float getR(){ return r; }

    /**
     * @brief Sets the radius of the outer circle of the regular polygon
     *
     * @param newR Radius of the outer circle in mm
     */
    void setR(float newR);

    /**
     * @brief Gets the rotational angle of the regular polygon
     *
     * @return The rotational angle of the regular polygon
     */
    float getRotAngle(){ return rotAngle; }

    /**
     * @brief Sets the orientation of the regular polygon
     *
     * @param newRotAngle The
     */
    void setRotAngle(float newRotAngle);

    /**
     * @brief Write the zone infos to the given json Object
     *
     * @param QJsonObject json object to be written
     */
    virtual void write(QJsonObject &json) override;

    /**
     * @brief Read the zone infos from the given json Object
     *
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

    /** @endcond */

signals:

    /** @cond DO_NOT_DOCUMENT */

    /**
     * @brief Emitted when the number of edges of the regular polygonal zone changes
     */
    void numEdgesChanged();

    /**
     * @brief Emitted when the x position of regular polygon defined circle's center of the regular polygonal zone changes
     */
    void xChanged();

    /**
     * @brief Emitted when the y position of regular polygon defined circle's center of the regular polygonal zone changes
     */
    void yChanged();

    /**
     * @brief Emitted when the radius of the regular polygon defined circle of the regular polygonal zone changes
     */
    void rChanged();

    /**
     * @brief Emitted when the rotational angle of the regular polygonal zone changes
     */
    void rotAngleChanged();

    /** @endcond */

protected:

    /** @cond DO_NOT_DOCUMENT */

    float numEdges;                         ///< number of edges of the regular polygon
    float x;                                ///< x position of regular polygon defined circle's center
    float y;                                ///< y position of regular polygon defined circle's center
    float r;                                ///< radius of the regular polygon defined circle
    float rotAngle;                         ///< rotational angle of the regular polygon

    /** @endcond */

private:

    /**
     * @brief Updates the vertices given the center position, radius of the outer circle, number of edges and rotation angle parameters
     */
    void updateVerticesFromRegularPolyParams();

};

/**
 * @brief CelluloZone Specific Class for regular polygonal zones inner determination
 *
 * Calculates whether the client's position is within the regular polygon, value is `0.0` or `1.0`.
 */
class CelluloZoneRegularPolygonInner : public CelluloZoneRegularPolygon {
    /* *INDENT-OFF* */
    Q_OBJECT
    /* *INDENT-ON* */

public:

    /** @cond DO_NOT_DOCUMENT */

    CelluloZoneRegularPolygonInner();

    /**
     * @brief Calculate whether the robot lies inside this regular polygonal zone
     *
     * @param xRobot x position of the robot
     * @param yRobot y position of the robot
     * @param thetaRobot theta position of the robot
     *
     * @return 1 if the robot is inside this regular polygonal zone 0 otherwise
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
 * @brief CelluloZone Specific Class for regular polygonal zones border determination
 *
 * Calculates whether the client's position is within **borderThickness** of the zone's border, value is `0.0` or `1.0`.
 */
class CelluloZoneRegularPolygonBorder : public CelluloZoneRegularPolygon {
    /* *INDENT-OFF* */
    Q_OBJECT
    /* *INDENT-ON* */

    /** @brief Total thickness of the border (mm) */
    Q_PROPERTY(qreal borderThickness WRITE setBorderThickness READ getBorderThickness NOTIFY borderThicknessChanged)

public:

    /** @cond DO_NOT_DOCUMENT */

    CelluloZoneRegularPolygonBorder();

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
     * @brief Calculate whether the robot lies on the border of this regular polygonal zone (given the zone's margin)
     *
     * @param xRobot x position of the robot
     * @param yRobot y position of the robot
     * @param thetaRobot theta position of the robot
     *
     * @return 1 if the robot is on the border of this regular polygonal zone (given the zone's margin) 0 otherwise
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

    qreal borderThickness;          ///< The border thickness in mm

};

/**
 * @brief CelluloZone Specific Class for regular polygonal zones distance determination
 *
 * Calculates the client's distance to the zone's border.
 */
class CelluloZoneRegularPolygonDistance : public CelluloZoneRegularPolygon {
    /* *INDENT-OFF* */
    Q_OBJECT
    /* *INDENT-ON* */

public:

    /** @cond DO_NOT_DOCUMENT */

    CelluloZoneRegularPolygonDistance();

    /**
     * @brief Calculate the distance between the robot pose and the border of this regular polygonal zone
     *
     * @param xRobot x position of the robot
     * @param yRobot y position of the robot
     * @param thetaRobot theta position of the robot
     *
     * @return the distance between the robot pose and the border of this regular polygonal zone
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

#endif // CELLULOZONEPOLYGON_H
