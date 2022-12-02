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

    property bool mobile: Qt.platform.os === "android"
    property real gWidth: mobile ? Screen.width : 800
    width: gWidth
    height: mobile ? Screen.desktopAvailableHeight : 0.7*Screen.height

    CelluloRobot{
        id: robotComm

        property vector2d trackedPosition: {
            if(trackerConstVel.enabled)
                return trackerConstVel.trackedPose.toVector2d();
            if(trackerConstAccel.enabled)
                return trackerConstAccel.trackedPose.toVector2d();
            if(trackerAdaptiveVel.enabled)
                return trackerAdaptiveVel.trackedPose.toVector2d();
            if(trackerProfiledVel.enabled)
                return trackerProfiledVel.trackedPose.toVector2d();
            return Qt.vector2d(0,0);
        }

        property vector2d trackedVelocity: {
            if(trackerConstVel.enabled)
                return trackerConstVel.trackedVelocity.toVector2d();
            if(trackerConstAccel.enabled)
                return trackerConstAccel.trackedVelocity.toVector2d();
            if(trackerAdaptiveVel.enabled)
                return trackerAdaptiveVel.trackedVelocity.toVector2d();
            if(trackerProfiledVel.enabled)
                return trackerProfiledVel.trackedVelocity.toVector2d();
            return Qt.vector2d(0,0);
        }
    }

    PolyBezierTrackerConstVel{
        id: trackerConstVel

        curve: pbcurve

        trackingVelocity: parseFloat(constVelText.text)

        goToStartFirst: goToStartFirstCheckbox.checked
        stopWhenGoalReached: stopWhenGoalReachedCheckbox.checked
        cleanCurve: cleanCurveCheckbox.checked
        controlPeriod: parseFloat(controlPeriodText.text)

        onStartReached: console.info("Start of the curve reached.")
        onEndReached: console.info("End of the curve reached.")
    }

    PolyBezierTrackerConstAccel{
        id: trackerConstAccel

        curve: pbcurve

        trackingVelocity: parseFloat(constAccelVelText.text)
        trackingAcceleration: parseFloat(constAccelAccelText.text)

        goToStartFirst: goToStartFirstCheckbox.checked
        stopWhenGoalReached: stopWhenGoalReachedCheckbox.checked
        cleanCurve: cleanCurveCheckbox.checked
        controlPeriod: parseFloat(controlPeriodText.text)

        onStartReached: console.info("Start of the curve reached.")
        onEndReached: console.info("End of the curve reached.")
    }

    PolyBezierTrackerAdaptiveVel{
        id: trackerAdaptiveVel

        curve: pbcurve

        maxTrackingVelocity: parseFloat(adaptiveMaxVelText.text)
        minTrackingVelocity: parseFloat(adaptiveMinVelText.text)

        goToStartFirst: goToStartFirstCheckbox.checked
        stopWhenGoalReached: stopWhenGoalReachedCheckbox.checked
        cleanCurve: cleanCurveCheckbox.checked
        controlPeriod: parseFloat(controlPeriodText.text)

        onStartReached: console.info("Start of the curve reached.")
        onEndReached: console.info("End of the curve reached.")
    }

    PolyBezierTrackerProfiledVel{
        id: trackerProfiledVel

        curve: pbcurve

        trackingVelocityProfile: profiledVelText.text.split(",").map(function(v){return parseFloat(v)})

        goToStartFirst: goToStartFirstCheckbox.checked
        stopWhenGoalReached: stopWhenGoalReachedCheckbox.checked
        cleanCurve: cleanCurveCheckbox.checked
        controlPeriod: parseFloat(controlPeriodText.text)

        onStartReached: console.info("Start of the curve reached.")
        onEndReached: console.info("End of the curve reached.")
    }

    PolyBezier{
        id: pbcurve

        Component.onCompleted: reset()

        function reset(){ loadFromFile(":/assets/curve.json"); }
    }

    ScrollView {
        anchors.fill: parent
        horizontalScrollBarPolicy: Qt.ScrollBarAlwaysOff
        verticalScrollBarPolicy: mobile ? Qt.ScrollBarAsNeeded : Qt.ScrollBarAlwaysOff

        Column{

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
                                pbcurve.appendPoint(point);
                                console.log("Point added: " + point);
                            }
                        }

                        PolyBezierVisual{
                            anchors.fill: parent
                            scaleCoeff: page.scaleCoeff
                            controlPoints: pbcurve.controlPoints
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
                            x: robotComm.trackedPosition.x*parent.scaleCoeff - width/2
                            y: robotComm.trackedPosition.y*parent.scaleCoeff - height/2
                            height: 10*parent.scaleCoeff
                            width: 10*parent.scaleCoeff
                            transformOrigin: Item.Left
                            color: "#800000FF"
                            radius: 5*parent.scaleCoeff
                            z: 1
                        }

                        Rectangle{
                            x: robotComm.trackedPosition.x*parent.scaleCoeff
                            y: robotComm.trackedPosition.y*parent.scaleCoeff
                            height: 3*parent.scaleCoeff
                            width: 30*parent.scaleCoeff
                            transformOrigin: Item.Left
                            rotation: Math.atan2(robotComm.trackedVelocity.y, robotComm.trackedVelocity.x)/Math.PI*180
                            color: "#80FF0000"
                            z: 1
                        }
                    }
                }
            }

            GroupBox{
                id: controlsBox
                title: "Controls"

                Column{
                    spacing: 5

                    Row{
                        spacing: 10

                        Button{
                            text: "Reset curve"
                            onClicked: pbcurve.reset()
                            anchors.verticalCenter: parent.verticalCenter
                        }

                        CheckBox{
                            id: goToStartFirstCheckbox
                            text: "Go to start of the curve first"
                            checked: true
                            anchors.verticalCenter: parent.verticalCenter
                        }

                        CheckBox{
                            id: stopWhenGoalReachedCheckbox
                            text: "Stop tracker when end is reached"
                            checked: false
                            anchors.verticalCenter: parent.verticalCenter
                        }

                        CheckBox{
                            id: cleanCurveCheckbox
                            text: "Clean up curve as it is tracked"
                            checked: false
                            anchors.verticalCenter: parent.verticalCenter
                        }

                        TextField{
                            id: controlPeriodText
                            text: "100"
                            placeholderText: "Control period (ms)"
                            anchors.verticalCenter: parent.verticalCenter
                        }
                    }

                    Row{
                        spacing: 5

                        Text{
                            text: "(1) Constant velocity tracker:"
                            anchors.verticalCenter: parent.verticalCenter
                        }

                        TextField{
                            id: constVelText
                            text: "75"
                            placeholderText: "Velocity (mm/s)"
                            anchors.verticalCenter: parent.verticalCenter
                        }

                        Button{
                            text: "Start"
                            anchors.verticalCenter: parent.verticalCenter
                            onClicked: {
                                trackerConstAccel.enabled = false;
                                trackerConstAccel.robot = null;
                                trackerAdaptiveVel.enabled = false;
                                trackerAdaptiveVel.robot = null;
                                trackerProfiledVel.enabled = false;
                                trackerProfiledVel.robot = null;

                                trackerConstVel.robot = robotComm;
                                trackerConstVel.startTracking();
                            }
                        }

                        Button{
                            text: "Stop"
                            anchors.verticalCenter: parent.verticalCenter
                            onClicked: trackerConstVel.enabled = false
                        }

                        Text{
                            text: trackerConstVel.enabled ? "Running: " + (trackerConstVel.trackingPercentage*100).toFixed(2) + "%" : "Not running"
                            color: trackerConstVel.enabled ? "green" : "black"
                            anchors.verticalCenter: parent.verticalCenter
                        }
                    }

                    Row{
                        spacing: 5

                        Text{
                            text: "(2) Constant acceleration tracker:"
                            anchors.verticalCenter: parent.verticalCenter
                        }

                        TextField{
                            id: constAccelVelText
                            text: "75"
                            placeholderText: "Velocity (mm/s)"
                            anchors.verticalCenter: parent.verticalCenter
                        }

                        TextField{
                            id: constAccelAccelText
                            text: "50"
                            placeholderText: "Acceleration (mm/s^2)"
                            anchors.verticalCenter: parent.verticalCenter
                        }

                        Button{
                            text: "Start"
                            anchors.verticalCenter: parent.verticalCenter
                            onClicked: {
                                trackerConstVel.enabled = false;
                                trackerConstVel.robot = null;
                                trackerAdaptiveVel.enabled = false;
                                trackerAdaptiveVel.robot = null;
                                trackerProfiledVel.enabled = false;
                                trackerProfiledVel.robot = null;

                                trackerConstAccel.robot = robotComm;
                                trackerConstAccel.startTracking();
                            }
                        }

                        Button{
                            text: "Stop"
                            anchors.verticalCenter: parent.verticalCenter
                            onClicked: trackerConstAccel.enabled = false
                        }

                        Text{
                            text: trackerConstAccel.enabled ? "Running: " + (trackerConstAccel.trackingPercentage*100).toFixed(2) + "%" : "Not running"
                            color: trackerConstAccel.enabled ? "green" : "black"
                            anchors.verticalCenter: parent.verticalCenter
                        }
                    }

                    Row{
                        spacing: 5

                        Text{
                            text: "(3) Adaptive velocity tracker:"
                            anchors.verticalCenter: parent.verticalCenter
                        }

                        TextField{
                            id: adaptiveMaxVelText
                            text: "100"
                            placeholderText: "Max velocity (mm/s)"
                            anchors.verticalCenter: parent.verticalCenter
                        }

                        TextField{
                            id: adaptiveMinVelText
                            text: "30"
                            placeholderText: "Min velocity (mm/s)"
                            anchors.verticalCenter: parent.verticalCenter
                        }

                        Button{
                            text: "Start"
                            anchors.verticalCenter: parent.verticalCenter
                            onClicked: {
                                trackerConstVel.enabled = false;
                                trackerConstVel.robot = null;
                                trackerConstAccel.enabled = false;
                                trackerConstAccel.robot = null;
                                trackerProfiledVel.enabled = false;
                                trackerProfiledVel.robot = null;

                                trackerAdaptiveVel.robot = robotComm;
                                trackerAdaptiveVel.startTracking();
                            }
                        }

                        Button{
                            text: "Stop"
                            anchors.verticalCenter: parent.verticalCenter
                            onClicked: trackerAdaptiveVel.enabled = false
                        }

                        Text{
                            text: trackerAdaptiveVel.enabled ? "Running: " + (trackerAdaptiveVel.trackingPercentage*100).toFixed(2) + "%" : "Not running"
                            color: trackerAdaptiveVel.enabled ? "green" : "black"
                            anchors.verticalCenter: parent.verticalCenter
                        }
                    }

                    Row{
                        spacing: 5

                        Text{
                            text: "(4) Profiled velocity tracker:"
                            anchors.verticalCenter: parent.verticalCenter
                        }

                        TextField{
                            id: profiledVelText
                            placeholderText: "Equidistant comma separated velocity list (mm/s)"
                            text: "50,150,150,30,75,150,150,40,100,75,30"
                            width: 300
                            anchors.verticalCenter: parent.verticalCenter
                        }

                        Button{
                            text: "Start"
                            anchors.verticalCenter: parent.verticalCenter
                            onClicked: {
                                trackerConstVel.enabled = false;
                                trackerConstVel.robot = null;
                                trackerConstAccel.enabled = false;
                                trackerConstAccel.robot = null;
                                trackerAdaptiveVel.enabled = false;
                                trackerAdaptiveVel.robot = null;

                                trackerProfiledVel.robot = robotComm;
                                trackerProfiledVel.startTracking();
                            }
                        }

                        Button{
                            text: "Stop"
                            anchors.verticalCenter: parent.verticalCenter
                            onClicked: trackerProfiledVel.enabled = false
                        }

                        Text{
                            text: trackerProfiledVel.enabled ? "Running: " + (trackerProfiledVel.trackingPercentage*100).toFixed(2) + "%" : "Not running"
                            color: trackerProfiledVel.enabled ? "green" : "black"
                            anchors.verticalCenter: parent.verticalCenter
                        }
                    }
                }
            }
        }
    }
}
