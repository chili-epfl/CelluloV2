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
 * @file CelluloZonePaintedItem.h
 * @brief Header for a QML compatible QQuickPaintedItem for Cellulo zones
 * @author Ayberk Özgür
 * @date 2016-06-28
 */

#ifndef CELLULOZONEPAINTEDITEM_H
#define CELLULOZONEPAINTEDITEM_H

#include <QQuickPaintedItem>
#include <QMetaObject>
#include <QEvent>

#include "CelluloZone.h"

namespace Cellulo{

class CelluloZone;

/**
 * @addtogroup zone
 * @{
 */

/**
 * @brief A QML compatible QQuickPaintedItem for CelluloZones
 *
 * Visual representation of a `CelluloZone`; can be used to draw `CelluloZone`s in e.g `Rectangle`s. Visually covers its
 * parent (from `[0, 0]` to `[parent.width, parent.height]`), using it as a canvas and drawing the zone at the
 * appropriate place.
 */
class CelluloZonePaintedItem : public QQuickPaintedItem {
    /* *INDENT-OFF* */
    Q_OBJECT
    /* *INDENT-ON* */

    /** @brief The color of the drawn zone */
    Q_PROPERTY(QColor color WRITE setColor READ getColor NOTIFY colorChanged)

    /** @brief Associated zone */
    Q_PROPERTY(Cellulo::CelluloZone* associatedZone WRITE setAssociatedZone READ getAssociatedZone NOTIFY associatedZoneChanged)

    /** @brief Physical playground width that the canvas represents (in mm) */
    Q_PROPERTY(qreal physicalPlaygroundWidth WRITE setPhysicalPlaygroundWidth READ getPhysicalPlaygroundWidth NOTIFY physicalPlaygroundWidthChanged)

    /** @brief Physical playground height that the canvas represents (in mm) */
    Q_PROPERTY(qreal physicalPlaygroundHeight WRITE setPhysicalPlaygroundHeight READ getPhysicalPlaygroundHeight NOTIFY physicalPlaygroundHeightChanged)

public:

    /** @cond DO_NOT_DOCUMENT */

    /**
     * @brief Creates a new CelluloZonePaintedItem
     *
     * @param parent QML parent
     */
    explicit CelluloZonePaintedItem(QQuickItem* parent = 0);

    /**
     * @brief Gets the color to paint with
     *
     * @return The paint color
     */
    QColor getColor(){ return color; }

    /**
     * @brief Updates the color to paint with
     *
     * @param newColor The new paint color
     */
    void setColor(QColor newColor);

    /**
     * @brief Sets the associated zone
     *
     * @param associatedZone The CelluloZone that will take care of painting
     */
    void setAssociatedZone(CelluloZone* zone);

    /**
     * @brief Gets the associated zone
     *
     * @return The CelluloZone that takes care of painting
     */
    CelluloZone* getAssociatedZone(){ return associatedZone; }

    /**
     * @brief Changes the physical playground width
     *
     * @param newWidth New width in millimeters
     */
    void setPhysicalPlaygroundWidth(qreal newWidth);

    /**
     * @brief Gets the physical playground width
     *
     * @return The physical playground width in mm
     */
    qreal getPhysicalPlaygroundWidth(){ return physicalPlaygroundWidth; }

    /**
     * @brief Changes the physical playground height
     *
     * @param newWidth New height in millimeters
     */
    void setPhysicalPlaygroundHeight(qreal newHeight);

    /**
     * @brief Gets the physical playground height
     *
     * @return The physical playground height in mm
     */
    qreal getPhysicalPlaygroundHeight(){ return physicalPlaygroundHeight; }

    /**
     * @brief Implementation of pure virtual function in QQuickPaintedItem
     *
     * @param painter Object to paint on
     */
    void paint(QPainter* painter) override;

    /** @endcond */

signals:

    /** @cond DO_NOT_DOCUMENT */

    /**
     * @brief Emitted when the paint color changes
     */
    void colorChanged();

    /**
     * @brief Emitted when the associated zone changes
     */
    void associatedZoneChanged();

    /**
     * @brief Emitted when the physical playground width changes
     */
    void physicalPlaygroundWidthChanged();

    /**
     * @brief Emitted when the physical playground height changes
     */
    void physicalPlaygroundHeightChanged();

    /** @endcond */

private slots:

    /**
     * @brief Sets this item's width equal to the parent's width
     */
    void setWidthToParent();

    /**
     * @brief Sets this item's height equal to the parent's height
     */
    void setHeightToParent();

private:

    /**
     * @brief Disconnects old binding, connects new binding and updates width/height
     *
     * @param newParent The new parent
     */
    void updateParentWidthHeightBinding(QQuickItem* newParent);

    /**
     * @brief Override; binds the new parent's height/width to this object
     *
     * @param change The change that occurred, must be ItemParentHasChanged
     * @param value Points to the new parent when change is ItemParentHasChanged
     */
    void itemChange(ItemChange change, const ItemChangeData& value) override;

    QColor color;                                   ///< Color of the paint
    CelluloZone* associatedZone;                    ///< The zone that will draw on the painter
    qreal physicalPlaygroundWidth;                  ///< Physical playground width in mm
    qreal physicalPlaygroundHeight;                 ///< Physical playground height in mm

    QMetaObject::Connection parentWidthConnection;  ///< Connection to bind this object's width to the parent
    QMetaObject::Connection parentHeightConnection; ///< Connection to bind this object's height to the parent
};

/** @} */

}

#endif // CELLULOZONEPAINTEDITEM_H
