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
 * @file CelluloUIUtil.h
 * @brief Source for user interface utilities
 * @author Ayberk Özgür
 * @date 2017-04-21
 */

#include "CelluloUIUtil.h"

namespace Cellulo{

CelluloUIUtil::CelluloUIUtil(QObject* parent) : QObject(parent){}

CelluloUIUtil::~CelluloUIUtil(){}

QColor CelluloUIUtil::colorFromString(QString const& color){
    return QColor(color);
}

QColor CelluloUIUtil::colorInterpolate(QColor const& color1, QColor const& color2, qreal color1Weight){
    return QColor(
        (int)(color1Weight*color1.red() +   (1 - color1Weight)*color2.red()),
        (int)(color1Weight*color1.green() + (1 - color1Weight)*color2.green()),
        (int)(color1Weight*color1.blue() +  (1 - color1Weight)*color2.blue())
    );
}

}
