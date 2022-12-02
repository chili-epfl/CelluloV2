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
 * @file CelluloZoneEngine.h
 * @brief Header for the zone engine
 * @author Joanna Salathé
 * @date 2016-03-04
 */

#ifndef CELLULOZONEENGINE_H
#define CELLULOZONEENGINE_H

#include <QQuickItem>
#include <QQmlComponent>
#include <QQmlEngine>
#include <QQmlContext>
#include <QChildEvent>
#include <QVariantList>

#include "CelluloZoneCircle.h"
#include "CelluloZoneRectangle.h"
#include "CelluloZoneLine.h"
#include "CelluloZonePoint.h"
#include "CelluloZonePolygon.h"
#include "CelluloZoneJsonHandler.h"

namespace Cellulo{

/**
 * @addtogroup zone
 * @{
 */

/**
 * @brief Object that ensures `CelluloZone` and `CelluloZoneClient` interaction.
 *
 * All `CelluloZoneClient`s in this engine have their `zoneValueChanged()` signals emitted when the value of a
 * `CelluloZone` (within this engine) with respect to the client changes.
 *
 * `CelluloZone`s instantiated as children of a `CelluloZoneEngine` will be automatically added to it.
 * `CelluloZoneClient`s must be manually added.
 */
class CelluloZoneEngine : public QQuickItem {
    /* *INDENT-OFF* */
    Q_OBJECT
    /* *INDENT-ON* */

    /** @brief Whether the zones in this engine are active, default true */
    Q_PROPERTY(bool active READ getActive WRITE setActive NOTIFY activeChanged)

public:

    /** @cond DO_NOT_DOCUMENT */

    /**
     * @brief Creates a new Cellulo zone engine
     *
     * @param parent Parent of this QML item
     */
    explicit CelluloZoneEngine(QQuickItem* parent = 0);

    /**
     * @brief Destroys this Cellulo robot communicator
     */
    virtual ~CelluloZoneEngine();

    /**
     * @brief Adds the given list of zones to the already existing zones
     *
     * @param newZones New zones
     */
    void addNewZones(QList<Cellulo::CelluloZone*> newZones);

    /** @endcond */

    /**
     * @brief Get list of names of all zones handled by this engine
     *
     * @return Names of the zones handled by this engine
     */
    Q_INVOKABLE QStringList getAllZoneNames();

    /**
     * @brief Get zone handled by this engine having the corresponding name
     *
     * @param name Lookup name for the zone
     * @return First zone handled by this engine to have this name or NULL if not found
     */
    Q_INVOKABLE QObject* getZoneByName(QString name);

    /**
     * @brief Returns the list of all zones in this engine
     *
     * @return The list of all zones
     */
    Q_INVOKABLE QVariantList getZonesList();

    /**
     * @brief Adds the given list of zones to the already existing zones
     *
     * @param newZones New zones (QML-compatible)
     */
    Q_INVOKABLE void addNewZones(QVariantList newZones);

public slots:

    /** @cond DO_NOT_DOCUMENT */

    /**
     * @brief Gets whether currently active
     *
     * @return Whether currently active
     */
    bool getActive(){ return active; }

    /**
     * @brief Sets whether the engine is active
     *
     * @param newActive Whether the engine should be active
     */
    void setActive(bool newActive);

    /** @endcond */

    /**
     * @brief Adds a new client to the engine, binds to all existing zones
     *
     * @param newClient Client to add
     */
    void addNewClient(Cellulo::CelluloZoneClient* newClient);

    /**
     * @brief Adds a new zone to the engine, binds to all existing clients
     *
     * @param newZone Zone to add
     */
    void addNewZone(Cellulo::CelluloZone* newZone);

    /**
     * @brief Deletes all zones
     */
    void clearZones();

signals:

    /** @cond DO_NOT_DOCUMENT */

    /**
     * @brief Emitted when active changes
     */
    void activeChanged();

    /** @endcond */

private slots:

    /**
     * @brief Called when a bound zone's active status changes
     */
    void zoneActiveChanged();

private:

    bool active;                                ///< Whether clients' poseChanged events are connected to zones; true by default
    QSet<Cellulo::CelluloZoneClient*> clients;  ///< All clients bound to this engine
    QSet<Cellulo::CelluloZone*> zones;          ///< All zones bound to this engine

    /**
     * @brief Binds client to zone
     *
     * Connects Client's poseChanged(real,real,real) signal to Zone's onClientPoseChanged(real,real,real) slot.
     * Zone invokes the zoneValueChanged(zone*,real) signal of the appropriate Client automatically.
     *
     * @param client Client to be bound
     * @param zone Zone to be bound
     */
    void bindClientToZone(CelluloZoneClient* client, CelluloZone* zone);

    /**
     * @brief Unbinds client from zone
     *
     * Disconnects Client's poseChanged(real,real,real) signal from Zone's onClientPoseChanged(real,real,real) slot.
     *
     * @param client Client to be unbound
     * @param zone Zone to be unbound
     */
    void unbindClientFromZone(CelluloZoneClient* client, CelluloZone* zone);

    /**
     * @brief Override; adds the added child to zones if it is a CelluloZone
     *
     * @param change The change that occurred, must be ItemChildAddedChange
     * @param value Points to the added child when change is ItemChildAddedChange
     */
    void itemChange(ItemChange change, const ItemChangeData& value) override;

};

/** @} */

}

Q_DECLARE_METATYPE(QList<Cellulo::CelluloZone*>)

#endif // CELLULOZONEENGINE_H
