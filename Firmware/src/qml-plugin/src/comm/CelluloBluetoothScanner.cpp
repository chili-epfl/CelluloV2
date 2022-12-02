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
 * @file CelluloBluetoothScanner.cpp
 * @brief Scans for Cellulo robots
 * @author Ayberk Özgür
 * @date 2017-03-16
 */

#include "CelluloBluetoothScanner.h"

#include "CelluloCommUtil.h"

namespace Cellulo{

CelluloBluetoothScanner::CelluloBluetoothScanner(QQuickItem* parent) : QQuickItem(parent){
    continuous = false;
    address_map={
        {"00:06:66:E7:8A:D0", "0"},
        {"00:06:66:E7:8A:D1", "1"},
        {"00:06:66:E7:8A:DF", "2"},
        {"00:06:66:74:3E:93","3"},
        {"00:06:66:74:3E:89","4"},
        {"00:06:66:E7:8A:E0","5"},
        {"00:06:66:E7:8E:58","6"},
        {"00:06:66:E7:8E:59","7"},
        {"00:06:66:E7:8E:64","8"},
        {"00:06:66:D2:CF:96","9"},
        {"00:06:66:E7:8A:CE", "10"},
        {"00:06:66:D2:CF:99", "11"},
        {"00:06:66:D2:CF:7B", "12"},
        {"00:06:66:E7:8E:65","13"},
        {"00:06:66:E7:8E:6A","14"},
        {"00:06:66:E7:8E:5E","15"},
        {"00:06:66:E7:8A:44","16"},
        {"00:06:66:E7:8A:D6","17"},
        {"00:06:66:E7:8E:B2","18"},
        {"00:06:66:E7:8A:DE","19"},
        {"00:06:66:74:40:DB", "20"},
        {"00:06:66:74:41:03", "21"},
        {"00:06:66:E7:8A:C9", "22"},
        {"00:06:66:74:40:D3","23"},
        {"00:06:66:D2:CF:8B","24"},
        {"00:06:66:E7:8A:41","25"},
        {"00:06:66:E7:8A:E6","26"},
        {"00:06:66:E7:8A:43","27"},
        {"00:06:66:74:48:A7","28"},
        {"00:06:66:74:3E:7E","29"},
        {"00:06:66:E7:8A:CA", "30"},
        {"00:06:66:74:3E:82", "31"},
        {"00:06:66:E7:8A:E2", "32"},
        {"00:06:66:E7:8A:E7", "33"},
        {"00:06:66:74:46:58", "34"},
        {"00:06:66:D2:CF:90", "35"},
        {"00:06:66:E7:8E:6B", "36"},
        {"00:06:66:E7:8E:BE", "37"},
        {"00:06:66:E7:91:BC", "38"},
        {"00:06:66:E7:8A:E8", "39"}
        };
    connect(&scanner, SIGNAL(finished()), this, SIGNAL(finished()));
    connect(&scanner, SIGNAL(canceled()), this, SIGNAL(finished()));
    connect(this, SIGNAL(finished()), this, SIGNAL(scanningChanged()));
    connect(this, SIGNAL(finished()), this, SLOT(decideRestart()));
    connect(&scanner, SIGNAL(deviceDiscovered(QBluetoothDeviceInfo const&)), this, SLOT(onDeviceDiscovered(QBluetoothDeviceInfo const&)));
}


CelluloBluetoothScanner::~CelluloBluetoothScanner(){}

bool CelluloBluetoothScanner::isScanning() const {
    return scanner.isActive();
}

void CelluloBluetoothScanner::start(){
    if(!isScanning()){
        clear();
        #if defined(Q_OS_ANDROID)
            scanner.start(); //QBluetoothDeviceDiscoveryAgent::ClassicMethod fails for some reason on Android
        #else
            scanner.start(QBluetoothDeviceDiscoveryAgent::ClassicMethod);
        #endif
        emit scanningChanged();
    }
}

void CelluloBluetoothScanner::stop(){
    if(isScanning()){
        scanner.stop();
        emit scanningChanged();
    }
}

void CelluloBluetoothScanner::clear(){
    if(!foundRobots.isEmpty()){
        foundRobots.clear();
        emit foundRobotsChanged();
    }
}

void CelluloBluetoothScanner::decideRestart(){
    if(continuous)
        start();
}

void CelluloBluetoothScanner::onDeviceDiscovered(QBluetoothDeviceInfo const& info){
    QString name = info.name();
    QString macAddr = info.address().toString().toUpper();
    if(name.toLower().startsWith("cellulo") && !foundRobots.contains(macAddr)){
        foundRobots.append(macAddr);
        emit foundRobotsChanged();
        emit robotDiscovered(macAddr,address_map.contains(macAddr)?address_map[macAddr]:macAddr);
    }
}

}
