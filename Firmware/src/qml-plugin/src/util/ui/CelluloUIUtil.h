/*
 * Copyright (C) 2017 EPFL
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
 * @file CelluloUIUtil.h
 * @brief Header for user interface utilities
 * @author Ayberk Özgür
 * @date 2017-04-21
 */

#ifndef CELLULOUIUTIL_H
#define CELLULOUIUTIL_H

#include<QObject>
#include<QColor>
#include<QString>

namespace Cellulo{

/**
 * @addtogroup util
 * @{
 */

/**
 * @brief User interface utilities used throughout Celullo activities
 * @singleton
 */
class CelluloUIUtil : public QObject {
    /* *INDENT-OFF* */
    Q_OBJECT
    /* *INDENT-ON* */

public:

    /** @cond DO_NOT_DOCUMENT */

    CelluloUIUtil(QObject* parent = 0);
    ~CelluloUIUtil();

    /** @endcond */

    /**
     * @brief Converts string representation of color to actual color
     *
     * @param color String representation of color
     * @return Color object
     */
    Q_INVOKABLE static QColor colorFromString(const QString& color);

    /**
     * @brief Interpolates two colors
     *
     * @param color1 First color
     * @param color2 Second color
     * @param color1Weight Weight of the first color in [0.0,1.0]; second color's weight will be (1 - color1Weight)
     * @return Interpolated color
     */
    Q_INVOKABLE static QColor colorInterpolate(const QColor& color1, const QColor& color2, qreal color1Weight);

};

/** @} */

}

#endif // CELLULOUIUTIL_H
