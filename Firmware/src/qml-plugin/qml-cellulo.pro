QT += qml quick bluetooth

CONFIG += qt plugin c++11 nostrip

linux:!android {
    CONFIG += link_pkgconfig

    packagesExist(bluez){
        PKGCONFIG += bluez
        DEFINES += BT_MULTIADAPTER_SUPPORT
        HEADERS += \
            src/comm/CelluloBluezUtil.h
        SOURCES += \
            src/comm/CelluloBluezUtil.cpp
        message("BlueZ found, enabling Bluetooth multiadapter support.")
    }
    else{
        message("BlueZ not found, disabling Bluetooth multiadapter support.")
    }
}

android{
    QT += androidextras
}

HEADERS += \
    src/CelluloPlugin.h \
    src/robot/CelluloRobot.h \
    src/robot/PolyBezierTracker.h \
    src/robot/PolyBezierTrackerConstVel.h \
    src/robot/PolyBezierTrackerConstAccel.h \
    src/robot/PolyBezierTrackerAdaptiveVel.h \
    src/robot/PolyBezierTrackerProfiledVel.h \
    src/comm/CelluloBluetooth.h \
    src/comm/CelluloCommUtil.h \
    src/comm/CelluloBluetoothEMP.h \
    src/comm/CelluloBluetoothScanner.h \
    src/comm/CelluloBluetoothEnums.h \
    src/comm/CameraFrameImageProvider.h \
    src/comm/CelluloBluetoothPacket.h \
    src/comm/relay/CelluloRelayServer.h \
    src/comm/relay/CelluloRelayClient.h \
    src/comm/relay/CelluloLocalRelayServer.h \
    src/comm/relay/CelluloLocalRelayClient.h \
    src/comm/relay/CelluloRobotPoolClient.h \
    src/comm/relay/CelluloRobotHubClient.h \
    src/comm/relay/CelluloTcpRelayServer.h \
    src/comm/relay/CelluloTcpRelayClient.h \
    src/zones/CelluloZoneClient.h \
    src/zones/CelluloZonePoint.h \
    src/zones/CelluloZoneLine.h \
    src/zones/CelluloZoneCircle.h \
    src/zones/CelluloZonePolygon.h \
    src/zones/CelluloZonePolyBezier.h \
    src/zones/CelluloZone.h \
    src/zones/CelluloZoneRectangle.h \
    src/zones/CelluloZoneAngle.h \
    src/zones/CelluloZoneAngleInterval.h \
    src/zones/CelluloZoneEngine.h \
    src/zones/CelluloZoneJsonHandler.h \
    src/zones/CelluloZoneTypes.h \
    src/zones/CelluloZonePaintedItem.h \
    src/util/math/CelluloMathUtil.h \
    src/util/math/CubicBezier.h \
    src/util/math/PolyBezier.h \
    src/util/svg/CelluloSVGUtil.h \
    src/util/lang/TranslationTool.h \
    src/util/system/CelluloSystemUtil.h \
    src/util/ui/CelluloUIUtil.h \
    src/util/ui/ProgressCircle.h \
    src/tile/PoseRemapper.h \
    src/tile/OffsetRemapper.h \
    src/tile/HexTileMap.h \
    src/tile/HexTile.h \
    src/tile/HexTileStdSourceCoords.h \
    src/tile/CoordSpaceConverter.h \
    src/tile/AxialHexCoords.h \
    src/tile/HexTileMapAutoBuilder.h

SOURCES += \
    src/CelluloPlugin.cpp \
    src/robot/CelluloRobot.cpp \
    src/robot/PolyBezierTracker.cpp \
    src/robot/PolyBezierTrackerConstVel.cpp \
    src/robot/PolyBezierTrackerConstAccel.cpp \
    src/robot/PolyBezierTrackerAdaptiveVel.cpp \
    src/robot/PolyBezierTrackerProfiledVel.cpp \
    src/comm/CelluloBluetooth.cpp \
    src/comm/CelluloCommUtil.cpp \
    src/comm/CelluloBluetoothEMP.cpp \
    src/comm/CelluloBluetoothScanner.cpp \
    src/comm/CelluloBluetoothEnums.cpp \
    src/comm/CameraFrameImageProvider.cpp \
    src/comm/CelluloBluetoothPacket.cpp \
    src/comm/relay/CelluloRelayServer.cpp \
    src/comm/relay/CelluloRelayClient.cpp \
    src/comm/relay/CelluloLocalRelayServer.cpp \
    src/comm/relay/CelluloLocalRelayClient.cpp \
    src/comm/relay/CelluloRobotPoolClient.cpp \
    src/comm/relay/CelluloRobotHubClient.cpp \
    src/comm/relay/CelluloTcpRelayServer.cpp \
    src/comm/relay/CelluloTcpRelayClient.cpp \
    src/zones/CelluloZoneClient.cpp \
    src/zones/CelluloZonePoint.cpp \
    src/zones/CelluloZoneLine.cpp \
    src/zones/CelluloZoneCircle.cpp \
    src/zones/CelluloZonePolygon.cpp \
    src/zones/CelluloZonePolyBezier.cpp \
    src/zones/CelluloZone.cpp \
    src/zones/CelluloZoneRectangle.cpp \
    src/zones/CelluloZoneAngle.cpp \
    src/zones/CelluloZoneAngleInterval.cpp \
    src/zones/CelluloZoneEngine.cpp \
    src/zones/CelluloZoneJsonHandler.cpp \
    src/zones/CelluloZoneTypes.cpp \
    src/zones/CelluloZonePaintedItem.cpp \
    src/util/math/CelluloMathUtil.cpp \
    src/util/math/CubicBezier.cpp \
    src/util/math/PolyBezier.cpp \
    src/util/svg/CelluloSVGUtil.cpp \
    src/util/lang/TranslationTool.cpp \
    src/util/system/CelluloSystemUtil.cpp \
    src/util/ui/CelluloUIUtil.cpp \
    src/util/ui/ProgressCircle.cpp \
    src/tile/PoseRemapper.cpp \
    src/tile/OffsetRemapper.cpp \
    src/tile/HexTileMap.cpp \
    src/tile/HexTile.cpp \
    src/tile/HexTileStdSourceCoords.cpp \
    src/tile/CoordSpaceConverter.cpp \
    src/tile/AxialHexCoords.cpp \
    src/tile/HexTileMapAutoBuilder.cpp

INCLUDEPATH += \
    include/

QML_SOURCES = \
    src/util/ui/MacAddrSelector.qml \
    src/util/ui/Toast.qml \
    src/util/ui/ToastManager.qml

JS_SOURCES =

test{
    message(Configuring test build...)

    TEMPLATE = app
    TARGET = celluloplugintests

    QT += testlib

    HEADERS += \
        test/TestCelluloBluetoothPacket.h

    SOURCES += \
        test/TestCelluloBluetoothPacket.cpp
}
else{
    TEMPLATE = lib
    TARGET = celluloplugin

    CONFIG += plugin
    CONFIG -= android_install

    TARGET = $$qtLibraryTarget($$TARGET)
    uri = Cellulo

    #File installation

    qmldir.files = qmldir
    javascript.files = $$JS_SOURCES
    qml.files = $$QML_SOURCES

    OTHER_FILES += qmldir.files javascript.files qml.files

    installPath = $$[QT_INSTALL_QML]/$$replace(uri, \\., /)
    qmldir.path = $$installPath
    javascript.path = $$installPath
    qml.path = $$installPath
    target.path = $$installPath
    INSTALLS += target qmldir javascript qml
}
