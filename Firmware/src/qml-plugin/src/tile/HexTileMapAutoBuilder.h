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
 * @file HexTileMapAutoBuilder.h
 * @brief Header for a hexagonal tile autobuilder
 * @author Ayberk Özgür
 * @date 2018-05-18
 */

#ifndef HEXTILEMAPAUTOBUILDER_H
#define HEXTILEMAPAUTOBUILDER_H

#include <QQuickItem>
#include <QVector2D>
#include <QVector3D>

#include "AxialHexCoords.h"
#include "HexTileStdSourceCoords.h"
#include "HexTileMap.h"

namespace Cellulo {

class HexTileMap;

/**
 * @addtogroup tile
 * @{
 */

/**
 * @brief HexTileMap autobuilder, keeps history of poses for one physical/virtual object (e.g robot) and adds unknown tiles to their correct places
 */
class HexTileMapAutoBuilder : public QObject {
    /* *INDENT-OFF* */
    Q_OBJECT
        /* *INDENT-ON* */

public:

    /** @cond DO_NOT_DOCUMENT */

    /**
     * @brief Creates a new HexTileMap autobuilder
     */
    explicit HexTileMapAutoBuilder(QObject* parent = 0);

    /**
     * @brief Deletes this HexTileMap autobuilder
     */
    virtual ~HexTileMapAutoBuilder();

    /** @endcond */

public slots:

    /**
     * @brief Resets autobuild history
     */
    void reset();

    /**
     * @brief Processes a position on an already known tile
     *
     * @param position   Remapped position
     * @param tileCoords Coordinates of the known tile
     */
    void processKnownTile(QVector2D const& position, AxialHexCoords* tileCoords);

    /**
     * @brief Processes a pose on an unknown tile
     *
     * @param  sourcePose Raw source coordinates
     * @param  addNewTile Whether to create and add a new tile to the map if possible
     * @param  mapEmpty   Whether the map is empty, if so, a new tile will be added at 0,0 if possible
     * @param  tileMap    Map to add the new tile to
     * @return            Remapped pose if possible, (0,0,0) otherwise
     */
    QVector3D processUnknownTile(QVector3D const& sourcePose, bool addNewTile, bool mapEmpty, Cellulo::HexTileMap* tileMap = nullptr);

private:

    bool knownCoordsExist;                        ///< Whether there is a reading from any known tile
    AxialHexCoords knownCoords;                   ///< The hex coordinates of the known tile, if any
    QList<QVector2D> knownHistory;                ///< The remapped coordinate history on the known tile

    HexTileStdSourceCoords* unknownStdCoords;     ///< The autodetected standard coordinates of the unknown tile
    QList<QVector2D> unknownHistory;              ///< The raw source coordinate history on the unknown tile

    constexpr static int knownHistorySize = 5;    ///< Maximum number of directions on the known tile to consider
    constexpr static int knownHistoryMinSize = 3; ///< Minimum number of directions on the known tile to consider
    constexpr static int unknownHistorySize = 3;  ///< Number of directions on the unknown tile to consider
    constexpr static float minVecSize = 2.0f;     ///< Min direction length in mm for consideration in unknown tile generation

};

/** @} */

}

Q_DECLARE_METATYPE(Cellulo::HexTileMapAutoBuilder*)

#endif // HEXTILEMAPAUTOBUILDER_H
