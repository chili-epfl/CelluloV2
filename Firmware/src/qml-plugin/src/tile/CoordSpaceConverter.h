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
 * @file CoordSpaceConverter.h
 * @brief Header for any space to any space converter
 * @author Ayberk Özgür
 * @date 2018-05-14
 */

#ifndef COORDSPACECONVERTER_H
#define COORDSPACECONVERTER_H

#include <QQuickItem>

namespace Cellulo {

/**
 * @addtogroup tile
 * @{
 */

/**
 * @brief Class that converts given coordinates in an input space to given coordinates in an output space
 */
class CoordSpaceConverter : public QQuickItem {
    /* *INDENT-OFF* */
    Q_OBJECT
    /* *INDENT-ON* */

    /** @brief The origin coordinates of the input space, in the input space units (could be mm) */
    Q_PROPERTY(QVector2D inputOrigin READ getInputOrigin WRITE setInputOrigin NOTIFY inputOriginChanged)

    /** @brief The size of the input space, must be positive, in the input space units (could be mm) */
    Q_PROPERTY(QVector2D inputSize READ getInputSize WRITE setInputSize NOTIFY inputSizeChanged)

    /** @brief The origin coordinates of the output space, in the output space units (could be pixels) */
    Q_PROPERTY(QVector2D outputOrigin READ getOutputOrigin WRITE setOutputOrigin NOTIFY outputOriginChanged)

    /** @brief The size of the output space, must be positive, in the output space units (could be pixels) */
    Q_PROPERTY(QVector2D outputSize READ getOutputSize WRITE setOutputSize NOTIFY outputSizeChanged)

public:

    /** @cond DO_NOT_DOCUMENT */

    /**
     * @brief Creates a new any space to any space converter
     */
    explicit CoordSpaceConverter(QQuickItem* parent = 0);

    /**
     * @brief Deletes this any space to any space converter
     */
    virtual ~CoordSpaceConverter();

    /**
     * @brief Gets the input space origin
     *
     * @return Input space origin
     */
    QVector2D getInputOrigin() const { return inputOrigin; }

    /**
     * @brief Gets the input space size
     *
     * @return Input space size
     */
    QVector2D getInputSize() const { return inputSize; }

    /**
     * @brief Gets the output space origin
     *
     * @return Output space origin
     */
    QVector2D getOutputOrigin() const { return outputOrigin; }

    /**
     * @brief Gets the output space size
     *
     * @return Output space size
     */
    QVector2D getOutputSize() const { return outputSize; }

    /**
     * @brief Sets the input space origin
     *
     * @param inputOrigin Input space origin
     */
    void setInputOrigin(QVector2D const& inputOrigin);

    /**
     * @brief Sets the input space size
     *
     * @param inputSize Input space size
     */
    void setInputSize(QVector2D const& inputSize);

    /**
     * @brief Sets the output space origin
     *
     * @param outputOrigin Output space origin
     */
    void setOutputOrigin(QVector2D const& outputOrigin);

    /**
     * @brief Sets the output space size
     *
     * @param outputSize Output space size
     */
    void setOutputSize(QVector2D const& outputSize);

    /** @endcond */

signals:

    /** @cond DO_NOT_DOCUMENT */

    /**
     * @brief Emitted when the input space origin changes
     */
    void inputOriginChanged();

    /**
     * @brief Emitted when the input space size changes
     */
    void inputSizeChanged();

    /**
     * @brief Emitted when the output space origin changes
     */
    void outputOriginChanged();

    /**
     * @brief Emitted when the output space size changes
     */
    void outputSizeChanged();

    /** @endcond */

public slots:

    /**
     * @brief Converts the given input coordinates to output coordinates
     *
     * Returns (input - inputOrigin)/inputSize*outputSize + outputOrigin
     *
     * @param  input Input coordinates
     * @return       Output coordinates
     */
    QVector2D convert(QVector2D const& input) const;

private:

    QVector2D inputOrigin;   ///< Origin coordinates of the input space
    QVector2D inputSize;     ///< Size of the input space
    QVector2D outputOrigin;  ///< Origin coordinates of the input space
    QVector2D outputSize;    ///< Size of the input space

};

/** @} */

}

Q_DECLARE_METATYPE(Cellulo::CoordSpaceConverter*)

#endif // COORDSPACECONVERTER_H
