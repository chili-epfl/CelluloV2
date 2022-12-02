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
 * @file HexTile.h
 * @brief Header for a hexagonal tile definition
 * @author Ayberk Özgür
 * @date 2018-04-27
 */

#ifndef HEXTILE_H
#define HEXTILE_H

#include <QQuickItem>
#include <QJsonObject>

#include "HexTileStdSourceCoords.h"
#include "AxialHexCoords.h"

namespace Cellulo {

class HexTileStdSourceCoords;

/**
 * @addtogroup tile
 * @{
 */

/**
 * @brief Hexagonal tile definition, built in axial coordinates
 */
class HexTile : public QQuickItem {
    /* *INDENT-OFF* */
    Q_OBJECT
        /* *INDENT-ON* */

    /** @brief Target axial hex coordinates */
    Q_PROPERTY(Cellulo::AxialHexCoords* coords READ getCoords NOTIFY coordsChanged)

    /** @brief Starting X coordinate of the rectangular source space (mm) */
    Q_PROPERTY(float sourceLeft MEMBER sourceLeft NOTIFY sourceLeftChanged)

    /** @brief Starting Y coordinate of the rectangular source space (mm) */
    Q_PROPERTY(float sourceTop MEMBER sourceTop NOTIFY sourceTopChanged)

    /** @brief Ending X coordinate of the rectangular source space (mm) */
    Q_PROPERTY(float sourceRight MEMBER sourceRight NOTIFY sourceRightChanged)

    /** @brief Ending Y coordinate of the rectangular source space (mm) */
    Q_PROPERTY(float sourceBottom MEMBER sourceBottom NOTIFY sourceBottomChanged)

    /** @brief X coordinate of the center of the hex tile in the source space, with respect to the top/left of the source space (mm) */
    Q_PROPERTY(float sourceCenterX MEMBER sourceCenterX NOTIFY sourceCenterXChanged)

    /** @brief Y coordinate of the center of the hex tile in the source space, with respect to the top/left of the source space (mm) */
    Q_PROPERTY(float sourceCenterY MEMBER sourceCenterY NOTIFY sourceCenterYChanged)

    /** @brief Standard coordinate description, null by default; if used, autosets the source* properties so they must not be manually modified */
    Q_PROPERTY(Cellulo::HexTileStdSourceCoords* stdSourceCoords WRITE setStdSourceCoords READ getStdSourceCoords NOTIFY stdSourceCoordsChanged)

    /** @brief The fill color of the rendering of this tile, white by default */
    Q_PROPERTY(QColor fillColor MEMBER fillColor NOTIFY fillColorChanged)

    /** @brief The border color of the rendering of this tile, black by default */
    Q_PROPERTY(QColor borderColor MEMBER borderColor NOTIFY borderColorChanged)

    /** @brief Size of the border with respect to the width, between 0.0 and 1.0, by default 0.05 */
    Q_PROPERTY(qreal borderSize MEMBER borderSize NOTIFY borderSizeChanged)

public:

    /** @cond DO_NOT_DOCUMENT */

    /**
     * @brief Creates a new hex tile
     */
    explicit HexTile(QQuickItem* parent = 0);

    /**
     * @brief Deletes this hex tile
     */
    virtual ~HexTile();

    /**
     * @brief Axial hex coordinates
     *
     * @return Axial hex coordinates
     */
    Cellulo::AxialHexCoords* getCoords() const { return coords; }

    /**
     * @brief Sets the new standard hex tile coordinate description
     *
     * @param stdSourceCoords New standard hex tile coordinate description
     */
    void setStdSourceCoords(Cellulo::HexTileStdSourceCoords* stdSourceCoords);

    /**
     * @brief Gets the standard hex tile coordinate description
     *
     * @return The current standard hex tile coordinate description
     */
    Cellulo::HexTileStdSourceCoords* getStdSourceCoords(){ return stdSourceCoords; }

    /**
     * @brief Dumps all properties of this tile to the JSON object
     *
     * @return Resulting JSON object describing this tile
     */
    QJsonObject dumpToJSON() const;

    /**
     * @brief Loads all properties of this tile from the given JSON object
     *
     * @param json Descriptor of this tile
     */
    void loadFromJSON(QJsonObject const& json);

    /** @endcond */

signals:

    /** @cond DO_NOT_DOCUMENT */

    /**
     * @brief Emitted when the axial hex tile coordinates change
     */
    void coordsChanged();

    /**
     * @brief Emitted when the standard hex tile coordinate definition changes
     */
    void stdSourceCoordsChanged();

    /**
     * @brief Emitted when sourceLeft changes
     */
    void sourceLeftChanged();

    /**
     * @brief Emitted when sourceRight changes
     */
    void sourceRightChanged();

    /**
     * @brief Emitted when sourceTop changes
     */
    void sourceTopChanged();

    /**
     * @brief Emitted when sourceBottom changes
     */
    void sourceBottomChanged();

    /**
     * @brief Emitted when sourceCenterX changes
     */
    void sourceCenterXChanged();

    /**
     * @brief Emitted when sourceCenterY changes
     */
    void sourceCenterYChanged();

    /**
     * @brief Emitted when the fill color changes
     */
    void fillColorChanged();

    /**
     * @brief Emitted when the border color changes
     */
    void borderColorChanged();

    /**
     * @brief Emitted when the border size changes
     */
    void borderSizeChanged();

    /** @endcond */

    /**
     * @brief Emitted when the user clicks this tile on the screen
     *
     * @param physicalPos Clicked position in mm
     */
    void clicked(QVector2D physicalPos);

public slots:

    /**
     * @brief Gets whether the given point is in the source space
     *
     * @param Point Given point (mm)
     * @return Whether the given point is in the source space
     */
    bool sourceContains(QVector2D const& point);

    /**
     * @brief Gets the point's coordinates in the source space, with respect to the tile's center
     *
     * Gets (point - tileTopLeft - tileCenter).
     *
     * @param  point Given point (mm)
     * @return Given point's coordinates in the source space, width respect to the tile's center
     */
    QVector2D sourceCoordinates(QVector2D const& point);

private slots:

    /**
     * @brief Sets the appropriate source coordinates according to the standard coordinate description
     */
    void updateFromStdSourceCoords();

    /**
     * @brief If the parent is a HexTileMap, recalculates x, y, width and height accordingly
     */
    void recalculateScreenCoords();

protected:

    /**
     * @brief Called when the user clicks this tile
     *
     * @param event Details of the click
     */
    void mousePressEvent(QMouseEvent* event) override;

private:

    /**
     * @brief Override; updates the screen coordinates if parent is a HexTileMap
     *
     * @param change The change that occurred, must be ItemParentHasChanged
     * @param value Points to the new parent
     */
    void itemChange(ItemChange change, const ItemChangeData& value) override;

    /**
     * @brief Implementation of the GL rendering routine for this hex tile
     *
     * @param  oldNode             Previously returned root node, nullptr if first time
     * @param  updatePaintNodeData UNUSED
     * @return                     The root node modified by the draw operaton
     */
    QSGNode* updatePaintNode(QSGNode* oldNode, UpdatePaintNodeData* updatePaintNodeData) override;

    Cellulo::AxialHexCoords* coords;                  ///< Axial hex tile coordinates
    float sourceLeft;                                 ///< Starting X coordinate of the rectangular source space (mm)
    float sourceTop;                                  ///< Starting Y coordinate of the rectangular source space (mm)
    float sourceRight;                                ///< Ending X coordinate of the rectangular source space (mm)
    float sourceBottom;                               ///< Ending Y coordinate of the rectangular source space (mm)
    float sourceCenterX;                              ///< X coordinate of the center of the hex tile in the source space, with respect to the top/left of the source space (mm)
    float sourceCenterY;                              ///< Y coordinate of the center of the hex tile in the source space, with respect to the top/left of the source space (mm)
    Cellulo::HexTileStdSourceCoords* stdSourceCoords; ///< Standard hex tile coordinate description if available

    QColor fillColor;                                 ///< Fill color of the rendering of this tile
    QColor borderColor;                               ///< Border color of the rendering of this tile
    qreal borderSize;                                 ///< Border size of the rendering with respect to the width
};

/** @} */

}

Q_DECLARE_METATYPE(Cellulo::HexTile*)

#endif // HEXTILE_H
