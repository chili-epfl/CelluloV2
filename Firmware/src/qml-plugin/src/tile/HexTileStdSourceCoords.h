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
 * @brief Header for a hexagonal tile standard source coordinates definition
 * @author Ayberk Özgür
 * @date 2018-05-02
 */

#ifndef HEXTILESTDSOURCECOORDS_H
#define HEXTILESTDSOURCECOORDS_H

#include <QQuickItem>

#include "HexTile.h"

namespace Cellulo {

class HexTile;

/**
 * @addtogroup tile
 * @{
 */

/**
 * @brief Hexagonal tile standard coordinates definition
 *
 * Each sheet has integer positive coordinates i,j whose coordinate space starts at [i*210, j*260] millimeters.
 *
 * On each sheet there are 4 tiles who have tile coordinates u,v that are either [0,0] (top left), [1,0] (top right), [0,1] (bottom left) or [1,1] (bottom right).
 * These tiles have the following coordinate spaces:
 *
 * Top left tile (u,v=0,0):
 *     Space left right: [0.0, 105.7] mm
 *     Space top bottom: [0.0, 130.0] mm
 *     Center x y: [54.3, 67.0] mm
 *
 * Top right tile (u,v=1,0):
 *     Space left right: [105.7, 210.0] mm
 *     Space top bottom: [0.0, 130.0] mm
 *     Center x y: [50.0, 67.0] mm
 *
 * Bottom left tile (u,v=0,1):
 *     Space left right: [0.0, 105.7] mm
 *     Space top bottom: [130.0, 260.0] mm
 *     Center x y: [54.3, 67.0] mm
 *
 * Bottom right tile (u,v=1,1):
 *     Space left right: [105.7, 210.0] mm
 *     Space top bottom: [130.0, 260.0] mm
 *     Center x y: [50.0, 67.0] mm
 */
class HexTileStdSourceCoords : public QQuickItem {
    /* *INDENT-OFF* */
    Q_OBJECT
    /* *INDENT-ON* */

    /** @brief Sheet horizontal index, >= 0 */
    Q_PROPERTY(int i WRITE setI READ getI NOTIFY iChanged)

    /** @brief Sheet vertical index, >= 0 */
    Q_PROPERTY(int j WRITE setJ READ getJ NOTIFY jChanged)

    /** @brief Horizontal tile index within sheet, either 0 or 1 */
    Q_PROPERTY(int u WRITE setU READ getU NOTIFY uChanged)

    /** @brief Vertical tile index within sheet, either 0 or 1 */
    Q_PROPERTY(int v WRITE setV READ getV NOTIFY vChanged)

public:

    /** @cond DO_NOT_DOCUMENT */

    /**
     * @brief Creates a new hex tile standard coordinate definition
     */
    explicit HexTileStdSourceCoords(QQuickItem* parent = 0);

    /**
     * @brief Deletes this hex tile standard coordinate definition
     */
    virtual ~HexTileStdSourceCoords();

    /**
     * @brief Gets i
     *
     * @return i
     */
    int getI() const { return i; }

    /**
     * @brief Gets j
     *
     * @return j
     */
    int getJ() const { return j; }

    /**
     * @brief Gets u
     *
     * @return u
     */
    int getU() const { return u; }

    /**
     * @brief Gets v
     *
     * @return v
     */
    int getV() const { return v; }

    /**
     * @brief Sets a new i
     *
     * @param i New i
     */
    void setI(int i);

    /**
     * @brief Sets a new j
     *
     * @param j New j
     */
    void setJ(int j);

    /**
     * @brief Sets a new u
     *
     * @param u New u
     */
    void setU(int u);

    /**
     * @brief Sets a new v
     *
     * @param v New v
     */
    void setV(int v);

    /** @endcond */

signals:

    /** @cond DO_NOT_DOCUMENT */

    /**
     * @brief Emitted when i changes
     */
    void iChanged();

    /**
     * @brief Emitted when j changes
     */
    void jChanged();

    /**
     * @brief Emitted when u changes
     */
    void uChanged();

    /**
     * @brief Emitted wjhen v changes
     */
    void vChanged();

    /** @endcond */

public slots:

    /**
     * @brief Connects the changed signals of i,j,u,v to the changed signal of tile's standard coords
     *
     * @param tile Tile owning this standard coords
     */
    void connectHexTileChangedSignals(Cellulo::HexTile* tile);

    /**
     * @brief Disconnects the changed signals of i,j,u,v from the changed signal of tile's standard coords
     *
     * @param tile Tile owning this standard coords
     */
    void disconnectHexTileChangedSignals(Cellulo::HexTile* tile);

    /**
     * @brief Attempts to estimate i,j,u,v from given raw physical coordinates
     *
     * @param coords Physical raw coordinates in mm
     * @return Whether the estimate was successful
     */
    bool estimateFromCoords(QVector2D const& coords);

    /**
     * @brief Compares these coords to another
     *
     * @param  other Other coords
     * @return       Whether these coords are equal to the other
     */
    bool equals(HexTileStdSourceCoords const& other) const;

private:

    int i; ///< Horizontal sheet index
    int j; ///< Vertical sheel index
    int u; ///< Horizontal tile index
    int v; ///< Vertical tile index

};

/** @} */

}

Q_DECLARE_METATYPE(Cellulo::HexTileStdSourceCoords*)

#endif // HEXTILESTDSOURCECOORDS_H
