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
 * @file OffsetRemapper.cpp
 * @brief Source for pose remapper base
 * @author Ayberk Özgür
 * @date 2018-04-27
 */

#include "OffsetRemapper.h"

#include<QDebug>

namespace Cellulo{

OffsetRemapper::OffsetRemapper(QQuickItem* parent) : PoseRemapper(parent){
    deltaX = 0;
    deltaY = 0;
    deltaTheta = 0;
}

OffsetRemapper::~OffsetRemapper(){}

QVector3D OffsetRemapper::remapPose(QVector3D const& pose, QObject* sender){
    Q_UNUSED(sender);
    
    qreal newTheta = pose.z() + deltaTheta;
    while(newTheta >= 360.0f)
        newTheta -= 360.0f;
    while(newTheta < 0.0f)
        newTheta += 360.0f;
    return QVector3D(pose.x() + deltaX, pose.y() + deltaY, newTheta);
}

}
