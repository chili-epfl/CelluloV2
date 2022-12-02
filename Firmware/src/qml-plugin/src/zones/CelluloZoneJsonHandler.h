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
 * @file CelluloZoneJsonHandler.h
 * @brief Header for zone JSON tools
 * @author Joanna Salathé
 * @date 2016-03-04
 */

#ifndef CELLULOZONEJSONHANDLER_H
#define CELLULOZONEJSONHANDLER_H

#include <QObject>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QVariant>

#include "CelluloZone.h"

namespace Cellulo{

/**
 * @addtogroup zone
 * @{
 */

/**
 * @brief Utilities to load/save `CelluloZone`s to to/from files in JSON format.
 * @singleton
 */
class CelluloZoneJsonHandler : public QObject {
    /* *INDENT-OFF* */
    Q_OBJECT
    /* *INDENT-ON* */

public:

    /** @cond DO_NOT_DOCUMENT */

    CelluloZoneJsonHandler(QObject* parent = 0);
    ~CelluloZoneJsonHandler();

    /**
     * @brief Save zones in JSON file
     *
     * @param zones Zones to be saved
     * @param path Path of the JSON file where to save the zones
     */
    static void saveZones(const QList<Cellulo::CelluloZone*>& zones, QString path);

    /**
     * @brief Load zones from JSON file
     *
     * @param Path path of the JSON file where to load the zones
     * @return List of zones
     */
    static QList<Cellulo::CelluloZone*> loadZonesCPP(QString path);

    /** @endcond */

    /**
     * @brief Save zones in JSON file
     *
     * @param zones Zones to be saved (QML-compatible)
     * @param path Path of the JSON file where to save the zones
     */
    Q_INVOKABLE static void saveZones(const QVariantList& zones, QString path);

    /**
     * @brief Load zones from JSON file
     *
     * @param path Path of the JSON file where to load the zones
     * @return List of zones (QML-compatible)
     */
    Q_INVOKABLE static QVariantList loadZonesQML(QString path);

private:

    /**
     * @brief Writes all given zones to the given Json object
     *
     * @param json Object to write to
     * @param zones Zones to serialize
     */
    static void write(QJsonArray& json, QList<Cellulo::CelluloZone*> zones);

    /**
     * @brief Retrieves zones from Json object and creates them
     *
     * @param json Object to read from
     * @return Retrieved zones
     */
    static QList<Cellulo::CelluloZone*> read(const QJsonArray& json);

};

/** @} */

}

#endif // CELLULOZONEJSONHANDLER_H
