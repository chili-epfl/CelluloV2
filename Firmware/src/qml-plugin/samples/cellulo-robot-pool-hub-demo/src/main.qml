import QtQuick 2.2
import QtQuick.Window 2.1
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.3
import QtBluetooth 5.2

import QMLCache 1.0
import Cellulo 1.0

ApplicationWindow {
    id: window
    visible: true
    minimumHeight: 100
    minimumWidth: 200

    property int currentNumRobots: client.robots.length

    property vector3d latticePose: Qt.vector3d(200, 500, 0)
    property real latticeDist: 80
    property var touchedRobot1: null
    property var touchedRobot2: null
    property int touchedRobot1Index: -1
    property int touchedRobot2Index: -1
    property int nearestSquareEdge: Math.ceil(Math.sqrt(currentNumRobots))
    property bool goChecked: go.checked

    property var client: poolButton.checked ? poolClient : hubClient

    CelluloRobotPoolClient{
        id: poolClient

        robotComponent: Qt.createComponent("LatticeRobot.qml")
        onNewRobotFound: {
            robot.index = robots.length - 1;
            robot.root = window;
        }

        autoConnect: poolButton.checked
        onAutoConnectChanged: {
            if(!autoConnect)
                disconnectFromServer();
        }
    }

    CelluloRobotHubClient{
        id: hubClient

        serverAddress: hubAddress.text
        port: parseInt(hubPort.text)

        robotComponent: Qt.createComponent("LatticeRobot.qml")
        onNewRobotFound: {
            robot.index = robots.length - 1;
            robot.root = window;
        }

        autoConnect: hubButton.checked
        onAutoConnectChanged: {
            if(!autoConnect)
                disconnectFromServer();
        }
    }

    function initTouched(robot){
        robot.setVisualEffect(CelluloBluetoothEnums.VisualEffectConstAll, "#202000", 0);
        robot.setHapticBackdriveAssist(0.7, 0.7, 0.7);
    }

    function initReleased(robot){
        robot.setVisualEffect(CelluloBluetoothEnums.VisualEffectConstAll, "#202020", 0);
        robot.setHapticBackdriveAssist(0,0,0);
    }

    function recalcTouchedRobots(){
        touchedRobot1 = null;
        touchedRobot2 = null;

        for(var i=0;i<client.robots.length;i++)
            if(client.robots[i].anyKey()){
                if(touchedRobot1 === null){
                    touchedRobot1 = client.robots[i];
                    touchedRobot1Index = i;
                    initTouched(touchedRobot1);
                }
                else{
                    touchedRobot2 = client.robots[i];
                    touchedRobot2Index = i;
                    initTouched(touchedRobot2);
                    break;
                }
            }
            else
                initReleased(client.robots[i]);
    }

    function recalcResize(){
        var r1X = touchedRobot1Index % nearestSquareEdge;
        var r1Y = (touchedRobot1Index - r1X)/nearestSquareEdge;
        var r2X = touchedRobot2Index % nearestSquareEdge;
        var r2Y = (touchedRobot2Index - r2X)/nearestSquareEdge;
        var unitXDiff = r2X - r1X;
        var unitYDiff = r2Y - r1Y;
        var originalLatticeAngle = Math.atan2(unitYDiff, unitXDiff);
        var unitDistance = Math.sqrt(unitXDiff*unitXDiff + unitYDiff*unitYDiff);

        var yDiff = touchedRobot2.y - touchedRobot1.y;
        var xDiff = touchedRobot2.x - touchedRobot1.x;
        var angle = Math.atan2(yDiff, xDiff) - originalLatticeAngle;
        latticePose.z = angle/Math.PI*180;

        latticeDist = Math.sqrt(xDiff*xDiff + yDiff*yDiff)/unitDistance;
    }

    //Visible items

    GroupBox {
        id: serverBox
        title: "Controls"

        Column{
            Row{
                spacing: 5

                ExclusiveGroup{ id: serverGroup }
                RadioButton{
                    id: poolButton
                    text: "Pool (Local)"
                    checked: true
                    exclusiveGroup: serverGroup
                }
                RadioButton{
                    id: hubButton
                    text: "Hub (Remote, e.g at RPi)"
                    exclusiveGroup: serverGroup
                }
            }

            GroupBox {
                title: "Hub Address"
                enabled: hubButton.checked

                Row{
                    spacing: 5

                    Text{
                        text: "Address:"
                        anchors.verticalCenter: parent.verticalCenter
                    }

                    TextField{
                        id: hubAddress
                        text: "192.168.2.1"
                    }

                    Text{
                        text: "Port:"
                        anchors.verticalCenter: parent.verticalCenter
                    }

                    TextField{
                        id: hubPort
                        text: "2556"
                    }
                }
            }

            Text{
                text: {
                    switch(client.connectionStatus){
                    case CelluloCommUtil.RelayConnectionStatusDisconnected: return "Disconnected.";
                    case CelluloCommUtil.RelayConnectionStatusConnecting: return "Connecting to Server...";
                    case CelluloCommUtil.RelayConnectionStatusConnected: return "Connected to Server.";
                    }
                }
                color: {
                    switch(client.connectionStatus){
                    case CelluloCommUtil.RelayConnectionStatusDisconnected: return "red";
                    case CelluloCommUtil.RelayConnectionStatusConnecting: return "yellow";
                    case CelluloCommUtil.RelayConnectionStatusConnected: return "green";
                    }
                }
            }

            CheckBox{
                id: go
                text: "Go!"
                checked: false
            }
        }
    }

    GroupBox{
        id: playgroundBox
        title: "Playground"
        anchors.top: serverBox.bottom

        Item{
            width: page.width + page.physicalRobotWidth*page.scaleCoeff
            height: page.height + page.physicalRobotWidth*page.scaleCoeff

            Rectangle{
                id: page

                anchors.centerIn: parent

                property real physicalWidth: 841        ///< Physical paper width in mm
                property real physicalHeight: 841       ///< Physical paper height in mm
                property real physicalRobotWidth: 75    ///< Physical robot width in mm

                width: height*physicalWidth/physicalHeight
                height: Screen.height/3

                property real scaleCoeff: width/physicalWidth

                color: "#EEEEEE"
                border.color: "black"
                border.width: 2
                radius: 5

                Repeater{
                    model: currentNumRobots

                    Image{
                        visible: client.robots[index].connectionStatus === CelluloBluetoothEnums.ConnectionStatusConnected
                        source: client.robots[index].reached ? "../assets/redHexagon.svg" : "../assets/greenHexagon.svg"
                        rotation: client.robots[index].theta
                        x: client.robots[index].x*parent.scaleCoeff - width/2
                        y: client.robots[index].y*parent.scaleCoeff - height/2
                        width: parent.physicalRobotWidth*parent.scaleCoeff
                        fillMode: Image.PreserveAspectFit
                    }
                }

                Repeater{
                    model: currentNumRobots

                    Image{
                        visible: client.robots[index].connectionStatus === CelluloBluetoothEnums.ConnectionStatusConnected
                        source: "../assets/yellowHexagon.svg"
                        rotation: client.robots[index].goalXYTheta.z
                        x: client.robots[index].goalXYTheta.x*parent.scaleCoeff - width/2
                        y: client.robots[index].goalXYTheta.y*parent.scaleCoeff - height/2
                        width: parent.physicalRobotWidth*parent.scaleCoeff
                        fillMode: Image.PreserveAspectFit
                    }
                }
            }
        }
    }
}
