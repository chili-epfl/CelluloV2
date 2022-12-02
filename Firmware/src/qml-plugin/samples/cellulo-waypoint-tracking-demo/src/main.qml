import QtQuick 2.2
import QtQuick.Shapes 1.0
import QtQuick.Window 2.1
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.3

import Cellulo 1.0
import QMLCache 1.0

ApplicationWindow {
    id: root
    visible: true
    minimumHeight: 100
    minimumWidth: 200

    CelluloRobot{
        id: robotComm

        PolyBezierTrackerConstAccel{
            id: tracker

            PolyBezier{ id: curve }

            trackingVelocity: parseFloat(velText.text)
            trackingAcceleration: parseFloat(accelText.text)

            goToStartFirst: true
            stopWhenGoalReached: false
            cleanCurve: true

            controlPeriod: 50

            onEndReached: console.info("End reached for now.")
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
        title: "Playground (Click somewhere to add a point to the curve)"
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

                MouseArea{
                    anchors.fill: parent
                    onClicked: {
                        var point = Qt.vector2d(mouse.x/page.scaleCoeff, mouse.y/page.scaleCoeff);
                        if(curve.isEmpty())
                            curve.start(Qt.vector2d(robotComm.x, robotComm.y), point);
                        else
                            curve.appendPoint(point);
                        console.log("Point added: " + point);
                    }
                }

                PolyBezierVisual{
                    anchors.fill: parent
                    scaleCoeff: page.scaleCoeff
                    controlPoints: curve.controlPoints
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

                Rectangle{
                    x: tracker.trackedPose.x*parent.scaleCoeff - width/2
                    y: tracker.trackedPose.y*parent.scaleCoeff - height/2
                    height: 10*parent.scaleCoeff
                    width: 10*parent.scaleCoeff
                    transformOrigin: Item.Left
                    color: "#800000FF"
                    radius: 5*parent.scaleCoeff
                    z: 1
                }

                Rectangle{
                    x: tracker.trackedPose.x*parent.scaleCoeff
                    y: tracker.trackedPose.y*parent.scaleCoeff
                    height: 3*parent.scaleCoeff
                    width: 30*parent.scaleCoeff
                    transformOrigin: Item.Left
                    rotation: Math.atan2(tracker.trackedVelocity.y, tracker.trackedVelocity.x)/Math.PI*180
                    color: "#80FF0000"
                    z: 1
                }
            }
        }
    }

    GroupBox{
        id: controlsBox
        title: "Controls"
        anchors.top: playgroundBox.bottom

        Column{
            spacing: 5

            Row{
                spacing: 5

                Button{
                    text: "Clear waypoints"
                    onClicked: curve.clear()
                    anchors.verticalCenter: parent.verticalCenter
                }

                Text{
                    text: "Velocity:"
                    anchors.verticalCenter: parent.verticalCenter
                }

                TextField{
                    id: velText
                    text: "100"
                    placeholderText: "Velocity (mm/s)"
                    anchors.verticalCenter: parent.verticalCenter
                }

                Text{
                    text: "Acceleration:"
                    anchors.verticalCenter: parent.verticalCenter
                }

                TextField{
                    id: accelText
                    text: "100"
                    placeholderText: "Acceleration (mm/s^2)"
                    anchors.verticalCenter: parent.verticalCenter
                }

                Button{
                    text: "Start"
                    anchors.verticalCenter: parent.verticalCenter
                    onClicked: tracker.startTracking()
                }

                Button{
                    text: "Stop"
                    anchors.verticalCenter: parent.verticalCenter
                    onClicked: tracker.enabled = false
                }

                Text{
                    text: tracker.enabled ? "Running: " + (tracker.trackingPercentage*100).toFixed(2) + "%" : "Not running"
                    color: tracker.enabled ? "green" : "black"
                    anchors.verticalCenter: parent.verticalCenter
                }
            }
        }
    }
}
