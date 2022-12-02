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
 * @file CelluloBluetoothPlugin.cpp
 * @brief Source for exposing Cellulo objects as QML objects
 * @author Ayberk Özgür
 * @date 2016-06-26
 */

#include "CelluloPlugin.h"

#include <QQmlEngine>
#include <qqml.h>

#include "robot/CelluloRobot.h"
#include "robot/PolyBezierTrackerConstVel.h"
#include "robot/PolyBezierTrackerConstAccel.h"
#include "robot/PolyBezierTrackerAdaptiveVel.h"
#include "robot/PolyBezierTrackerProfiledVel.h"
#include "comm/CelluloBluetooth.h"
#include "comm/CelluloCommUtil.h"
#include "comm/relay/CelluloLocalRelayClient.h"
#include "comm/relay/CelluloLocalRelayServer.h"
#include "comm/relay/CelluloRobotPoolClient.h"
#include "comm/relay/CelluloRobotHubClient.h"
#include "comm/relay/CelluloTcpRelayClient.h"
#include "comm/relay/CelluloTcpRelayServer.h"
#include "comm/CelluloBluetoothScanner.h"
#include "comm/CelluloBluetoothEMP.h"
#include "comm/CelluloBluetoothEnums.h"
#include "comm/CameraFrameImageProvider.h"
#include "zones/CelluloZone.h"
#include "zones/CelluloZoneClient.h"
#include "zones/CelluloZonePolyBezier.h"
#include "zones/CelluloZonePolygon.h"
#include "zones/CelluloZonePoint.h"
#include "zones/CelluloZoneLine.h"
#include "zones/CelluloZoneCircle.h"
#include "zones/CelluloZoneRectangle.h"
#include "zones/CelluloZoneAngle.h"
#include "zones/CelluloZoneAngleInterval.h"
#include "zones/CelluloZoneEngine.h"
#include "zones/CelluloZoneJsonHandler.h"
#include "zones/CelluloZoneTypes.h"
#include "zones/CelluloZonePaintedItem.h"
#include "util/math/CelluloMathUtil.h"
#include "util/math/PolyBezier.h"
#include "util/svg/CelluloSVGUtil.h"
#include "util/lang/TranslationTool.h"
#include "util/system/CelluloSystemUtil.h"
#include "util/ui/CelluloUIUtil.h"
#include "util/ui/ProgressCircle.h"
#include "tile/OffsetRemapper.h"
#include "tile/HexTileMap.h"
#include "tile/HexTile.h"
#include "tile/HexTileStdSourceCoords.h"
#include "tile/AxialHexCoords.h"
#include "tile/CoordSpaceConverter.h"

/**
 * @brief Scope for core Cellulo objects
 */
namespace Cellulo{

void CelluloPlugin::registerTypes(const char *uri){
    qmlRegisterType<CelluloRobot>(uri, 1, 0, "CelluloRobot");
    qmlRegisterType<PolyBezierTrackerConstVel>(uri, 1, 0, "PolyBezierTrackerConstVel");
    qmlRegisterType<PolyBezierTrackerConstAccel>(uri, 1, 0, "PolyBezierTrackerConstAccel");
    qmlRegisterType<PolyBezierTrackerAdaptiveVel>(uri, 1, 0, "PolyBezierTrackerAdaptiveVel");
    qmlRegisterType<PolyBezierTrackerProfiledVel>(uri, 1, 0, "PolyBezierTrackerProfiledVel");

    qmlRegisterType<CelluloBluetooth>(uri, 1, 0, "CelluloBluetooth");
    qmlRegisterType<CelluloLocalRelayClient>(uri, 1, 0, "CelluloLocalRelayClient");
    qmlRegisterType<CelluloLocalRelayServer>(uri, 1, 0, "CelluloLocalRelayServer");
    qmlRegisterType<CelluloRobotPoolClient>(uri, 1, 0, "CelluloRobotPoolClient");
    qmlRegisterType<CelluloRobotHubClient>(uri, 1, 0, "CelluloRobotHubClient");
    qmlRegisterType<CelluloTcpRelayClient>(uri, 1, 0, "CelluloTcpRelayClient");
    qmlRegisterType<CelluloTcpRelayServer>(uri, 1, 0, "CelluloTcpRelayServer");
    qmlRegisterType<CelluloBluetoothEMP>(uri, 1, 0, "CelluloBluetoothEMP");
    qmlRegisterType<CelluloBluetoothScanner>(uri, 1, 0, "CelluloBluetoothScanner");

    qmlRegisterSingletonType<CelluloBluetoothEnums>(uri, 1, 0, "CelluloBluetoothEnums",
                                                    [] (QQmlEngine* qmlEngine, QJSEngine* jsEngine)->QObject* {
                                                        Q_UNUSED(qmlEngine)
                                                        Q_UNUSED(jsEngine)
                                                        return new CelluloBluetoothEnums();
                                                    });
    qmlRegisterSingletonType<CelluloCommUtil>(uri, 1, 0, "CelluloCommUtil",
                                                [] (QQmlEngine* qmlEngine, QJSEngine* jsEngine)->QObject* {
                                                    Q_UNUSED(qmlEngine)
                                                    Q_UNUSED(jsEngine)
                                                    return new CelluloCommUtil();
                                                });

    qmlRegisterType<CelluloZoneClient>(uri, 1, 0, "CelluloZoneClient");
    qmlRegisterType<CelluloZoneEngine>(uri, 1, 0, "CelluloZoneEngine");
    qmlRegisterSingletonType<CelluloZoneJsonHandler>(uri, 1, 0, "CelluloZoneJsonHandler",
                                                     [] (QQmlEngine* qmlEngine, QJSEngine* jsEngine)->QObject* {
                                                         Q_UNUSED(qmlEngine)
                                                         Q_UNUSED(jsEngine)
                                                         return new CelluloZoneJsonHandler();
                                                     });
    qmlRegisterSingletonType<CelluloZoneTypes>(uri, 1, 0, "CelluloZoneTypes",
                                               [] (QQmlEngine* qmlEngine, QJSEngine* jsEngine)->QObject* {
                                                   Q_UNUSED(qmlEngine)
                                                   Q_UNUSED(jsEngine)
                                                   return new CelluloZoneTypes();
                                               });

    qmlRegisterType<CelluloZoneCircleInner>(uri, 1, 0, "CelluloZoneCircleInner");
    qmlRegisterType<CelluloZoneCircleBorder>(uri, 1, 0, "CelluloZoneCircleBorder");
    qmlRegisterType<CelluloZoneCircleDistance>(uri, 1, 0, "CelluloZoneCircleDistance");

    qmlRegisterType<CelluloZoneRectangleInner>(uri, 1, 0, "CelluloZoneRectangleInner");
    qmlRegisterType<CelluloZoneRectangleBorder>(uri, 1, 0, "CelluloZoneRectangleBorder");
    qmlRegisterType<CelluloZoneRectangleDistance>(uri, 1, 0, "CelluloZoneRectangleDistance");

    qmlRegisterType<CelluloZoneAngleDifference>(uri, 1, 0, "CelluloZoneAngleDifference");

    qmlRegisterType<CelluloZoneAngleIntervalInner>(uri, 1, 0, "CelluloZoneAngleIntervalInner");
    qmlRegisterType<CelluloZoneAngleIntervalBorder>(uri, 1, 0, "CelluloZoneAngleIntervalBorder");
    qmlRegisterType<CelluloZoneAngleIntervalDistance>(uri, 1, 0, "CelluloZoneAngleIntervalDistance");

    qmlRegisterType<CelluloZoneLineSegmentDistance>(uri, 1, 0, "CelluloZoneLineSegmentDistance");
    qmlRegisterType<CelluloZoneLineDistanceSigned>(uri, 1, 0, "CelluloZoneLineDistanceSigned");
    qmlRegisterType<CelluloZoneLineBorder>(uri, 1, 0, "CelluloZoneLineBorder");

    qmlRegisterType<CelluloZonePointDistance>(uri, 1, 0, "CelluloZonePointDistance");

    qmlRegisterType<CelluloZoneIrregularPolygonInner>(uri, 1, 0, "CelluloZoneIrregularPolygonInner");
    qmlRegisterType<CelluloZoneIrregularPolygonBorder>(uri, 1, 0, "CelluloZoneIrregularPolygonBorder");
    qmlRegisterType<CelluloZoneIrregularPolygonDistance>(uri, 1, 0, "CelluloZoneIrregularPolygonDistance");

    qmlRegisterType<CelluloZoneRegularPolygonInner>(uri, 1, 0, "CelluloZoneRegularPolygonInner");
    qmlRegisterType<CelluloZoneRegularPolygonBorder>(uri, 1, 0, "CelluloZoneRegularPolygonBorder");
    qmlRegisterType<CelluloZoneRegularPolygonDistance>(uri, 1, 0, "CelluloZoneRegularPolygonDistance");

    qmlRegisterType<CelluloZonePolyBezierDistance>(uri, 1, 0, "CelluloZonePolyBezierDistance");
    qmlRegisterType<CelluloZonePolyBezierBorder>(uri, 1, 0, "CelluloZonePolyBezierBorder");
    qmlRegisterType<CelluloZonePolyBezierInner>(uri, 1, 0, "CelluloZonePolyBezierInner");
    qmlRegisterType<CelluloZonePolyBezierClosestT>(uri, 1, 0, "CelluloZonePolyBezierClosestT");
    qmlRegisterType<CelluloZonePolyBezierXT>(uri, 1, 0, "CelluloZonePolyBezierXT");
    qmlRegisterType<CelluloZonePolyBezierYT>(uri, 1, 0, "CelluloZonePolyBezierYT");

    qmlRegisterType<CelluloZonePaintedItem>(uri, 1, 0, "CelluloZonePaintedItem");

    qmlRegisterSingletonType<CelluloMathUtil>(uri, 1, 0, "CelluloMathUtil",
                                              [] (QQmlEngine* qmlEngine, QJSEngine* jsEngine)->QObject* {
                                                  Q_UNUSED(qmlEngine)
                                                  Q_UNUSED(jsEngine)
                                                  return new CelluloMathUtil();
                                              });
    qmlRegisterType<PolyBezier>(uri, 1, 0, "PolyBezier");

    qmlRegisterSingletonType<CelluloSVGUtil>(uri, 1, 0, "CelluloSVGUtil",
                                             [] (QQmlEngine* qmlEngine, QJSEngine* jsEngine)->QObject* {
                                                 Q_UNUSED(qmlEngine)
                                                 Q_UNUSED(jsEngine)
                                                 return new CelluloSVGUtil();
                                             });

    qmlRegisterType<TranslationTool>(uri, 1, 0, "TranslationTool");

    qmlRegisterSingletonType<CelluloSystemUtil>(uri, 1, 0, "CelluloSystemUtil",
                                                [] (QQmlEngine* qmlEngine, QJSEngine* jsEngine)->QObject* {
                                                    Q_UNUSED(qmlEngine)
                                                    Q_UNUSED(jsEngine)
                                                    return new CelluloSystemUtil();
                                                });

    qmlRegisterSingletonType<CelluloUIUtil>(uri, 1, 0, "CelluloUIUtil",
                                                [] (QQmlEngine* qmlEngine, QJSEngine* jsEngine)->QObject* {
                                                    Q_UNUSED(qmlEngine)
                                                    Q_UNUSED(jsEngine)
                                                    return new CelluloUIUtil();
                                                });
    qmlRegisterType<ProgressCircle>(uri, 1, 0, "ProgressCircle");

    qmlRegisterType<OffsetRemapper>(uri, 1, 0, "OffsetRemapper");
    qmlRegisterType<HexTileMap>(uri, 1, 0, "HexTileMap");
    qmlRegisterType<HexTile>(uri, 1, 0, "HexTile");
    qmlRegisterType<HexTileStdSourceCoords>(uri, 1, 0, "HexTileStdSourceCoords");
    qmlRegisterType<AxialHexCoords>(uri, 1, 0, "AxialHexCoords");
    qmlRegisterType<CoordSpaceConverter>(uri, 1, 0, "CoordSpaceConverter");
}

void CelluloPlugin::initializeEngine(QQmlEngine *engine, const char *uri){
    Q_UNUSED(uri)
    engine->addImageProvider("cameraFrame", new CameraFrameImageProvider());
}

}
