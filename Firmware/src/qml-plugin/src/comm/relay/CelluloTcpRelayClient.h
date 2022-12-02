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
 * @file CelluloTcpRelayClient.h
 * @brief Relays packets between a server and virtual robot objects over a TCP socket
 * @author Ayberk Özgür
 * @date 2017-03-16
 */

#ifndef CELLULOTCPRELAYCLIENT_H
#define CELLULOTCPRELAYCLIENT_H

#include "CelluloRelayClient.h"

namespace Cellulo{

class CelluloBluetooth;

/**
 * @addtogroup comm-relay
 * @{
 */

/**
 * @brief Relays packets between a server and virtual robot objects over a TCP socket. Connects to a `CelluloTcpRelayServer`.
 */
class CelluloTcpRelayClient : public CelluloRelayClient {
    /* *INDENT-OFF* */
    Q_OBJECT
    /* *INDENT-ON* */

public:

    /** @cond DO_NOT_DOCUMENT */

    /**
     * @brief Creates a new CelluloTcpRelayClient with the given QML parent
     *
     * @param parent The QML parent
     */
    CelluloTcpRelayClient(QQuickItem* parent = 0);

    /**
     * @brief Destroys this CelluloTcpRelayClient
     */
    virtual ~CelluloTcpRelayClient();

    /** @endcond */

};

/** @} */

}

#endif // CELLULOTCPRELAYCLIENT_H
