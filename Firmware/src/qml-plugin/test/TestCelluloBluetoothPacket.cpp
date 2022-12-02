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
 * @file TestCelluloBluetoothPacket.cpp
 * @brief CelluloBluetoothPacket unit test source
 * @author Ayberk Özgür
 */

#include "TestCelluloBluetoothPacket.h"
#include "../src/CelluloBluetoothPacket.h"

#include <QByteArray>

using SEND_PACKET_TYPE = CelluloBluetoothPacket::SEND_PACKET_TYPE;

/**
 * @brief Quick byte checker on unsigned char* bytes, increments bytes after check
 * @param B Byte to check
 */
#define BCOMP(B) QCOMPARE(static_cast<unsigned char>((B)), *bytes++)

void TestCelluloBluetoothPacket::loadUnload(){
    CelluloBluetoothPacket packet;

    packet.setSendPacketType(SEND_PACKET_TYPE::RESET);

    //Negative signed int loads
    packet.load((qint8)-100);
    packet.load((qint16)-22454);
    packet.load24((qint32)-6123456);
    packet.load24((qint32)-1000000000); //Overflow, should result in -8388608
    packet.load((qint32)-1734567890);

    //Positive signed int loads
    packet.load((qint8)126);
    packet.load((qint16)12545);
    packet.load24((qint32)6654321);
    packet.load24((qint32)1000000000); //Overflow, should result in 8388607
    packet.load((qint32)1834567890);

    //Unsigned int loads
    packet.load((quint8)200);
    packet.load((quint16)52545);
    packet.load24((quint32)12154321);
    packet.load24((quint32)1000000000); //Overflow, should result in 16777215
    packet.load((quint32)3034567890);

    QByteArray data = packet.getSendData();
    const unsigned char* bytes = (const unsigned char*) data.data();

    //Check header
    char packetStartChar = CelluloBluetoothPacket::PACKET_START_CHAR;
    QCOMPARE(packetStartChar, (char)bytes[0]);
    QCOMPARE((CelluloBluetoothPacket::sendPacketTypeStr[(int)SEND_PACKET_TYPE::RESET])[0], (char)bytes[1]);
    bytes += 2;

    //Check negative signed int loads
    BCOMP(0x9C); //-100
    BCOMP(0xA8); BCOMP(0x4A); //-22454
    BCOMP(0xA2); BCOMP(0x90); BCOMP(0x40); //-6123456
    BCOMP(0x80); BCOMP(0x00); BCOMP(0x00); //-1000000000
    BCOMP(0x98); BCOMP(0x9C); BCOMP(0x98); BCOMP(0x2E); //-1734567890

    //Check positive signed int loads
    BCOMP(0x7E); //126
    BCOMP(0x31); BCOMP(0x01); //12545
    BCOMP(0x65); BCOMP(0x89); BCOMP(0x71); //6654321
    BCOMP(0x7F); BCOMP(0xFF); BCOMP(0xFF); //1000000000
    BCOMP(0x6D); BCOMP(0x59); BCOMP(0x48); BCOMP(0xD2); //1834567890

    //Check unsigned int loads
    BCOMP(0xC8); //200
    BCOMP(0xCD); BCOMP(0x41); //52545
    BCOMP(0xB9); BCOMP(0x75); BCOMP(0xD1); //12154321
    BCOMP(0xFF); BCOMP(0xFF); BCOMP(0xFF); //1000000000
    BCOMP(0xB4); BCOMP(0xDF); BCOMP(0xD4); BCOMP(0xD2); //3034567890

    //Check negative signed int unloads
    QCOMPARE(packet.unloadInt8(), (qint8)-100);
    QCOMPARE(packet.unloadInt16(), (qint16)-22454);
    QCOMPARE(packet.unloadInt24(), (qint32)-6123456);
    QCOMPARE(packet.unloadInt24(), (qint32)-8388608);
    QCOMPARE(packet.unloadInt32(), (qint32)-1734567890);

    //Check positive signed int unloads
    QCOMPARE(packet.unloadInt8(), (qint8)126);
    QCOMPARE(packet.unloadInt16(), (qint16)12545);
    QCOMPARE(packet.unloadInt24(), (qint32)6654321);
    QCOMPARE(packet.unloadInt24(), (qint32)8388607);
    QCOMPARE(packet.unloadInt32(), (qint32)1834567890);

    //Check usigned int unloads
    QCOMPARE(packet.unloadUInt8(), (quint8)200);
    QCOMPARE(packet.unloadUInt16(), (quint16)52545);
    QCOMPARE(packet.unloadUInt24(), (quint32)12154321);
    QCOMPARE(packet.unloadUInt24(), (quint32)16777215);
    QCOMPARE(packet.unloadUInt32(), (quint32)3034567890);
}

QTEST_MAIN(TestCelluloBluetoothPacket)
