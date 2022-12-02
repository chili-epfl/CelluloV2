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
 * @file CelluloBluetoothPacket.h
 * @brief Header for a Bluetooth message to/from Cellulo robots
 * @author Ayberk Özgür
 */

#ifndef CELLULOBLUETOOTHPACKET_H
#define CELLULOBLUETOOTHPACKET_H

#include <QObject>
#include <QDebug>
#include <QtGlobal>
#include <QByteArray>

#include "CelluloBluetoothSharedDefs.h"

namespace Cellulo{

/** @cond DO_NOT_DOCUMENT */

/**
 * @brief Bluetooth communicator for a Cellulo robot
 */
class CelluloBluetoothPacket : public QObject {
    /* *INDENT-OFF* */
    Q_OBJECT
    /* *INDENT-ON* */

public:
    COMMAND_PACKET_TYPE_ENUM_SHARED;
    EVENT_PACKET_TYPE_ENUM_SHARED;

    static const char* cmdPacketTypeStr[];    ///< Strings sent over Bluetooth to give commands
    static const char* eventPacketTypeStr[];  ///< Strings received over Bluetooth as response or event

    static const int cmdPacketPayloadLen[];   ///< Total lengths of packets sent over Bluetooth
    static const int eventPacketPayloadLen[]; ///< Total lengths of packets received over Bluetooth

    /**
     * @brief Creates a new Cellulo Bluetooth message
     */
    CelluloBluetoothPacket();

    /**
     * @brief Destroys this Cellulo Bluetooth message
     */
    virtual ~CelluloBluetoothPacket();

    /**
     * @brief Creates a new Cellulo Bluetooth message identical to this
     *
     * @return Identical copy of this packet
     */
    CelluloBluetoothPacket* clone() const;

    /**
     * @brief Prettyprinting
     *
     * @return String representation of this object
     */
    operator QString() const;

    /**
     * @brief Sets the outgoing command message type
     *
     * @param type Outgoing message type
     */
    void setCmdPacketType(CmdPacketType type);

    /**
     * @brief Sets the outgoing event message type
     *
     * @param type Outgoing event message type
     */
    void setEventPacketType(EventPacketType type);

    /**
     * @brief Clears the payload
     */
    void clear();

    /**
     * @brief Appends the given number in binary form to the payload
     *
     * @param num The number to append
     */
    void load(quint32 num);

    /**
     * @brief Appends the first 24 bits of the given number in binary form (2's complement) to the payload
     *
     * Clamps the value to 24-bits if outside of bounds.
     *
     * @param num The number to append
     */
    void load24(quint32 num);

    /**
     * @brief Appends the given number in binary form to the payload
     *
     * @param num The number to append
     */
    void load(quint16 num);

    /**
     * @brief Appends the given number in binary form to the payload
     *
     * @param num The number to append
     */
    void load(quint8 num);

    /**
     * @brief Appends the given number in binary form to the payload
     *
     * @param num The number to append
     */
    void load(qint32 num);

    /**
     * @brief Appends the first 24 bits of the given number in binary form (2's complement) to the payload
     *
     * Clamps the value to 24-bits if outside of bounds.
     *
     * @param num The number to append
     */
    void load24(qint32 num);

    /**
     * @brief Appends the given number in binary form to the payload
     *
     * @param num The number to append
     */
    void load(qint16 num);

    /**
     * @brief Appends the given number in binary form to the payload
     *
     * @param num The number to append
     */
    void load(qint8 num);

    /**
     * @brief Appends the given number in binary form to the payload
     *
     * @param num The number to append
     */
    void load(float num);

    /**
     * @brief Constructs the command message to be sent
     *
     * @return Command message to be sent
     */
    QByteArray getCmdSendData() const;

    /**
     * @brief Constructs the event message to be sent
     *
     * @return Event message to be sent
     */
    QByteArray getEventSendData() const;

    /**
     * @brief Processes incoming byte belonging to an event packet, determines type if byte is relevant
     *
     * @param c Incoming event packet byte
     * @return Whether the packet is completely received
     */
    bool loadEventByte(char c);

    /**
     * @brief Processes incoming byte belonging to an command packet, determines type if byte is relevant
     *
     * @param c Incoming command packet byte
     * @return Whether the packet is completely received
     */
    bool loadCmdByte(char c);

    /**
     * @brief Gets the received event packet type if any
     *
     * @return The received event packet type
     */
    EventPacketType getEventPacketType() const { return eventPacketType; }

    /**
     * @brief Gets the received command packet type if any
     *
     * @return The received command packet type
     */
    CmdPacketType getCmdPacketType() const { return cmdPacketType; }

    /**
     * @brief Attempts to unload a 32-bit unsigned integer
     *
     * @return Unloaded value if successful
     */
    quint32 unloadUInt32();

    /**
     * @brief Attempts to unload a 24-bit unsigned integer
     *
     * @return Unloaded value if successful
     */
    quint32 unloadUInt24();

    /**
     * @brief Attempts to unload a 16-bit unsigned integer
     *
     * @return Unloaded value if successful
     */
    quint16 unloadUInt16();

    /**
     * @brief Attempts to unload an 8-bit unsigned integer
     *
     * @return Unloaded value if successful
     */
    quint8 unloadUInt8();

    /**
     * @brief Attempts to unload a 32-bit signed integer
     *
     * @return Unloaded value if successful
     */
    qint32 unloadInt32();

    /**
     * @brief Attempts to unload a 24-bit signed integer
     *
     * @return Unloaded value if successful
     */
    qint32 unloadInt24();

    /**
     * @brief Attempts to unload a 16-bit signed integer
     *
     * @return Unloaded value if successful
     */
    qint16 unloadInt16();

    /**
     * @brief Attempts to unload a 8-bit signed integer
     *
     * @return Unloaded value if successful
     */
    qint8 unloadInt8();

    /**
     * @brief Attempts to unload a 32-bit floating point number
     *
     * @return Unloaded value if successful
     */
    float unloadFloat();

private:

    /**
     * @brief Describes the status of the packet while it's being received
     */
    enum class ReceiveStatus {
        NotReceiving = 0,
        WaitingForType,
        PayloadReceiving,
        EndOfPacket,
        NumElements
    };

    CmdPacketType cmdPacketType;           ///< Packet type if command packet
    EventPacketType eventPacketType;       ///< Packet type if event packet

    ReceiveStatus receiveStatus;           ///< Current status if receiving packet
    int receiveBytesRemaining;             ///< Number of bytes left to receive

    QByteArray payload;                    ///< Payload of the actual packet
    int unloadIndex;                       ///< Current index to unload from the payload
};

/** @endcond */

}

#endif // CELLULOBLUETOOTHPACKET_H
