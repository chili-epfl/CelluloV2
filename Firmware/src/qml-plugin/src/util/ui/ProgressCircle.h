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
 * @file ProgressCircle.h
 * @brief Header for a circular progress indicator
 * @author Ayberk Özgür
 * @date 2018-05-25
 */

#ifndef PROGRESSCIRCLE_H
#define PROGRESSCIRCLE_H

#include <QQuickItem>

namespace Cellulo {

/**
 * @addtogroup util
 * @{
 */

/**
 * @brief A circular progress indicator
 */
class ProgressCircle : public QQuickItem {
    /* *INDENT-OFF* */
    Q_OBJECT
        /* *INDENT-ON* */

    /** @brief Progress, between 0.0 and 1.0, by default 0.0 */
    Q_PROPERTY(qreal progress MEMBER progress NOTIFY progressChanged)

    /** @brief The foreground color, light blue by default */
    Q_PROPERTY(QColor color MEMBER color NOTIFY colorChanged)

    /** @brief The background color, light grey by default */
    Q_PROPERTY(QColor backgroundColor MEMBER backgroundColor NOTIFY backgroundColorChanged)

    /** @brief Thickness of the progress arc with respect to the radius, between 0.0 and 1.0, by default 0.3 */
    Q_PROPERTY(qreal thickness MEMBER thickness NOTIFY thicknessChanged)

public:

    /** @cond DO_NOT_DOCUMENT */

    /**
     * @brief Creates a new progress circle
     */
    explicit ProgressCircle(QQuickItem* parent = 0);

    /**
     * @brief Deletes this progress circle
     */
    virtual ~ProgressCircle();

    /** @endcond */

signals:

    /** @cond DO_NOT_DOCUMENT */

    /**
     * @brief Emitted when the progress changes
     */
    void progressChanged();

    /**
     * @brief Emitted when the progress changes
     */
    void colorChanged();

    /**
     * @brief Emitted when the progress changes
     */
    void backgroundColorChanged();

    /**
     * @brief Emitted when the progress changes
     */
    void thicknessChanged();

    /** @endcond */

private:

    /**
     * @brief Implementation of the GL rendering routine for this hex tile
     *
     * @param  oldNode             Previously returned root node, nullptr if first time
     * @param  updatePaintNodeData UNUSED
     * @return                     The root node modified by the draw operaton
     */
    QSGNode* updatePaintNode(QSGNode* oldNode, UpdatePaintNodeData* updatePaintNodeData) override;

    qreal progress;  ///< Progress
    QColor color; ///< Foreground color
    QColor backgroundColor; ///< Background color
    qreal thickness; ///< Arc thickness w.r.t whole radius

    constexpr static int RESOLUTION = 64;   ///< Number of segments in rendering

};

/** @} */

}

#endif // PROGRESSCIRCLE_H
