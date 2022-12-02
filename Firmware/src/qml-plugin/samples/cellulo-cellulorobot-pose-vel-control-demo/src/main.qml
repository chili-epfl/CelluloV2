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
    minimumHeight: height
    minimumWidth: width

    Column{
        spacing: 5

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
                            robot.localAdapterMacAddr = selectedLocalAdapterAddress;
                            robot.macAddr = selectedAddress;
                        }
                        onDisconnectRequested: robot.disconnectFromServer()
                        connectionStatus: robot.connectionStatus
                    }

                    Button{
                        text: "Reset"
                        onClicked: robot.reset()
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

        Text{ text: "x error = " + (robot.x - goalPose.x).toFixed(2) }
        Text{ text: "y error = " + (robot.y - goalPose.y).toFixed(2) }
        Text{
            text: {
                var err = robot.theta - goalPose.z;
                while(err < -180)
                    err += 360;
                while(err >= 180)
                    err -= 360;
                return "theta error = " + err.toFixed(2);
            }
        }
        Text{ text: "Vx error = " + (robot.vxyw.x - goalVel.x).toFixed(2) }
        Text{ text: "Vy error = " + (robot.vxyw.y - goalVel.y).toFixed(2) }
        Text{ text: "w error = " + (robot.vxyw.z - goalVel.z).toFixed(2) }

        CheckBox{
            id: go
            text: "Go"
            checked: false
        }

        Item{
            width: page.width + page.physicalRobotWidth*page.scaleCoeff
            height: page.height + page.physicalRobotWidth*page.scaleCoeff

            Rectangle{
                id: page

                anchors.centerIn: parent

                property real physicalWidth: 400        ///< Physical paper width in mm
                property real physicalHeight: 400       ///< Physical paper height in mm
                property real physicalRobotWidth: 75    ///< Physical robot width in mm

                width: height*physicalWidth/physicalHeight
                height: Screen.height/3

                property real scaleCoeff: width/physicalWidth

                color: "#EEEEEE"
                border.color: "black"
                border.width: 2
                radius: 5

                Image{
                    visible: robot.connectionStatus === CelluloBluetoothEnums.ConnectionStatusConnected
                    source: "../assets/greenHexagon.svg"
                    rotation: robot.theta
                    x: robot.x*parent.scaleCoeff - width/2
                    y: robot.y*parent.scaleCoeff - height/2
                    width: parent.physicalRobotWidth*parent.scaleCoeff
                    fillMode: Image.PreserveAspectFit
                }

                Image{
                    source: "../assets/yellowHexagon.svg"
                    rotation: root.goalPose.z
                    x: root.goalPose.x*parent.scaleCoeff - width/2
                    y: root.goalPose.y*parent.scaleCoeff - height/2
                    width: parent.physicalRobotWidth*parent.scaleCoeff
                    fillMode: Image.PreserveAspectFit
                }

                Image{
                    source: "../assets/arrowRed.svg"
                    rotation: Math.atan2(root.goalVel.y, root.goalVel.x)/Math.PI*180
                    x: (root.goalPose.x)*parent.scaleCoeff - width/2
                    y: (root.goalPose.y)*parent.scaleCoeff - height/2
                    width: parent.linearVel*parent.scaleCoeff
                    fillMode: Image.PreserveAspectFit
                }
            }
        }
    }

    property vector3d goalPose: Qt.vector3d(0,0,0)
    property vector3d goalVel: Qt.vector3d(0,0,0)

    CelluloRobot{
        id: robot

        poseVelControlEnabled: go.checked
        poseVelControlPeriod: 50

        onNextGoalPoseVelRequested: {
            if(poseVelControlEnabled){
                calcGoalPoseVel();
                setGoalPoseAndVelocity(root.goalPose.x, root.goalPose.y, root.goalPose.z, root.goalVel.x, root.goalVel.y, root.goalVel.z);
            }
            else
                setGoalVelocity(0,0,0);
        }
    }

    property real motionPeriod: 10000
    property vector2d motionCenter: Qt.vector2d(200, 200)
    property real motionR: 100
    property real linearVel: motionR*2*Math.PI/motionPeriod*1000
    property real angularVel: 2*Math.PI/(motionPeriod/2)*1000

    function calcGoalPoseVel(){
        var motionTime = Date.now() % motionPeriod;
        var motionT = 2*Math.PI*motionTime/motionPeriod;

        goalPose.x = motionCenter.x + motionR*Math.cos(motionT);
        goalPose.y = motionCenter.y + motionR*Math.sin(motionT);
        if(motionT < Math.PI)
            goalPose.z = motionT*2;
        else
            goalPose.z = 2*Math.PI - (motionT - Math.PI)*2;
        goalPose.z = goalPose.z/Math.PI*180;

        goalVel.x = linearVel*(-Math.sin(motionT));
        goalVel.y = linearVel*(Math.cos(motionT));
        if(motionT < Math.PI)
            goalVel.z = angularVel;
        else
            goalVel.z = -angularVel;
    }
}
