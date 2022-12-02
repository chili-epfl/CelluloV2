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
 * @file CelluloZone.h
 * @brief Header for base class zone architecture
 * @author Joanna Salathé
 * @date 2016-03-04
 */

#ifndef CELLULOZONE_H
#define CELLULOZONE_H

#include <QQuickItem>
#include <QQuickPaintedItem>
#include <QPainter>
#include <QHash>
#include <QJsonObject>
#include <QJsonArray>

#include "CelluloZoneTypes.h"
#include "CelluloZoneClient.h"
#include "CelluloZonePaintedItem.h"

namespace Cellulo{

class CelluloZonePaintedItem;

/**
 * @addtogroup zone
 * @{
 */

/**
 * @brief CelluloZone Base Class for zones
 *
 * Abstract description of a "zone" on a 2D plane. These zones could be closed curves, polygons, open curves,
 * even points.
 *
 * They are meant interact with `CelluloZoneClient`s (such as a robot, or a virtual robot on a screen), calculating a
 * real value with respect to each client (for example, the distance to the client in the case of a point zone). With
 * the help of `CelluloZoneEngine`, a zone emits each client's `zoneValueChanged()` signal upon the changing of the
 * value with respect to that client.
 */
class CelluloZone : public QQuickItem {
    /* *INDENT-OFF* */
    Q_OBJECT
    /* *INDENT-ON* */

    /** @brief Whether this zone is active, i.e generates `zoneValueChanged` signals on `CelluloZoneClient`s, default true */
    Q_PROPERTY(bool active WRITE setActive READ isActive NOTIFY activeChanged)

    /** @brief Name of this zone, must be unique, default is `"anonymousZone"` */
    Q_PROPERTY(QString name WRITE setName READ getName NOTIFY nameChanged)

    /** @brief Type of this zone, read-only */
    Q_PROPERTY(Cellulo::CelluloZoneTypes::ZoneType type READ getType NOTIFY typeChanged)

public:

    /** @cond DO_NOT_DOCUMENT */

    /**
     * @brief Creates a new Cellulo zone
     */
    explicit CelluloZone(QQuickItem* parent = 0);

    /**
     * @brief Deletes this zone and its PaintedItem if it exists
     */
    ~CelluloZone();

    /**
     * @brief Get the type of the zone
     *
     * @return Type of the zone
     */
    CelluloZoneTypes::ZoneType getType(){ return type; }

    /**
     * @brief Get the name of the zone
     *
     * @return Name of the zone
     */
    QString getName(){ return name; }

    /**
     * @brief Set the name of the zone
     *
     * @param newName new name for the zone
     */
    void setName(QString newName);

    /**
     * @brief Return if the zone is active or not
     *
     * @return Active state
     */
    float isActive(){ return active; }

    /**
     * @brief Sets active state
     *
     * @param newActive new active state
     */
    void setActive(float newActive);

    /** @endcond */

    /**
     * @brief Calculate the zone quantity of this zone according to the robot's pose
     *
     * @param xRobot x position of the robot
     * @param yRobot y position of the robot
     * @param thetaRobot theta position of the robot
     *
     * @return Zone quantity for this zone and this robot's pose
     */
    Q_INVOKABLE virtual float calculate(float xRobot, float yRobot, float thetaRobot) = 0;

    /** @cond DO_NOT_DOCUMENT */

    /**
     * @brief Write the zone infos to the given json Object
     *
     * @param QJsonObject Json object to be written
     */
    virtual void write(QJsonObject& json);

    /**
     * @brief Read the zone infos from the given json Object
     *
     * @param json Json object to be read
     */
    virtual void read(const QJsonObject& json);

    /**
     * @brief Draws this zone onto the painter
     *
     * @param painter Object to draw onto
     * @param color Color to paint with
     * @param canvasWidth Screen width of the canvas in pixels
     * @param canvasHeight Screen height of the canvas in pixels
     * @param physicalWidth Physical width of the canvas in mm
     * @param physicalHeight Physical height of the canvas in mm
     */
    virtual void paint(QPainter* painter, QColor color, qreal canvasWidth, qreal canvasHeight, qreal physicalWidth, qreal physicalHeight) = 0;

    /** @endcond */

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
    Q_INVOKABLE virtual bool isMouseInside(QVector2D  mousePosition, qreal canvasWidth, qreal canvasHeight, qreal physicalWidth, qreal physicalHeight) = 0;

    /**
     * @brief Creates a PaintedItem that is the visual representation of this zone
     *
     * @param parent Visual QML parent that this item will fill
     * @param color Color to paint with
     * @param physicalPlaygroundWidth Playground width in mm
     * @param physicalPlaygroundHeight Playground height in mm
     *
     * @return A PaintedItem that is the visual representation of this zone
     */
    Q_INVOKABLE Cellulo::CelluloZonePaintedItem* createPaintedItem(QQuickItem* parent, QColor color, qreal physicalPlaygroundWidth, qreal physicalPlaygroundHeight);

    /**
     * @brief Associates a painted item with this zone so that it can be redrawn if the zone changes
     *
     * @param newPaintedItem New painted item
     */
    Q_INVOKABLE void setPaintedItem(Cellulo::CelluloZonePaintedItem* newPaintedItem);

    /**
     * @brief Get painted item associated with this zone so that its properties can be changed
     *
     * @return The PaintedItem that is the visual representation of this zone
     */
    Q_INVOKABLE Cellulo::CelluloZonePaintedItem* getPaintedItem(){ return paintedItem; }

public slots:

    /** @cond DO_NOT_DOCUMENT */

    /**
     * @brief Slot associated to the CelluloZoneClient signal poseChanged
     *
     * Calculates zone quantity for the sender client (must inherit CelluloZoneClient) if zone is active
     *
     * @param x New x coordinate of the client
     * @param y New y coordinate of the client
     * @param theta New orientaton of the client
     */
    void onClientPoseChanged(qreal x, qreal y, qreal theta);

    /** @endcond */

protected:

    /** @cond DO_NOT_DOCUMENT */

    QString name;                               ///< Name of the zone
    CelluloZoneTypes::ZoneType type;            ///< Type of the zone

    /**
     * @brief Repaints the associated PaintedItem (if any)
     */
    void updatePaintedItem();

    /** @endcond */

signals:

    /** @cond DO_NOT_DOCUMENT */

    /**
     * @brief Emitted when the active state of the zone changes
     */
    void activeChanged();

    /**
     * @brief Emitted when the name of the zone changes
     */
    void nameChanged();

    /**
     * @brief Emitted when the type of the zone changes
     */
    void typeChanged();

    /** @endcond */

private:

    QHash<Cellulo::CelluloZoneClient*, qreal> clientsLastValues;    ///< Stores the most recent values calculated for clients
    CelluloZonePaintedItem* paintedItem;                            ///< The PaintedItem associated with this zone
    bool active;                                                    ///< Whether the zone is active

};

/** @} */

}

Q_DECLARE_METATYPE(Cellulo::CelluloZone*)

#endif // CELLULOZONE_H
