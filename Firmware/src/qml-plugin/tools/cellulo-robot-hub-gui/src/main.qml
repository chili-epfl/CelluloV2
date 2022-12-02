import QtQuick 2.2
import QtQuick.Window 2.1
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.2
import QtQuick.Controls.Private 1.0
import QtQuick.Controls.Styles 1.3

import QMLCache 1.0
import QMLBluetoothExtras 1.0
import QMLExtraDataStructures 1.0
import Cellulo 1.0

ApplicationWindow {
    id: window
    visible: true

    property bool mobile: android
    property bool android: Qt.platform.os === "android"
    property bool osx: Qt.platform.os === "osx"
    property bool winrt: Qt.platform.os === "winrt"

    function em(x){ return Math.round(x*TextSingleton.font.pixelSize); }

    minimumWidth: width
    minimumHeight: height

    width: mobile ? Screen.width : 0.7*Screen.width
    height: mobile ? Screen.desktopAvailableHeight : 0.7*Screen.height

    CelluloRobotHubClient{
        id: client

        serverAddress: addressBox.currentText
        port: parseInt(portBox.currentText)

        autoConnect: autoConnectCheckBox.checked

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
                    title: "Server status & control"

                    Column{
                        spacing: 5

                        Text{
                            text: {
                                switch(client.connectionStatus){
                                case CelluloCommUtil.RelayConnectionStatusDisconnected: return "Disconnected";
                                case CelluloCommUtil.RelayConnectionStatusConnecting: return "Connecting to server...";
                                case CelluloCommUtil.RelayConnectionStatusConnected: return "Connected to server";
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

                        Text{
                            text: "Local adapters on server: " + client.localAdapters
                        }

                        Row{
                            spacing: 5

                            Text{
                                text: "Address:"
                                anchors.verticalCenter: parent.verticalCenter
                            }

                            ComboBox{
                                id: addressBox

                                anchors.verticalCenter: parent.verticalCenter

                                editable: true
                                width: em(12)

                                onAccepted: {
                                    var tempModel = [];
                                    var exists = false;
                                    var newElement = editText;
                                    for(var i=0;i<model.length;i++){
                                        tempModel.push(model[i]);
                                        if(model[i] == newElement)
                                            exists = true;
                                    }
                                    if(!exists)
                                        tempModel.push(newElement);
                                    model = tempModel;
                                    selectAddress(newElement);
                                }

                                onModelChanged: {
                                    if(Array.isArray(model))
                                        QMLCache.write("serverAddresses", model.join(","))
                                }

                                Component.onCompleted: {
                                    var cachedAddrs = QMLCache.read("serverAddresses").split(",");
                                    if(cachedAddrs.length == 1 && cachedAddrs[0] == "") //Weird behavior on split...
                                        cachedAddrs = [];
                                    if(!Array.isArray(cachedAddrs) || !cachedAddrs.length)
                                        model = ["192.168.2.1", "192.168.4.1"];
                                    else
                                        model = cachedAddrs;
                                }

                                function selectAddress(address){
                                    for(var i=0;i<model.length;i++)
                                        if(model[i] === address){
                                            currentIndex = i;
                                            return;
                                        }
                                    currentIndex = -1;
                                }
                            }

                            Text{
                                text: "Port:"
                                anchors.verticalCenter: parent.verticalCenter
                            }

                            ComboBox{
                                id: portBox

                                anchors.verticalCenter: parent.verticalCenter

                                editable: true
                                width: em(12)

                                inputMethodHints: Qt.ImhDigitsOnly

                                onAccepted: {
                                    var tempModel = [];
                                    var exists = false;
                                    var newElement = editText;
                                    for(var i=0;i<model.length;i++){
                                        tempModel.push(model[i]);
                                        if(model[i] == newElement)
                                            exists = true;
                                    }
                                    if(!exists)
                                        tempModel.push(newElement);
                                    model = tempModel;
                                    selectPort(newElement);
                                }

                                onModelChanged: {
                                    if(Array.isArray(model))
                                        QMLCache.write("serverPorts", model.join(","))
                                }

                                Component.onCompleted: {
                                    var cachedPorts = QMLCache.read("serverPorts").split(",");
                                    if(cachedPorts.length == 1 && cachedPorts[0] == "") //Weird behavior on split...
                                        cachedPorts = [];
                                    if(!Array.isArray(cachedPorts) || !cachedPorts.length)
                                        model = ["2556"];
                                    else
                                        model = cachedPorts;
                                }

                                function selectPort(port){
                                    for(var i=0;i<model.length;i++)
                                        if(model[i] === port){
                                            currentIndex = i;
                                            return;
                                        }
                                    currentIndex = -1;
                                }
                            }

                            CheckBox{
                                id: autoConnectCheckBox
                                text: "Autoconnect?"
                                checked: true
                            }

                            Button{
                                enabled: !autoConnectCheckBox.checked
                                text: "Connect"
                                anchors.verticalCenter: parent.verticalCenter
                                onClicked: client.connectToServer()
                            }

                            Button{
                                enabled: !autoConnectCheckBox.checked
                                text: "Disconnect"
                                anchors.verticalCenter: parent.verticalCenter
                                onClicked: client.disconnectFromServer()
                            }
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
                                    property string localAdapterMacAddr: robot.localAdapterMacAddr.toUpperCase()

                                    addresses: [robot.macAddr.toUpperCase()]
                                    localAdapterAddresses: client.localAdapters
                                    connectionStatus: robot.connectionStatus

                                    onConnectionStatusChanged: {
                                        if(connectionStatus === CelluloBluetoothEnums.ConnectionStatusConnected)
                                            robot.setVisualEffect(CelluloBluetoothEnums.VisualEffectAlertAll,"#800080",3);
                                    }

                                    onLocalAdapterMacAddrChanged: selectLocalAdapterAddress(localAdapterMacAddr.toUpperCase())
                                    onSelectedAddrRefreshRequested: {
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

                    Text{
                        visible: winrt
                        text: "Robots must first be paired from\nOS Bluetooth settings to appear in scan!"
                    }

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
