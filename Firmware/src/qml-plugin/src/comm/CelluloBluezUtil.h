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
 * @file CelluloBluezUtil.h
 * @brief All things Cellulo done with the bluez c API
 * @author Ayberk Özgür
 * @date 2018-09-04
 */

#ifndef CELLULOBLUEZUTIL_H
#define CELLULOBLUEZUTIL_H

#include<QBluetoothSocket>

namespace Cellulo{


/** @cond DO_NOT_DOCUMENT */

/**
 * @brief Object that exposes Cellulo functionalities that require the bluez c API
 */
class CelluloBluezUtil{

public:

    /**
     * @brief Binds a QBluetoothSocket to a local adapter, meaning that the connection to the socket will be requested over this adapter upon connection
     *
     * @param  socket              Yet unopened socket
     * @param  localAdapterMacAddr Local adapter's MAC address
     * @return                     Whether successful (WARNING: Returns success on many cases of failure)
     */
    static bool bindToLocalAdapter(QBluetoothSocket* socket, QString const& localAdapterMacAddr);

    /**
     * @brief Checks whether the given device is connected over another local adapter than the desired one
     *
     * @param  macAddr                    MAC address of the device of interest
     * @param  correctLocalAdapterMacAddr MAC address of the local adapter the device is supposed to be connected over
     * @param  wrongLocalAdapterDevID     [Output] Low level device ID of the wrong local adapter in case one is found
     * @return                            Whether connected over a wrong adapter
     */
    static bool connectedOverWrongLocalAdapter(QString const& macAddr, QString const& correctLocalAdapterMacAddr, int& wrongLocalAdapterDevID);

    /**
     * @brief Attempts to disconnect given device from the given local adapter, only if the connection is established over that adapter
     *
     * @param  macAddr           MAC address of the device of interest
     * @param  localAdapterDevID Low level device ID of the local adapter
     * @return                   Whether disconnection was successful
     */
    static bool disconnectFromLocalAdapter(QString const& macAddr, int localAdapterDevID);

};

/** @endcond */

}

#endif // CELLULOBLUEZUTIL_H
