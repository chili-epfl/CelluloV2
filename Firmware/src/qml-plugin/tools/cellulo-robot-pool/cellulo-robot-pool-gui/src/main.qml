import QtQuick 2.2
import QtQuick.Window 2.1
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.2
import QtQuick.Controls.Private 1.0
import QtQuick.Controls.Styles 1.3

import QMLBluetoothExtras 1.0
import QMLExtraDataStructures 1.0
import Cellulo 1.0

ApplicationWindow {
    id: window
    visible: true

    property bool mobile: android
    property bool android: Qt.platform.os === "android"
    property bool osx: Qt.platform.os === "osx"

    minimumWidth: width
    minimumHeight: height

    width: mobile ? Screen.width : 0.7*Screen.width
    height: mobile ? Screen.desktopAvailableHeight : 0.7*Screen.height

    Component.onCompleted: {
        if(Qt.platform.os === "linux"){
            if(CelluloCommUtil.testRobotPoolDaemon()){
                if(CelluloCommUtil.startRobotPoolDaemon())
                    toast.show("Robot pool daemon started, connecting...");
                else
                    toast.show("Robot pool daemon already running, connecting...");
                client.connectToServer();
            }
            else{
                var err = "/usr/local/bin/cellulorobotpoold not found!!!";
                toast.show(err);
                console.log(err);
            }
        }
    }

    CelluloRobotPoolClient{
        id: client

        onConnected: toast.show("Connected to Server.")
        onDisconnected: toast.show("Disconnected from Server.")

        function hasRobot(macAddr){
            for(var i=0;i<robots.length;i++)
                if(robots[i].macAddr.toUpperCase() === macAddr.toUpperCase())
                    return true;
            return false;
        }
    }


    ScrollView {
        anchors.fill: parent
        horizontalScrollBarPolicy: Qt.ScrollBarAlwaysOff
        verticalScrollBarPolicy: mobile ? Qt.ScrollBarAsNeeded : Qt.ScrollBarAlwaysOff

        Row{
            spacing: 5

            Column{
                spacing: 5

                GroupBox{
                    title: "Server controls" + (android ? " (use Cellulo Robot Pool Service app to start and stop)" : "")

                    Row{
                        spacing: 5

                        Button{
                            text: "Start Server"
                            anchors.verticalCenter: parent.verticalCenter
                            onClicked: {
                                if(CelluloCommUtil.startRobotPoolDaemon())
                                    toast.show("Started robot pool daemon.");
                                else
                                    toast.show("Cannot start robot pool daemon, possibly already running.");
                            }
                            enabled: (osx || android) ? false : true
                        }
                        Button{
                            text: "Stop Server"
                            anchors.verticalCenter: parent.verticalCenter
                            onClicked: {
                                if(CelluloCommUtil.stopRobotPoolDaemon())
                                    toast.show("Stopped robot pool daemon.");
                                else
                                    toast.show("Cannot stop robot pool daemon, possibly not running.");
                            }
                            enabled: (osx || android) ? false : true
                        }
                        Button{
                            text: "Clean Socket"
                            anchors.verticalCenter: parent.verticalCenter
                            onClicked: toast.show(client.cleanSocket() ? "Cleaned socket successfully." : "Could not clean socket.")
                            enabled: android ? false : true
                        }
                        Text{
                            text: {
                                var string;
                                var needInfo = false;
                                switch(client.connectionStatus){
                                case CelluloCommUtil.RelayConnectionStatusDisconnected:
                                    string = "Disconnected";
                                    needInfo = true;
                                    break;
                                case CelluloCommUtil.RelayConnectionStatusConnecting:
                                    string = "Connecting to server...";
                                    needInfo = true;
                                    break;
                                case CelluloCommUtil.RelayConnectionStatusConnected:
                                    string = "Connected to server";
                                    break;
                                }

                                if(needInfo){
                                    if(osx)
                                        string += "\n(must launch \"Cellulo Robot Pool Daemon\" manually on macOS)";
                                    else if(android)
                                        string += "\n(must launch \"Cellulo Robot Pool Service\" manually on Android)";
                                }

                                return string;
                            }
                            color: {
                                switch(client.connectionStatus){
                                case CelluloCommUtil.RelayConnectionStatusDisconnected: return "red";
                                case CelluloCommUtil.RelayConnectionStatusConnecting: return "yellow";
                                case CelluloCommUtil.RelayConnectionStatusConnected: return "green";
                                }
                            }
                            anchors.verticalCenter: parent.verticalCenter
                        }
                    }
                }

                GroupBox{
                    title: "Manually add robot"

                    Row{
                        spacing: 5

                        Text{
                            id: prefix
                            text: "00:06:66:74:"
                            anchors.verticalCenter: parent.verticalCenter
                        }

                        TextField{
                            id: suffix
                            placeholderText: "XX:XX"
                            anchors.verticalCenter: parent.verticalCenter

                            onAccepted: addButton.clicked()
                        }

                        Button{
                            id: addButton

                            text: "+"
                            anchors.verticalCenter: parent.verticalCenter
                            onClicked: client.createAddSelectRobot(prefix.text + suffix.text)
                        }
                    }
                }

                GroupBox{
                    title: "Robots on server"

                    enabled: client.connected

                    Column{
                        id: robotList

                        RepeaterList{
                            id: robotListMacAddrSelectors

                            model: client.robots.length

                            Row{
                                spacing: 5

                                property var selectorAtRow: selector

                                MacAddrSelector{
                                    id: selector

                                    property var robot: client.robots[index]
                                    property string localAdapterMacAddr: robot.localAdapterMacAddr

                                    enableLocalAdapters: !android && !osx

                                    addresses: [robot.macAddr.toUpperCase()]
                                    localAdapterAddresses: client.localAdapters
                                    connectionStatus: robot.connectionStatus

                                    onConnectionStatusChanged: {
                                        if(connectionStatus === CelluloBluetoothEnums.ConnectionStatusConnected)
                                            robot.setVisualEffect(CelluloBluetoothEnums.VisualEffectAlertAll,"#800080",3);
                                    }

                                    onLocalAdapterMacAddrChanged: selectLocalAdapterAddress(localAdapterMacAddr.toUpperCase())
                                    onSelectedAddrRefreshRequested:{
                                        selectLocalAdapterAddress(localAdapterMacAddr.toUpperCase());
                                        selectAddress(robot.macAddr.toUpperCase());
                                    }

                                    onConnectRequested: {
                                        robot.localAdapterMacAddr = selectedLocalAdapterAddress;
                                        robot.macAddr = selectedAddress;
                                        robot.connectToServer();
                                    }

                                    onDisconnectRequested: robot.disconnectFromServer()
                                }
                            }
                        }

                        Row{
                            spacing: 5

                            Button{
                                text: "Equally distribute local adapters"
                                onClicked:{
                                    for(var i=0;i<client.robots.length;i++)
                                        client.robots[i].localAdapterMacAddr = client.localAdapters[i % client.localAdapters.length];
                                }
                                enabled: (osx || android) ? false : true
                            }
                        }
                    }
                }

                GroupBox{
                    title: "Robot mass control"

                    Row{
                        spacing: 5

                        Button{
                            text: "Reset all"
                            anchors.verticalCenter: parent.verticalCenter

                            onClicked:{
                                for(var i=0;i<client.robots.length;i++)
                                    client.robots[i].reset();
                            }
                        }
                        Button{
                            text: "Shutdown all"
                            anchors.verticalCenter: parent.verticalCenter

                            onClicked:{
                                for(var i=0;i<client.robots.length;i++)
                                    client.robots[i].shutdown();
                            }
                        }
                        Button{
                            text: "Connect to all"
                            anchors.verticalCenter: parent.verticalCenter

                            onClicked:{
                                for(var i=0;i<client.robots.length;i++){
                                    client.robots[i].localAdapterMacAddr = robotListMacAddrSelectors.items[i].selectorAtRow.selectedLocalAdapterAddress;
                                    client.robots[i].macAddr = robotListMacAddrSelectors.items[i].selectorAtRow.selectedAddress;
                                    client.robots[i].connectToServer();
                                }
                            }
                        }
                        Button{
                            text: "Disconnect from all"
                            anchors.verticalCenter: parent.verticalCenter

                            onClicked:{
                                for(var i=0;i<client.robots.length;i++)
                                    client.robots[i].disconnectFromServer();
                            }
                        }
                    }
                }
            }

            GroupBox{
                title: "Scan for robots"

                CelluloBluetoothScanner{
                    id: scanner
                    continuous: continuousCheckBox.checked
                }

                Column{
                    id: scanList

                    CheckBox{
                        id: continuousCheckBox

                        text: "Scan continuously"
                    }

                    Row{
                        spacing: 5

                        Button{
                            text: scanner.scanning ? "Scanning..." : "Scan"
                            enabled: !scanner.scanning
                            onClicked: scanner.start()
                        }

                        Button{
                            text: "Stop"
                            enabled: scanner.scanning
                            onClicked: scanner.stop()
                        }
                    }

                    Repeater{
                        model: scanner.foundRobots.length

                        Row{
                            spacing: 5

                            property string currentMacAddr: scanner.foundRobots[index]

                            Text{
                                text: currentMacAddr
                                anchors.verticalCenter: parent.verticalCenter
                                color: client.hasRobot(currentMacAddr) ? "gray" : "black"
                            }

                            Button{
                                text: "+"
                                anchors.verticalCenter: parent.verticalCenter

                                enabled: !client.hasRobot(currentMacAddr)

                                onClicked: client.createAddSelectRobot(currentMacAddr)
                            }
                        }
                    }

                    Button{
                        text: "Add all above"
                        enabled: scanner.foundRobots.length > 0
                        onClicked: {
                            for(var i=0;i<scanner.foundRobots.length;i++)
                                if(!client.hasRobot(scanner.foundRobots[i]))
                                    client.createAddSelectRobot(scanner.foundRobots[i]);
                        }
                    }
                }
            }
        }
    }

    ToastManager{ id: toast }
}
