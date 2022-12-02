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
 * @file CelluloLocalRelayServer.h
 * @brief Relays packets between a client and physical robot objects over a Unix domain socket
 * @author Ayberk Özgür
 * @date 2017-03-16
 */

#ifndef CELLULOLOCALRELAYSERVER_H
#define CELLULOLOCALRELAYSERVER_H

#include "CelluloRelayServer.h"

namespace Cellulo{

class CelluloBluetooth;

/**
 * @addtogroup comm-relay
 * @{
 */

/**
 * @brief Relays packets between a client and physical robots over a Unix domain socket. Accepts connections from a `CelluloLocalRelayClient`.
 */
class CelluloLocalRelayServer : public CelluloRelayServer {
    /* *INDENT-OFF* */
    Q_OBJECT
    /* *INDENT-ON* */

public:

    /** @cond DO_NOT_DOCUMENT */

    /**
     * @brief Creates a new CelluloLocalRelayServer with the given QML parent
     *
     * @param parent The QML parent
     */
    CelluloLocalRelayServer(QQuickItem* parent = 0);

    /**
     * @brief Destroys this CelluloLocalRelayServer
     */
    virtual ~CelluloLocalRelayServer();

    /** @endcond */

public slots:

    /**
     * @brief Removes the local socket file, i.e calls QLocalServer::removeServer()
     *
     * @return Whether the socket file was successfully cleaned
     */
    bool cleanSocket();

};

/** @} */

}

#endif // CELLULOLOCALRELAYSERVER_H
