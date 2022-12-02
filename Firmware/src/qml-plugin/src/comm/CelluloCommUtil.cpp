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
 * @file CelluloCommUtil.cpp
 * @brief Common objects used by Cellulo communication objects
 * @author Ayberk Özgür
 * @date 2017-03-15
 */

#include "CelluloCommUtil.h"

#if defined(Q_OS_ANDROID)
    #include<QAndroidJniObject>
    #include<QtAndroid>
#endif

#include "../util/system/CelluloSystemUtil.h"

namespace Cellulo{

CelluloCommUtil::CelluloCommUtil(QObject* parent) : QObject(parent){}

CelluloCommUtil::~CelluloCommUtil(){}

QList<quint8> CelluloCommUtil::getOctets(QString const& macAddr){
    QList<quint8> octets;

    if(macAddr.isEmpty())
        for(int i=0;i<6;i++)
            octets.append(0);
    else{
        QStringList macAddrOctetStrings = macAddr.split(':');
        if(macAddrOctetStrings.size() != 6){
            qWarning() << "CelluloCommUtil::getOctets(): Provided MAC address is in the wrong format.";
            for(int i=0;i<6;i++)
                octets.append(0);
        }
        else
            for(int i=0;i<6;i++)
                octets.append((quint8)(macAddrOctetStrings[i].toUInt(NULL, 16)));
    }

    return octets;
}

QString CelluloCommUtil::getMacAddr(QList<quint8> const& octets){
    if(octets.size() != 6){
        qWarning() << "CelluloCommUtil::getMacAddr(): Wrong number of octets, must be 6.";
        return "";
    }

    quint8 octet = octets[0];
    QString macAddr = (octet <= 0xF ? "0" : "") + QString::number(octet, 16);
    for(int i=1;i<6;i++){
        octet = octets[i];
        macAddr += ":";
        macAddr += (octet <= 0xF ? "0" : "") + QString::number(octet, 16);
    }
    if(macAddr == "00:00:00:00:00:00")
        macAddr = "";

    return macAddr;
}

bool CelluloCommUtil::testRobotPoolDaemon(){
    #if defined(Q_OS_ANDROID)
        return QAndroidJniObject::isClassAvailable("ch/epfl/chili/cellulo/cellulorobotpoold/CelluloRobotPoolService");
    #elif defined(Q_OS_LINUX)
        return CelluloSystemUtil::exec("test", QStringList() << "-f" << "/usr/local/bin/cellulorobotpoold") == 0;
    #else
        qWarning() << "CelluloCommUtil::testRobotPoolDaemon(): Not implemented on this platform.";
        return false;
    #endif
}

bool CelluloCommUtil::startRobotPoolDaemon(){
    #if defined(Q_OS_ANDROID)
        QAndroidJniObject::callStaticMethod<void>(
            "ch/epfl/chili/cellulo/cellulorobotpoold/CelluloRobotPoolService",
            "startCelluloRobotPoolService",
            "(Landroid/content/Context;)V",
            QtAndroid::androidActivity().object()
        );
        return true;
    #elif defined(Q_OS_LINUX)
        return CelluloSystemUtil::exec("start-stop-daemon", QStringList() << "--start" << "--exec" << "/usr/local/bin/cellulorobotpoold") == 0;
    #else
        qWarning() << "CelluloCommUtil::startRobotPoolDaemon(): Not implemented on this platform.";
        return false;
    #endif
}

bool CelluloCommUtil::stopRobotPoolDaemon(){
    #if defined(Q_OS_ANDROID)
        QAndroidJniObject::callStaticMethod<void>(
            "ch/epfl/chili/cellulo/cellulorobotpoold/CelluloRobotPoolService",
            "stopCelluloRobotPoolService",
            "(Landroid/content/Context;)V",
            QtAndroid::androidActivity().object()
        );
        return true;
    #elif defined(Q_OS_LINUX)
        return CelluloSystemUtil::exec("start-stop-daemon", QStringList() << "--stop" << "--exec" << "/usr/local/bin/cellulorobotpoold") == 0;
    #else
        qWarning() << "CelluloCommUtil::stopRobotPoolDaemon(): Not implemented on this platform.";
        return false;
    #endif
}

}
