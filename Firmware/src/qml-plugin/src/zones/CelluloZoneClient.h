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
 * @file CelluloZoneClient.h
 * @brief Header for the Cellulo zone client
 * @author Ayberk Özgür
 * @date 2016-06-26
 */

#ifndef CELLULOZONECLIENT_H
#define CELLULOZONECLIENT_H

#include <QQuickItem>

namespace Cellulo{

class CelluloZone;

/**
 * @addtogroup zone
 * @{
 */

/**
 * @brief Describes an object with a physical pose (e.g a robot) that may interact with Cellulo zones
 *
 * Abstract object that interacts with `CelluloZone`s through a `CelluloZoneEngine`. Inherit from this object (i.e if
 * being used from QML, create the `YourObject.qml` file with `CelluloZoneClient` as the top object) to have your
 * custom object interact with `CelluloZone`s. For example, `CelluloBluetooth` already inherits from this object.
 *
 * @abstract
 */
class CelluloZoneClient : public QQuickItem {
    /* *INDENT-OFF* */
    Q_OBJECT
    /* *INDENT-ON* */

public:

    /** @cond DO_NOT_DOCUMENT */

    /**
     * @brief Creates a new zone client
     *
     * @param parent Qt parent
     */
    explicit CelluloZoneClient(QQuickItem* parent = 0);

    /** @endcond */

signals:

    /**
     * @brief Emitted when a zone's value with respect to this object changes
     *
     * @param zone Zone whose value has changed
     * @param value The new value
     */
    void zoneValueChanged(QObject* zone, qreal value);

    /**
     * @brief User is responsible for emitting this signal when this client's pose (in mm, mm, deg) changes
     *
     * @param x New x coordinate in mm
     * @param y New y coordinate in mm
     * @param theta New orientation in degrees
     */
    void poseChanged(qreal x, qreal y, qreal theta);

};

/** @} */

}

#endif //CELLULOZONECLIENT_H
