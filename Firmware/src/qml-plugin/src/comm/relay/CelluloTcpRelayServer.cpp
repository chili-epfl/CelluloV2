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
 * @file CelluloTcpRelayServer.cpp
 * @brief Relays packets between a client and virtual robot objects over a TCP socket
 * @author Ayberk Özgür
 * @date 2017-03-16
 */

#include "CelluloTcpRelayServer.h"
#include "../CelluloCommUtil.h"

namespace Cellulo{

CelluloTcpRelayServer::CelluloTcpRelayServer(QQuickItem* parent) : CelluloRelayServer(CelluloCommUtil::RelayProtocol::Tcp, parent){}

CelluloTcpRelayServer::~CelluloTcpRelayServer(){}

}
