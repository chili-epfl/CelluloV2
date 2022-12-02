import QtQuick 2.2
import QtQuick.Window 2.1
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.3
import QtBluetooth 5.2
import Cellulo 1.0
import QMLCache 1.0

ApplicationWindow {
    id: root
    visible: true
    minimumHeight: 100
    minimumWidth: 200

    CelluloZoneEngine{
        id: zoneEngine

        property var zoneClosestT: null
        property vector2d closestPoint: Qt.vector2d(0,0)
        property vector2d closestPointTangent: Qt.vector2d(0,0)
        property vector2d closestPointNormal: Qt.vector2d(0,0)

        Component.onCompleted: {
            var zones = CelluloZoneJsonHandler.loadZonesQML(":/assets/zones.json");
            for(var i=0;i<zones.length;i++){
                switch(zones[i].name){
                case "ZONE_INNER":
                    zones[i].createPaintedItem(page, "#80FF0000", page.physicalWidth, page.physicalHeight);
                    break;
                case "ZONE_BORDER":
                    zones[i].createPaintedItem(page, "#800000FF", page.physicalWidth, page.physicalHeight);
                    break;
                case "ZONE_DISTANCE":
                    zones[i].createPaintedItem(page, "#8000FF00", page.physicalWidth, page.physicalHeight);
                    break;
                case "ZONE_CLOSEST_T":
                    zoneClosestT = zones[i];
                    break;
                default:
                    break;
                }
            }
            addNewZones(zones);
        }
    }

    CelluloRobot{
        id: robotComm

        property bool inInner: false
        property bool inBorder: false

        Component.onCompleted: zoneEngine.addNewClient(robotComm)

        onConnectionStatusChanged: {
            if(connectionStatus === CelluloBluetoothEnums.ConnectionStatusConnected){
                setCasualBackdriveAssistEnabled(true);
                setPoseBcastPeriod(0);
            }
        }

        onTrackingGoalReached: {
            setCasualBackdriveAssistEnabled(true);
            clearTracking();
        }

        onZoneValueChanged: {
            switch(zone.name){
            case "ZONE_INNER":
                inInner = value;
                updateColors();
                break;
            case "ZONE_BORDER":
                inBorder = value;
                updateColors();
                break;
            case "ZONE_DISTANCE":
                distText.text = "Distance: " + value.toFixed(6) + "mm"
                break;
            case "ZONE_CLOSEST_T":
                zoneEngine.closestPoint = zoneEngine.zoneClosestT.getPoint(value);
                zoneEngine.closestPointNormal = zoneEngine.zoneClosestT.getNormal(value);
                zoneEngine.closestPointTangent = zoneEngine.zoneClosestT.getTangent(value);
                break;
            default:
                break;
            }
        }

        function updateColors(){
            if(inInner && inBorder)
                setVisualEffect(CelluloBluetoothEnums.VisualEffectConstAll, "#FF00FF", 0);
            else if(inInner)
                setVisualEffect(CelluloBluetoothEnums.VisualEffectConstAll, "#FF0000", 0);
            else if(inBorder)
                setVisualEffect(CelluloBluetoothEnums.VisualEffectConstAll, "#0000FF", 0);
            else
                setVisualEffect(CelluloBluetoothEnums.VisualEffectConstAll, "#FFFFFF", 0);
        }
    }

    //Visible items

    GroupBox {
        id: addressBox
        title: "Robot"

        Column{
            spacing: 5

            CelluloBluetoothScanner{
                id: scanner
                onRobotDiscovered: {
                    var newAddresses = macAddrSelector.addresses;
                    if(newAddresses.indexOf(macAddr) < 0){
                        console.log(macAddr + " discovered.");
                        newAddresses.push(macAddr);
                        newAddresses.sort();
                    }
                    macAddrSelector.addresses = newAddresses;
                    QMLCache.write("addresses", macAddrSelector.addresses.join(','));
                }
            }

            Row{
                spacing: 5

                MacAddrSelector{
                    id: macAddrSelector
                    addresses: QMLCache.read("addresses").split(",")
                    onConnectRequested: {
                        robotComm.localAdapterMacAddr = selectedLocalAdapterAddress;
                        robotComm.macAddr = selectedAddress;
                    }
                    onDisconnectRequested: robotComm.disconnectFromServer()
                    connectionStatus: robotComm.connectionStatus
                }

                Button{
                    text: "Reset"
                    onClicked: robotComm.reset()
                }
            }

            Row{
                spacing: 5

                BusyIndicator{
                    running: scanner.scanning
                    height: scanButton.height
                }

                Button{
                    id: scanButton
                    text: "Scan"
                    onClicked: scanner.start()
                }

                Button{
                    text: "Clear List"
                    onClicked: {
                        macAddrSelector.addresses = [];
                        QMLCache.write("addresses","");
                    }
                }
            }
        }
    }

    GroupBox{
        id: playgroundBox
        title: "Playground"
        anchors.top: addressBox.bottom

        Item{
            width: page.width + page.physicalRobotWidth*page.scaleCoeff
            height: page.height + page.physicalRobotWidth*page.scaleCoeff

            Rectangle{
                id: page

                anchors.centerIn: parent

                property real physicalWidth: 210        ///< Physical paper width in mm
                property real physicalHeight: 297       ///< Physical paper height in mm
                property real physicalRobotWidth: 75    ///< Physical robot width in mm

                width: height*physicalWidth/physicalHeight
                height: Screen.height/2

                property real scaleCoeff: width/physicalWidth

                color: "#EEEEEE"
                border.color: "black"
                border.width: 2
                radius: 5

                Rectangle{
                    x: zoneEngine.closestPoint.x*parent.scaleCoeff
                    y: zoneEngine.closestPoint.y*parent.scaleCoeff
                    height: 3*parent.scaleCoeff
                    width: 30*parent.scaleCoeff
                    transformOrigin: Item.TopLeft
                    rotation: Math.atan2(zoneEngine.closestPointTangent.y, zoneEngine.closestPointTangent.x)/Math.PI*180
                    color: "#800000FF"
                    radius: 5
                    z: 1
                }

                Rectangle{
                    x: zoneEngine.closestPoint.x*parent.scaleCoeff
                    y: zoneEngine.closestPoint.y*parent.scaleCoeff
                    height: 3*parent.scaleCoeff
                    width: 30*parent.scaleCoeff
                    transformOrigin: Item.TopLeft
                    rotation: Math.atan2(zoneEngine.closestPointNormal.y, zoneEngine.closestPointNormal.x)/Math.PI*180
                    color: "#80FF0000"
                    z: 1
                }

                Rectangle{
                    x: zoneEngine.closestPoint.x*parent.scaleCoeff - width/2
                    y: zoneEngine.closestPoint.y*parent.scaleCoeff - height/2
                    height: parent.scaleCoeff*zoneEngine.closestPointTangent.length
                    rotation: Math.atan2(zoneEngine.closestPointTangent.y, zoneEngine.closestPointTangent.x)/Math.PI*180
                    width: 3
                    color: "#800000FF"
                    z: 1
                }

                Image{
                    source: robotComm.kidnapped ? "../assets/redHexagon.svg" : "../assets/greenHexagon.svg"
                    rotation: robotComm.theta
                    x: robotComm.x*parent.scaleCoeff - width/2
                    y: robotComm.y*parent.scaleCoeff - height/2
                    width: parent.physicalRobotWidth*parent.scaleCoeff
                    fillMode: Image.PreserveAspectFit

                    Rectangle{
                        x: parent.width/2 - width/2
                        y: parent.height/2 - height/2
                        height: 5
                        width: 5
                        color: "black"
                        border.color: "black"
                        border.width: 1
                        radius: 2.5
                    }
                }
            }
        }
    }

    Text{
        id: distText
        anchors.top: playgroundBox.bottom
    }
}
