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
 * @file HexTileMap.h
 * @brief Header for a hexagonal multi-tile map
 * @author Ayberk Özgür
 * @date 2018-04-27
 */

#ifndef HEXTILEMAP_H
#define HEXTILEMAP_H

#include <QVector3D>
#include <QVariantList>
#include <QRectF>
#include <QQmlComponent>

#include "PoseRemapper.h"
#include "CoordSpaceConverter.h"
#include "../util/math/CelluloMathUtil.h"
#include "HexTileMapAutoBuilder.h"

namespace Cellulo {

class HexTileMapAutoBuilder;

/**
 * @addtogroup tile
 * @{
 */

/**
 * @brief Map built from hex tiles
 */
class HexTileMap : public PoseRemapper {
    /* *INDENT-OFF* */
    Q_OBJECT
    /* *INDENT-ON* */

    /** @brief Automatically determine physicalTopLeft and physicalSize depending on the owned tiles, default true */
    Q_PROPERTY(bool autoResize READ getAutoResize WRITE setAutoResize NOTIFY autoResizeChanged)

    /** @brief The physical coordinates of the top left of this map, in mm */
    Q_PROPERTY(QVector2D physicalTopLeft READ getPhysicalTopLeft WRITE setPhysicalTopLeft NOTIFY physicalTopLeftChanged)

    /** @brief The physical size of this map, in mm */
    Q_PROPERTY(QVector2D physicalSize READ getPhysicalSize WRITE setPhysicalSize NOTIFY physicalSizeChanged)

    /** @brief Converter from physical sizes (mm) to screen sizes (pixels) */
    Q_PROPERTY(Cellulo::CoordSpaceConverter* toScreenSize READ getToScreenSize NOTIFY toScreenSizeChanged)

    /** @brief Converter from physical coords (mm) to screen coords (pixels) */
    Q_PROPERTY(Cellulo::CoordSpaceConverter* toScreenCoords READ getToScreenCoords NOTIFY toScreenCoordsChanged)

    /** @brief Try to build map automatically with standard coordinates through robot readings, i.e auto-add unknown tiles, default false */
    Q_PROPERTY(bool autoBuild READ getAutoBuild WRITE setAutoBuild NOTIFY autoBuildChanged)

    /** @brief If set, tiles will be created out of this component instead of the standard HexTile, default null */
    Q_PROPERTY(QQmlComponent* tileComponent READ getTileComponent WRITE setTileComponent NOTIFY tileComponentChanged)

    //TODO: BETTER STORAGE
    Q_PROPERTY(QVariantList tiles MEMBER tiles)

public:

    /** @cond DO_NOT_DOCUMENT */

    /**
     * @brief Creates a new hex tile map
     */
    explicit HexTileMap(QQuickItem* parent = 0);

    /**
     * @brief Deletes this hex tile map
     */
    virtual ~HexTileMap();

    /**
     * @brief Gets whether to automatically resize physicalSize and physicalTopLeft
     *
     * @return Whether to automatically resize physicalSize and physicalTopLeft
     */
    bool getAutoResize() const { return autoResize; }

    /**
     * @brief Sets whether to automatically resize physicalSize and physicalTopLeft
     *
     * @param autoResize Whether to automatically resize physicalSize and physicalTopLeft
     */
    void setAutoResize(bool autoResize);

    /**
     * @brief Gets the physical size, used for drawing
     *
     * @return The physical size described by this map
     */
    QVector2D getPhysicalSize() const { return physicalSize; }

    /**
     * @brief Sets the new physical area, must have positive width/height
     *
     * @param physicalArea New physical area described by this map, used for drawing
     */
    void setPhysicalSize(QVector2D const& physicalArea);

    /**
     * @brief Gets the physical top left coordinate, used for drawing
     *
     * @return The physical top left coordinate described by this map
     */
    QVector2D getPhysicalTopLeft() const { return physicalTopLeft; }

    /**
     * @brief Sets the new physical top left coordinate
     *
     * @param physicalTopLeft New physical top left coordinate of this map, used for drawing
     */
    void setPhysicalTopLeft(QVector2D const& physicalTopLeft);

    /**
     * @brief Gets the converter from physical sizes to screen sizes
     *
     * @return Converter from physical sizes to screen sizes
     */
    Cellulo::CoordSpaceConverter* getToScreenSize(){ return &toScreenSize; }

    /**
     * @brief Gets the converter from physical coords to screen coords
     *
     * @return Converter from physical coords to screen coords
     */
    Cellulo::CoordSpaceConverter* getToScreenCoords(){ return &toScreenCoords; }

    /**
     * @brief Gets whether the map will try to automatically build according to standard coordinate hex tiles
     *
     * @return Whether the map will try to automatically build according to standard coordinate hex tiles
     */
    bool getAutoBuild() const { return autoBuild; }

    /**
     * @brief Sets whether the map will try to automatically build according to standard coordinate hex tiles
     *
     * @param autoBuild Whether the map will try to automatically build according to standard coordinate hex tiles
     */
    void setAutoBuild(bool autoBuild);

    /**
     * @brief Gets the tile component that creates new tiles
     *
     * @return Current tile component
     */
    QQmlComponent* getTileComponent(){ return tileComponent; }

    /**
     * @brief Sets the tile component that creates new tiles
     *
     * @param tileComponent New tile component
     */
    void setTileComponent(QQmlComponent* tileComponent);

    /** @endcond */

signals:

    /** @cond DO_NOT_DOCUMENT */

    /**
     * @brief Emitted when autoResize changes
     */
    void autoResizeChanged();

    /**
     * @brief Emitted when the physical size changes
     */
    void physicalSizeChanged();

    /**
     * @brief Emitted when the physical top left coordinate changes
     */
    void physicalTopLeftChanged();

    /**
     * @brief Emitted when toScreenSize changes
     */
    void toScreenSizeChanged();

    /**
     * @brief Emitted when toScreenCoords changes
     */
    void toScreenCoordsChanged();

    /**
     * @brief Emitted when autoBuild changes
     */
    void autoBuildChanged();

    /**
     * @brief Emitted when the tile component changes
     */
    void tileComponentChanged();

    /** @endcond */

    /**
     * @brief Emitted when screen children should redraw
     */
    void markedDirty();

    /**
     * @brief Emitted when a new tile is added
     *
     * @param newTile New tile
     */
    void tileAdded(HexTile* newTile);

    /**
     * @brief Emitted when a tile is removed
     *
     * @param oldTileQ Horizontal coordinate of the removed tile
     * @param oldTileR Vertical coordinate of the removed tile
     */
    void tileRemoved(int oldTileQ, int oldTileR);

    /**
     * @brief Emitted when a tile is clicked
     *
     * @param tile             Tile that was clicked
     * @param physicalPosition Physical position in mm corresponding to the screen click
     */
    void tileClicked(Cellulo::HexTile* tile, QVector2D physicalPosition);

public slots:

    /**
     * @brief Remaps the given pose to another pose based on the hex tile configuration
     *
     * @param  pose Given pose (x,y is in mm, z is orientation in degrees)
     * @param  sender Object whose pose is being remapped, used for detecting e.g kidnaps and improving the remapping if provided
     * @return Remapped pose (x,y is in mm, z is orientation in degrees)
     */
    virtual QVector3D remapPose(QVector3D const& pose, QObject* sender = nullptr) override;

    /**
     * @brief Adds new tile, removes old tile if one with same q,r coordinates is present
     *
     * @param newTile New tile
     */
    void addTile(Cellulo::HexTile* newTile);

    /**
     * @brief Tile lookup by axial coordinates
     *
     * @param  q Horizontal coordinate
     * @param  r Vertical coordinate
     * @return   Tile if found, nullptr otherwise
     */
    Cellulo::HexTile* getTile(int q, int r);

    /**
     * @brief Tile lookup by output coordinates
     *
     * @param  position Output coordinates (i.e regular coordinates in the hex tile map space and not source coordinates) in mm
     * @return Tile if found, nullptr otherwise
     */
    Cellulo::HexTile* getTile(QVector2D const& position);

    /**
     * @brief Removes the given tile
     *
     * @param oldTile Tile to remove
     * @return Whether the tile was there and was removed
     */
    bool removeTile(Cellulo::HexTile* oldTile);

    /**
     * @brief Removes the given tile
     *
     * @param q Horizontal axial coordinate
     * @param r Vertical axial coordinate
     * @return Whether the tile was there and was removed
     */
    bool removeTile(int q, int r);

    /**
     * @brief Removes all tiles
     */
    void clearTiles();

    /**
     * @brief Dumps all tiles to a JSON file
     *
     * @param filename Full path to a file
     */
    void dumpTilesToJSON(QString const& filename);

    /**
     * @brief Loads tiles from a JSON file
     *
     * @param filename Full path to file
     */
    void loadTilesFromJSON(QString const& filename);

private slots:

    /**
     * @brief Updates the internal coordinates of toScreenSize according to this map
     */
    void updateToScreenSize();

    /**
     * @brief Updates the internal coordinates of toScreenCoords according to this map
     */
    void updateToScreenCoords();

    /**
     * @brief Calculates physicalSize and physicalTopLeft to tiles
     */
    void fitPhysicalCoordsToTiles();

    /**
     * @brief Emits necessary signals upon tile click
     *
     * @param physicalPos Physical position in mm corresponding to the screen click
     */
    void processTileClick(QVector2D physicalPos);

private:

    /**
     * @brief Override; adds the added child to tiles if it is a HexTile
     *
     * @param change The change that occurred, must be ItemChildAddedChange
     * @param value Points to the added child when change is ItemChildAddedChange
     */
    void itemChange(ItemChange change, const ItemChangeData& value) override;

    /**
     * @brief Tile lookup by raw source coordinates
     *
     * @param  position Raw source coordinates in mm
     * @return Tile if found, nullptr otherwise
     */
    Cellulo::HexTile* getTileBySourceCoords(QVector2D const& position);

    /**
     * @brief Reset all auto builders
     */
    void resetAutoBuilders();

    bool autoResize;                                      ///< Determine physicalTopLeft and physicalSize depending on the tiles
    QVector2D physicalTopLeft;                            ///< Physical top left coordinate of this map in mm, used when drawing
    QVector2D physicalSize;                               ///< Physical size described by this map in mm, used when drawing
    CoordSpaceConverter toScreenSize;                     ///< Converter from physical sizes to screen sizes
    CoordSpaceConverter toScreenCoords;                   ///< Converter from physical coords to screen coords

    bool autoBuild;                                       ///< Whether to try to automatically build the map
    QHash<QObject*, HexTileMapAutoBuilder*> autoBuilders; ///< One autobuilder per pose generator (e.g robot)

    QQmlComponent* tileComponent;                         ///< Component to create tiles out of

    //TODO: BETTER STORAGE
    QVariantList tiles;

};

/** @} */

}

#endif // HEXTILEMAP_H
