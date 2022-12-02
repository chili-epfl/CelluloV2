import QtQuick 2.2
import QtQuick.Window 2.1
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.3
import QtCharts 2.2
import Cellulo 1.0
import QMLExtraDataStructures 1.0
import QMLCache 1.0
import Logger 1.0


ApplicationWindow {
    id: window
    visible: true
    width: 1024
    height: 768

    CSVLogger{
        id: logger
        enabled: true
        logTime: true
        logMillis: true
        filename: "cellulo/cellulo-motion-test/"+ linearMinVel+"_"+linearMaxVel+"_"+angularMaxVel*10+"/"+ LoggerUtil.getUniqueDeviceID(
                      ) + "_" + Qt.formatDateTime(
                      new Date(), "ddd-dd-MM-yyyy_hh:mm:ss") + ".csv"
        header: ['x','y','theta','goalx','goaly','goalTheta','distanceThreshold','angleThreshold']
        toConsole: false
    }


    property real linearMinVel: 50 //mm/s
    property real linearMaxVel: 185 //mm/s
    property real angularMaxVel: 7.5 //rad/s
    property real distanceThreshold: 10
    property real angleThreshold: 3

    ToastManager{ id: toast }

    CelluloRobot{
        id: robot
        onConnectionStatusChanged:{
            if(connectionStatus === CelluloBluetoothEnums.ConnectionStatusConnected)
                init();
        }

        function init(){
            setPoseBcastPeriod(0);
            setTimestampingEnabled(true);
        }

        function start(){
            currentGoal = 0;
            charts.clear();
            goToCurrent();
            toast.show("Starting test.");
        }

        function goToCurrent(){
            if(Math.floor(currentGoal/3) < 12){
                if((currentGoal % 3) == 1)
                    toast.show("Testing " + robot.testAngles[Math.floor(currentGoal/3)] + "°");
                setGoalPose(goals[currentGoal].x, goals[currentGoal].y, goals[currentGoal].z, linearMaxVel, angularMaxVel);
                resultsScrollView.scrollTo(Math.floor(currentGoal/3));
            }
        }

        property vector2d startPos: Qt.vector2d(105, 52.5)
        property vector2d endPos: Qt.vector2d(105, 244.5)

        property var testAngles: [0, 30, 60, 90, 120, 150, 180, 210, 240, 270, 300, 330]

        property var goals: [
            startPos.toVector3d().plus(Qt.vector3d(0, 0, testAngles[0])),
            endPos.  toVector3d().plus(Qt.vector3d(0, 0, testAngles[0])),
            startPos.toVector3d().plus(Qt.vector3d(0, 0, testAngles[0])),

            startPos.toVector3d().plus(Qt.vector3d(0, 0, testAngles[1])),
            endPos.  toVector3d().plus(Qt.vector3d(0, 0, testAngles[1])),
            startPos.toVector3d().plus(Qt.vector3d(0, 0, testAngles[1])),

            startPos.toVector3d().plus(Qt.vector3d(0, 0, testAngles[2])),
            endPos.  toVector3d().plus(Qt.vector3d(0, 0, testAngles[2])),
            startPos.toVector3d().plus(Qt.vector3d(0, 0, testAngles[2])),

            startPos.toVector3d().plus(Qt.vector3d(0, 0, testAngles[3])),
            endPos.  toVector3d().plus(Qt.vector3d(0, 0, testAngles[3])),
            startPos.toVector3d().plus(Qt.vector3d(0, 0, testAngles[3])),

            startPos.toVector3d().plus(Qt.vector3d(0, 0, testAngles[4])),
            endPos.  toVector3d().plus(Qt.vector3d(0, 0, testAngles[4])),
            startPos.toVector3d().plus(Qt.vector3d(0, 0, testAngles[4])),

            startPos.toVector3d().plus(Qt.vector3d(0, 0, testAngles[5])),
            endPos.  toVector3d().plus(Qt.vector3d(0, 0, testAngles[5])),
            startPos.toVector3d().plus(Qt.vector3d(0, 0, testAngles[5])),

            startPos.toVector3d().plus(Qt.vector3d(0, 0, testAngles[6])),
            endPos.  toVector3d().plus(Qt.vector3d(0, 0, testAngles[6])),
            startPos.toVector3d().plus(Qt.vector3d(0, 0, testAngles[6])),

            startPos.toVector3d().plus(Qt.vector3d(0, 0, testAngles[7])),
            endPos.  toVector3d().plus(Qt.vector3d(0, 0, testAngles[7])),
            startPos.toVector3d().plus(Qt.vector3d(0, 0, testAngles[7])),

            startPos.toVector3d().plus(Qt.vector3d(0, 0, testAngles[8])),
            endPos.  toVector3d().plus(Qt.vector3d(0, 0, testAngles[8])),
            startPos.toVector3d().plus(Qt.vector3d(0, 0, testAngles[8])),

            startPos.toVector3d().plus(Qt.vector3d(0, 0, testAngles[9])),
            endPos.  toVector3d().plus(Qt.vector3d(0, 0, testAngles[9])),
            startPos.toVector3d().plus(Qt.vector3d(0, 0, testAngles[9])),

            startPos.toVector3d().plus(Qt.vector3d(0, 0, testAngles[10])),
            endPos.  toVector3d().plus(Qt.vector3d(0, 0, testAngles[10])),
            startPos.toVector3d().plus(Qt.vector3d(0, 0, testAngles[10])),

            startPos.toVector3d().plus(Qt.vector3d(0, 0, testAngles[11])),
            endPos.  toVector3d().plus(Qt.vector3d(0, 0, testAngles[11])),
            startPos.toVector3d().plus(Qt.vector3d(0, 0, testAngles[11]))
        ]
        property int currentGoal: 0

        onVxywChanged: {
            if((currentGoal % 3) == 1){
                var currentAngle = Math.floor(currentGoal/3);
                var linearVel = vxyw.toVector2d();
                var measuredMagnitude = linearVel.length();
                if(measuredMagnitude > linearMinVel){
                    charts.items[currentAngle].angleChart.add(lastTimestamp, theta);
                    charts.items[currentAngle].magnitudeChart.add(lastTimestamp, measuredMagnitude);
                }
            }
        }

        onPoseChanged: {
            logger.log([robot.x, robot.y, robot.theta, goals[currentGoal].x, goals[currentGoal].y, goals[currentGoal].z,distanceThreshold,angleThreshold])
            if(Math.floor(currentGoal/3) < 12){
                var dist = Qt.vector2d(x,y).minus(goals[currentGoal].toVector2d()).length();
                var rot = theta - goals[currentGoal].z;
                while(rot < -180)
                    rot += 360;
                while(rot > 180)
                    rot -= 360;
                if(dist < distanceThreshold && Math.abs(rot) < angleThreshold){
                    distanceThreshold = 10;
                    angleThreshold = 3;
                    currentGoal++;
                    goToCurrent();
                }
            }
            else{
                currentGoal = 0;
                reset();
                toast.show("Done, resetting robot.");
            }
        }
    }

    CelluloBluetoothScanner{
        id: scanner
        onRobotDiscovered: {
            var newAddresses = macAddrSelector.addresses;
            if(newAddresses.indexOf(macAddr) < 0){
                toast.show(macAddr + " discovered.");
                newAddresses.push(macAddr);
                newAddresses.sort();
            }
            macAddrSelector.addresses = newAddresses;
            QMLCache.write("addresses", macAddrSelector.addresses.join(','));
        }
    }

    GroupBox {
        id: addressBox
        title: "Robot"

        Row{
            spacing: 5

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
        }
    }

    GroupBox {
        id: controlsBox
        title: "Controls"

        anchors.top: addressBox.bottom

        Row{
            spacing: 5
            Button {
                text: "Start"
                onClicked: robot.start()
            }
            Button {
                text: "Reset"
                onClicked: robot.reset()
            }
            Button {
                text: "Shutdown"
                onClicked: robot.shutdown()
            }
            Button {
                text: "Error Threshold"
                onClicked: {
                    angleThreshold++
                    distanceThreshold++
                }
            }
        }
    }

    GroupBox {
        id: pageBox
        title: "Paper"

        anchors.top: controlsBox.bottom
        anchors.left: parent.left

        width: height/297*210
        height: window.height - addressBox.height - controlsBox.height

        Rectangle{
            id: page

            property real scaleCoeff: height/297

            anchors.top: parent.top
            anchors.bottom: parent.bottom

            width: height/297*210
            color: "#EEEEEE"
            border.color: "black"
            border.width: 2
            radius: 5

            Image{
                source: "../assets/paper.svg"
                width: 210*parent.scaleCoeff
                height: 297*parent.scaleCoeff
            }

            Image{
                source: robot.kidnapped ? "../assets/redHexagon.svg" : "../assets/greenHexagon.svg"
                rotation: robot.theta
                visible: robot.connectionStatus === CelluloBluetoothEnums.ConnectionStatusConnected
                x: robot.x*parent.scaleCoeff - width/2
                y: robot.y*parent.scaleCoeff - height/2
                sourceSize.width: 75*parent.scaleCoeff
                sourceSize.height: 85*parent.scaleCoeff
            }
        }
    }

    GroupBox {
        id: resultsBox
        title: "Results"

        anchors.top: pageBox.top
        anchors.left: pageBox.right

        height: pageBox.height
        width: window.width - pageBox.width

        ScrollView{
            id: resultsScrollView
            horizontalScrollBarPolicy: Qt.ScrollBarAlwaysOff
            anchors.fill: parent

            function scrollTo(index){
                var p = 0;
                for(var i=0;i<index && i<charts.items.length;i++)
                    p += charts.items[i].height;
                flickableItem.contentY = p;
            }

            Column{
                spacing: 5

                Row{
                    Text{
                        id: testAngleText
                        text: "Angle"
                    }
                    Text{
                        text: "Orientation"
                        width: resultsBox.width/2 - 30/2 - testAngleText.width/2
                        horizontalAlignment: Text.AlignHCenter
                    }
                    Text{
                        text: "Magnitude"
                        width: resultsBox.width/2 - 30/2 - testAngleText.width/2
                        horizontalAlignment: Text.AlignHCenter
                    }
                }

                RepeaterList{
                    id: charts

                    model: 12

                    function clear(){
                        for(var i=0;i<model;i++)
                            items[i].clear();
                    }

                    Row{
                        property var angleChart: angleChartObj
                        property var magnitudeChart: magnitudeChartObj

                        function clear(){
                            angleChartObj.clear();
                            magnitudeChartObj.clear();
                        }

                        Text{
                            text: robot.testAngles[index] + "°"
                            width: testAngleText.width
                            horizontalAlignment: Text.AlignRight
                            anchors.verticalCenter: parent.verticalCenter
                        }

                        ChartView{
                            id: angleChartObj

                            property var startT: 0

                            function clear(){
                                startT = 0;
                                removeAllSeries();
                                createSeries(ChartView.SeriesTypeLine, "", axisXAng, axisYAng);
                                createSeries(ChartView.SeriesTypeLine, "", axisXAng, axisYAng);
                            }

                            function add(t, angle){
                                while(angle < robot.testAngles[index] - 180)
                                    angle += 360;
                                while(angle > robot.testAngles[index] + 180)
                                    angle -= 360;

                                var ideal = series(0);
                                if(startT == 0){
                                    startT = t;
                                    axisXAng.min = startT;
                                    axisXAng.max = startT + 1;

                                    ideal.append(startT, robot.testAngles[index]);
                                    ideal.append(startT + 1, robot.testAngles[index]);
                                }
                                else{
                                    axisXAng.max = t;
                                    ideal.remove(1);
                                    ideal.append(t, robot.testAngles[index]);
                                }

                                var measured = series(1);
                                measured.append(t, angle);
                            }

                            backgroundRoundness: 0
                            legend.visible: false
                            backgroundColor: "transparent"
                            margins.left: 0; margins.right: 0; margins.top: 0; margins.bottom: 0
                            width: resultsBox.width/2 - 30/2 - testAngleText.width/2
                            height: width/2
                            antialiasing: true

                            ValueAxis {
                                id: axisYAng
                                min: robot.testAngles[index] - 45
                                max: robot.testAngles[index] + 45
                            }

                            ValueAxis {
                                id: axisXAng
                                min: 0
                                max: 1
                            }
                        }

                        ChartView{
                            id: magnitudeChartObj

                            property var startT: 0

                            function clear(){
                                startT = 0;
                                removeAllSeries();
                                createSeries(ChartView.SeriesTypeLine, "", axisXMag, axisYMag);
                                createSeries(ChartView.SeriesTypeLine, "", axisXMag, axisYMag);
                            }

                            function add(t, magnitude){
                                var ideal = series(0);
                                if(startT == 0){
                                    startT = t;
                                    axisXMag.min = startT;
                                    axisXMag.max = startT + 1;

                                    ideal.append(startT, linearMaxVel);
                                    ideal.append(startT + 1, linearMaxVel);
                                }
                                else{
                                    axisXMag.max = t;
                                    ideal.remove(1);
                                    ideal.append(t, linearMaxVel);
                                }

                                var measured = series(1);
                                measured.append(t, magnitude);
                            }

                            backgroundRoundness: 0
                            legend.visible: false
                            backgroundColor: "transparent"
                            margins.left: 0; margins.right: 0; margins.top: 0; margins.bottom: 0
                            width: resultsBox.width/2 - 30/2 - testAngleText.width/2
                            height: width/2
                            antialiasing: true

                            ValueAxis {
                                id: axisYMag
                                min: linearMaxVel - 100
                                max: linearMaxVel + 100
                            }

                            ValueAxis {
                                id: axisXMag
                                min: 0
                                max: 1
                            }
                        }
                    }
                }
            }
        }
    }
}
