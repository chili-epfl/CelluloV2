/*
 * Copyright (C) 2018 EPFL
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
 * @file AxialHexCoords.h
 * @brief Header for a hexagonal tile axial coordinates definition
 * @author Ayberk Özgür
 * @date 2018-05-16
 */

#ifndef AXIALHEXCOORDS_H
#define AXIALHEXCOORDS_H

#include <QQuickItem>

namespace Cellulo {

/**
 * @addtogroup tile
 * @{
 */

/**
 * @brief Hexagonal tile definition, built in axial hex coordinates
 *
 * See https://www.redblobgames.com/grids/hexagons/#coordinates-axial.
 */
class AxialHexCoords : public QQuickItem {
    /* *INDENT-OFF* */
    Q_OBJECT
    /* *INDENT-ON* */

    /** @brief Tile width (mm), height would be 2*width/sqrt(3) */
    Q_PROPERTY(float physicalWidth READ getPhysicalWidth WRITE setPhysicalWidth NOTIFY physicalWidthChanged)

    /** @brief Tile height (mm), 2*width/sqrt(3) */
    Q_PROPERTY(float physicalHeight READ getPhysicalHeight NOTIFY physicalHeightChanged)

    /** @brief Target Q index (horizontal) in axial discrete hex tile coordinates */
    Q_PROPERTY(int q READ getQ WRITE setQ NOTIFY qChanged)

    /** @brief Target R index (vertical, 120 degrees to the Q axis) in axial discrete hex tile coordinates */
    Q_PROPERTY(int r READ getR WRITE setR NOTIFY rChanged)

public:

    /** @cond DO_NOT_DOCUMENT */

    /**
     * @brief Creates a new hex tile axial coordinates
     */
    explicit AxialHexCoords(QQuickItem* parent = 0);

    /**
     * @brief Deletes this hex tile axial coordinates
     */
    virtual ~AxialHexCoords();

    /**
     * @brief Copies other coordinates into this one
     *
     * @param other Coordinates to be copied
     */
    void copyFrom(AxialHexCoords const& other);

    /**
     * @brief Gets the horizontal hex tile coordinate
     *
     * @return Horizontal hex tile coordinate
     */
    int getQ() const { return q; }

    /**
     * @brief Gets the vertical hex tile coordinate
     *
     * @return Vertical hex tile coordinate
     */
    int getR() const { return r; }

    /**
     * @brief Sets the horizontal hex tile coordinate
     *
     * @param q Horizontal hex tile coordinate
     */
    void setQ(int q);

    /**
     * @brief Sets the vertical hex tile coordinate
     *
     * @param r Vertical hex tile coordinate
     */
    void setR(int r);

    /**
     * @brief Gets the physical width of this tile
     *
     * @return Physical width in mm
     */
    float getPhysicalWidth() const { return physicalWidth; }

    /**
     * @brief Gets the physical height of this tile
     *
     * @return Always physicalWidth*2/sqrt(3)
     */
    float getPhysicalHeight() const { return physicalWidth*1.15470053837925152902f; /* 2/sqrt(3) */ }

    /**
     * @brief Sets the physical width of this tile
     *
     * @param physicalWidth Physical width in mm
     */
    void setPhysicalWidth(float physicalWidth);

    static constexpr float PHYSICAL_WIDTH_DEFAULT = 99.0f; ///< Default physical width of the hex tile in mm (height would be 2*width/sqrt(3))

    /** @endcond */

signals:

    /** @cond DO_NOT_DOCUMENT */

    /**
     * @brief Emitted when the physical tile width changes
     */
    void physicalWidthChanged();

    /**
     * @brief Emitted when the physical tile height changes
     */
    void physicalHeightChanged();

    /**
     * @brief Emitted when q changes
     */
    void qChanged();

    /**
     * @brief Emitted when r changes
     */
    void rChanged();

    /** @endcond */

public slots:

    /**
     * @brief Gets the tile center's coordinates in the continuous mapped space composed of hex tiles
     *
     * @return Tile center's coordinates in the continuous mapped space composed of hex tiles
     */
    QVector2D hexOffset();

    /**
     * @brief Gets the list of the 6 midpoints of the edges of this tile
     *
     * @param list Output list of 6 midpoints
     */
    void edgeMidList(QVector<QVector2D>& list);

    /**
     * @brief Gets the list of the 6 corners of this tile
     *
     * @param list Output list of 6 corners in the order: top, top-left, bottom-left, bottom, bottom-right, top-right
     */
    void cornerList(QVector<QVector2D>& list);

    /**
     * @brief Gets the limits of this tile
     *
     * @param left   Smallest horizontal coordinate
     * @param right  Largest horizontal coordinate
     * @param top    Smallest vertical coordinate
     * @param bottom Largest vertical coordinate
     */
    void limits(float& left, float& right, float& top, float& bottom);

private:

    float physicalWidth;                            ///< Tile width in mm, height would be 2*width/sqrt(3)
    int q;                                          ///< Q index (horizontal) in axial hex tile coordinates
    int r;                                          ///< R index (vertical, 120 degrees to the Q axis) in axial hex tile coordinates

};

/** @} */

}

Q_DECLARE_METATYPE(Cellulo::AxialHexCoords*)

#endif // AXIALHEXCOORDS_H
