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
 * @file CelluloBluetoothScanner.h
 * @brief Scans for Cellulo robots
 * @author Ayberk Özgür
 * @date 2017-03-16
 */

#ifndef CELLULOBLUETOOTHSCANNER_H
#define CELLULOBLUETOOTHSCANNER_H

#include <QQuickItem>
#include <QBluetoothDeviceDiscoveryAgent>

namespace Cellulo{

/**
 * @addtogroup comm
 * @{
 */

/**
 * @brief Object that scans for Cellulo robots
 *
 * **Warning:** On WinRT, the robots must be first paired from the OS Bluetooth scanner in order to appear in the scan.
 */
class CelluloBluetoothScanner : public QQuickItem {
    /* *INDENT-OFF* */
    Q_OBJECT
    /* *INDENT-ON* */

    /** @brief Whether to scan continuously, default false */
    Q_PROPERTY(bool continuous MEMBER continuous)

    /** @brief Whether scanning now, read-only */
    Q_PROPERTY(bool scanning READ isScanning NOTIFY scanningChanged)

    /** @brief List of found robots, read-only */
    Q_PROPERTY(QStringList foundRobots READ getFoundRobots NOTIFY foundRobotsChanged)

public:

    /** @cond DO_NOT_DOCUMENT */

    /**
     * @brief Creates a new Cellulo robot scanner
     *
     * @param parent Parent of this QML item
     */
    CelluloBluetoothScanner(QQuickItem* parent = 0);

    /**
     * @brief Destroys this Cellulo robot scanner
     */
    virtual ~CelluloBluetoothScanner();

    /**
     * @brief Gets whether currently scanning
     *
     * @return Whether currently scanning
     */
    bool isScanning() const;

    /**
     * @brief Gets the list of found robot MAC addresses
     *
     * @return The list of found robot MAC addresses
     */
    QStringList getFoundRobots() const { return foundRobots; }

    /** @endcond */

signals:

    /**
     * @brief Emitted when a new Cellulo robot is discovered
     *
     * @param macAddr Mac address of the newly discovered robot
     */
    void robotDiscovered(QString const& macAddr,QString const& robotNb);

    /**
     * @brief Emitted when scanning finishes
     */
    void finished();

    /** @cond DO_NOT_DOCUMENT */

    /**
     * @brief Emitted when scan stops or starts
     */
    void scanningChanged();

    /**
     * @brief Emitted when list of found robot MAC addresses changes
     */
    void foundRobotsChanged();

    /** @endcond */

public slots:

    /**
     * @brief Starts scanning
     */
    void start();

    /**
     * @brief Stops scanning
     */
    void stop();

    /**
     * @brief Clears the list of found robots
     */
    void clear();

private slots:

    /**
     * @brief Publishes device if it is a Cellulo robot
     *
     * @param info Newly discovered device
     */
    void onDeviceDiscovered(QBluetoothDeviceInfo const& info);

    /**
     * @brief Restarts if continuously scanning
     */
    void decideRestart();

private:

    bool continuous;                        ///< Whether to continuously scan
    QBluetoothDeviceDiscoveryAgent scanner; ///< Bluetooth device scanner
    QStringList foundRobots;                ///< List of robot MAC addresses that are found
    QMap<QString, QString> address_map;

};

/** @} */

}

#endif // CELLULOBLUETOOTHSCANNER_H
