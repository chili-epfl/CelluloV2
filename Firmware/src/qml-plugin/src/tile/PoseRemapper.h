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
 * @file PoseRemapper.h
 * @brief Header for pose remapper base
 * @author Ayberk Özgür
 * @date 2018-04-27
 */

#ifndef POSEREMAPPER_H
#define POSEREMAPPER_H

#include<QQuickItem>
#include<QVector3D>

namespace Cellulo{

/**
 * @addtogroup tile
 * @{
 */

/**
 * @brief Abstract object that should remap given poses to another poses based on some rule to be determined by the extending classes
 *
 * @abstract
 */
class PoseRemapper : public QQuickItem {
    /* *INDENT-OFF* */
    Q_OBJECT
    /* *INDENT-ON* */

public:

    /** @cond DO_NOT_DOCUMENT */

    /**
     * @brief Creates a new pose remapper
     */
    explicit PoseRemapper(QQuickItem* parent = 0);

    /**
     * @brief Deletes this remapper
     */
    virtual ~PoseRemapper();

    /** @endcond */

public slots:

    /**
     * @brief Remaps the given pose to another pose based on some rule to be determined by the extending classes
     *
     * @param  pose Given pose (x,y is in mm, z is orientation in degrees)
     * @param  sender Object whose pose is being remapped, used for detecting e.g kidnaps and improving the remapping if provided
     * @return Remapped pose (x,y is in mm, z is orientation in degrees)
     */
    virtual QVector3D remapPose(QVector3D const& pose, QObject* sender = nullptr) = 0;

};

/** @} */

}

Q_DECLARE_METATYPE(Cellulo::PoseRemapper*)

#endif // POSEREMAPPER_H
