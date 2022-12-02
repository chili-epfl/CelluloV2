/*
 * Copyright (C) 2015 EPFL
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
 * @file CelluloBluetoothPacket.cpp
 * @brief Source for Bluetooth messages to/from Cellulo robots
 * @author Ayberk Özgür
 */

#include "CelluloBluetoothPacket.h"

#include <QMetaObject>
#include <QMetaEnum>

namespace Cellulo{

const char* CelluloBluetoothPacket::cmdPacketTypeStr[] = COMMAND_PACKET_STR_SHARED;
const int CelluloBluetoothPacket::cmdPacketPayloadLen[] = COMMAND_PACKET_PAYLOAD_LEN_SHARED;
const char* CelluloBluetoothPacket::eventPacketTypeStr[] = EVENT_PACKET_STR_SHARED;
const int CelluloBluetoothPacket::eventPacketPayloadLen[] = EVENT_PACKET_PAYLOAD_LEN_SHARED;

CelluloBluetoothPacket::operator QString() const {
    QString str;
    const QMetaObject& metaObject = CelluloBluetoothPacket::staticMetaObject;
    QMetaEnum metaEnum;

    metaEnum = metaObject.enumerator(metaObject.indexOfEnumerator("CmdPacketType"));
    str += "sendPacketType: ";
    str += metaEnum.valueToKey((int)cmdPacketType);

    metaEnum = metaObject.enumerator(metaObject.indexOfEnumerator("EventPacketType"));
    str += "; receivePacketType: ";
    str += metaEnum.valueToKey((int)eventPacketType);

    metaEnum = metaObject.enumerator(metaObject.indexOfEnumerator("ReceiveStatus"));
    str += "; receiveStatus: ";
    str += metaEnum.valueToKey((int)receiveStatus);

    str += "; payload:";
    QByteArray hex = payload.toHex().toUpper();
    for(int i=0; i<hex.length(); i+=2){
        str += " 0x";
        str += hex[i];
        str += hex[i + 1];
    }

    return str;
}

CelluloBluetoothPacket::CelluloBluetoothPacket(){
    clear();
}

CelluloBluetoothPacket::~CelluloBluetoothPacket(){ }

CelluloBluetoothPacket* CelluloBluetoothPacket::clone() const {
    CelluloBluetoothPacket* newPacket = new CelluloBluetoothPacket();
    newPacket->cmdPacketType = cmdPacketType;
    newPacket->eventPacketType = eventPacketType;
    newPacket->receiveStatus = receiveStatus;
    newPacket->receiveBytesRemaining = receiveBytesRemaining;
    newPacket->unloadIndex = unloadIndex;
    newPacket->payload.append(payload.data(), payload.size());
    return newPacket;
}

void CelluloBluetoothPacket::setCmdPacketType(CmdPacketType type){
    cmdPacketType = type;
}

void CelluloBluetoothPacket::setEventPacketType(EventPacketType type){
    eventPacketType = type;
}

void CelluloBluetoothPacket::clear(){
    cmdPacketType = CmdPacketTypeNumElements;
    eventPacketType = EventPacketTypeNumElements;

    receiveStatus = ReceiveStatus::NotReceiving;
    receiveBytesRemaining = -1;

    payload.clear();
    unloadIndex = 0;
}

void CelluloBluetoothPacket::load(quint32 num){

    //Convert to big endian
    char* p = ((char*)&num);
    payload.append(p[3]);
    payload.append(p[2]);
    payload.append(p[1]);
    payload.append(p[0]);
}

void CelluloBluetoothPacket::load24(quint32 num){
    if(num > 0xFFFFFF){
        num = 0xFFFFFF;
        qDebug() << "CelluloBluetoothPacket::load24(quint32 num): Number loaded too large, clamping to " << 0xFFFFFF;
    }

    //Convert to big endian
    char* p = ((char*)&num);
    payload.append(p[2]);
    payload.append(p[1]);
    payload.append(p[0]);
}

void CelluloBluetoothPacket::load(quint16 num){

    //Convert to big endian
    char* p = ((char*)&num);
    payload.append(p[1]);
    payload.append(p[0]);
}

void CelluloBluetoothPacket::load(quint8 num){
    payload.append(*((char*)&num));
}

void CelluloBluetoothPacket::load(qint32 num){

    //Convert to big endian
    char* p = ((char*)&num);
    payload.append(p[3]);
    payload.append(p[2]);
    payload.append(p[1]);
    payload.append(p[0]);
}

void CelluloBluetoothPacket::load24(qint32 num){
    if(num > 0x7FFFFF){
        num = 0x7FFFFF;
        qDebug() << "CelluloBluetoothPacket::load24(qint32 num): Number loaded too large, clamping to " << 0x7FFFFF;
    }
    else if(num < -0x800000){
        num = -0x800000;
        qDebug() << "CelluloBluetoothPacket::load24(qint32 num): Number loaded too small, clamping to " << -0x800000;
    }

    //Convert to big endian
    char* p = ((char*)&num);
    payload.append(p[2]);
    payload.append(p[1]);
    payload.append(p[0]);
}

void CelluloBluetoothPacket::load(qint16 num){

    //Convert to big endian
    char* p = ((char*)&num);
    payload.append(p[1]);
    payload.append(p[0]);
}

void CelluloBluetoothPacket::load(qint8 num){
    payload.append(*((char*)&num));
}

void CelluloBluetoothPacket::load(float num){

    //Convert to big endian
    char* p = ((char*)&num);
    payload.append(p[3]);
    payload.append(p[2]);
    payload.append(p[1]);
    payload.append(p[0]);
}

QByteArray CelluloBluetoothPacket::getCmdSendData() const{
    QByteArray data;

    data.append(PACKET_START_CHAR_SHARED);
    data.append(cmdPacketTypeStr[(int)cmdPacketType]);
    data.append(payload);

    return data;
}

QByteArray CelluloBluetoothPacket::getEventSendData() const{
    QByteArray data;

    data.append(PACKET_START_CHAR_SHARED);
    data.append(eventPacketTypeStr[(int)eventPacketType]);
    data.append(payload);

    return data;
}

bool CelluloBluetoothPacket::loadEventByte(char c){
    switch(receiveStatus){
        case ReceiveStatus::NotReceiving:
            if(c == PACKET_START_CHAR_SHARED)
                receiveStatus = ReceiveStatus::WaitingForType;
            return false;

        case ReceiveStatus::WaitingForType:

            //Determine type
            eventPacketType = EventPacketTypeNumElements;
            for(int i=0; i<(int)EventPacketTypeNumElements; i++)
                if(eventPacketTypeStr[i][0] == c){
                    eventPacketType = (EventPacketType)i;
                    break;
                }

            //Valid packet type
            if(eventPacketType != EventPacketTypeNumElements){
                receiveBytesRemaining = eventPacketPayloadLen[(int)eventPacketType];
                if(receiveBytesRemaining <= 0){
                    receiveStatus = ReceiveStatus::EndOfPacket;
                    return true;
                }
                else{
                    receiveStatus = ReceiveStatus::PayloadReceiving;
                    return false;
                }
            }

            //Invalid packet type, reset packet
            else{
                clear();
                return false;
            }

        case ReceiveStatus::PayloadReceiving:
            payload.append(c);
            receiveBytesRemaining--;
            if(receiveBytesRemaining <= 0){
                receiveStatus = ReceiveStatus::EndOfPacket;
                return true;
            }
            else
                return false;

        case ReceiveStatus::EndOfPacket:
            if(c == PACKET_START_CHAR_SHARED){
                clear();
                receiveStatus = ReceiveStatus::WaitingForType;
            }
            return false;

        default:
            return false;
    }
}

bool CelluloBluetoothPacket::loadCmdByte(char c){
    switch(receiveStatus){
        case ReceiveStatus::NotReceiving:
            if(c == PACKET_START_CHAR_SHARED)
                receiveStatus = ReceiveStatus::WaitingForType;
            return false;

        case ReceiveStatus::WaitingForType:

            //Determine type
            cmdPacketType = CmdPacketTypeNumElements;
            for(int i=0; i<(int)CmdPacketTypeNumElements; i++)
                if(cmdPacketTypeStr[i][0] == c){
                    cmdPacketType = (CmdPacketType)i;
                    break;
                }

            //Valid packet type
            if(cmdPacketType != CmdPacketTypeNumElements){
                receiveBytesRemaining = cmdPacketPayloadLen[(int)cmdPacketType];
                if(receiveBytesRemaining <= 0){
                    receiveStatus = ReceiveStatus::EndOfPacket;
                    return true;
                }
                else{
                    receiveStatus = ReceiveStatus::PayloadReceiving;
                    return false;
                }
            }

            //Invalid packet type, reset packet
            else{
                clear();
                return false;
            }

        case ReceiveStatus::PayloadReceiving:
            payload.append(c);
            receiveBytesRemaining--;
            if(receiveBytesRemaining <= 0){
                receiveStatus = ReceiveStatus::EndOfPacket;
                return true;
            }
            else
                return false;

        case ReceiveStatus::EndOfPacket:
            if(c == PACKET_START_CHAR_SHARED){
                clear();
                receiveStatus = ReceiveStatus::WaitingForType;
            }
            return false;

        default:
            return false;
    }
}

quint32 CelluloBluetoothPacket::unloadUInt32(){
    if(unloadIndex + 4 > payload.length()){
        qDebug() << "CelluloBluetoothPacket::unloadUInt32(): Unload index out of bounds";
        return 0;
    }
    else{

        //Decode from big endian
        unsigned char* p = (unsigned char*)payload.data() + unloadIndex;
        unloadIndex += 4;
        return (quint32)((p[0] << 24) | (p[1] << 16) | (p[2] << 8) | (p[3] << 0)); //TODO: CHECK OVERFLOW ON PROMOTE TO INT WHEN << 24
    }
}

quint32 CelluloBluetoothPacket::unloadUInt24(){
    if(unloadIndex + 3 > payload.length()){
        qDebug() << "CelluloBluetoothPacket::unloadUInt24(): Unload index out of bounds";
        return 0;
    }
    else{

        //Decode from big endian
        unsigned char* p = (unsigned char*)payload.data() + unloadIndex;
        unloadIndex += 3;
        return (quint32)((p[0] << 16) | (p[1] << 8) | (p[2] << 0));
    }
}

quint16 CelluloBluetoothPacket::unloadUInt16(){
    if(unloadIndex + 2 > payload.length()){
        qDebug() << "CelluloBluetoothPacket::unloadUInt16(): Unload index out of bounds";
        return 0;
    }
    else{

        //Decode from big endian
        unsigned char* p = (unsigned char*)payload.data() + unloadIndex;
        unloadIndex += 2;
        return (quint16)((p[0] << 8) | (p[1] << 0));
    }
}

quint8 CelluloBluetoothPacket::unloadUInt8(){
    if(unloadIndex + 1 > payload.length()){
        qDebug() << "CelluloBluetoothPacket::unloadUInt8(): Unload index out of bounds";
        return 0;
    }
    else{
        quint8 result = (quint8)(*((unsigned char*)payload.data() + unloadIndex));
        unloadIndex++;
        return result;
    }
}

qint32 CelluloBluetoothPacket::unloadInt32(){
    if(unloadIndex + 4 > payload.length()){
        qDebug() << "CelluloBluetoothPacket::unloadInt32(): Unload index out of bounds";
        return 0;
    }
    else{

        //Decode from big endian
        unsigned char* p = (unsigned char*)payload.data() + unloadIndex;
        unloadIndex += 4;
        quint32 unum = (quint32)((p[0] << 24) | (p[1] << 16) | (p[2] << 8) | (p[3] << 0)); //TODO: CHECK OVERFLOW ON PROMOTE TO INT WHEN << 24
        return *((qint32*)(&unum));
    }
}

qint32 CelluloBluetoothPacket::unloadInt24(){
    if(unloadIndex + 3 > payload.length()){
        qDebug() << "CelluloBluetoothPacket::unloadInt24(): Unload index out of bounds";
        return 0;
    }
    else{

        //Decode from big endian
        unsigned char* p = (unsigned char*)payload.data() + unloadIndex;
        unloadIndex += 3;
        quint32 unum = (quint32)((p[0] << 16) | (p[1] << 8) | (p[2] << 0));

        //Sign extension
        if(unum >= 0x800000)
            unum += 0xFF000000;
        return *((qint32*)(&unum));
    }
}

qint16 CelluloBluetoothPacket::unloadInt16(){
    if(unloadIndex + 2 > payload.length()){
        qDebug() << "CelluloBluetoothPacket::unloadInt16(): Unload index out of bounds";
        return 0;
    }
    else{

        //Decode from big endian
        unsigned char* p = (unsigned char*)payload.data() + unloadIndex;
        unloadIndex += 2;
        quint16 unum = (quint16)((p[0] << 8) | (p[1] << 0));
        return *((qint16*)(&unum));
    }
}

qint8 CelluloBluetoothPacket::unloadInt8(){
    if(unloadIndex + 1 > payload.length()){
        qDebug() << "CelluloBluetoothPacket::unloadInt8(): Unload index out of bounds";
        return 0;
    }
    else{
        qint8 result = (qint8)(*((char*)payload.data() + unloadIndex));
        unloadIndex++;
        return result;
    }
}

float CelluloBluetoothPacket::unloadFloat(){
    if(unloadIndex + 4 > payload.length()){
        qDebug() << "CelluloBluetoothPacket::unloadFloat(): Unload index out of bounds";
        return 0;
    }
    else{

        //Decode from big endian
        unsigned char* p = (unsigned char*)payload.data() + unloadIndex;
        unloadIndex += 4;
        quint32 unum = (quint32)((p[0] << 24) | (p[1] << 16) | (p[2] << 8) | (p[3] << 0)); //TODO: CHECK OVERFLOW ON PROMOTE TO INT WHEN << 24
        float* num = (float*)(&unum);
        return *num;
    }
}

}
