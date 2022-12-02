import QtQuick 2.2
import QtQuick.Window 2.1
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.3
import QtBluetooth 5.2
import Cellulo 1.0
import QMLCache 1.0

ApplicationWindow {
    id: window
    visible: true
    minimumHeight: height
    minimumWidth: width

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

    Rectangle{
        id: page
        anchors.top: addressBox.bottom
        anchors.left: parent.left
        anchors.margins: robotHalf

        property real scaleCoeff: Math.min((Screen.width*0.8)/210, (Screen.height*0.8 - addressBox.height)/297)
        property real robotHalf: 75*scaleCoeff/2

        width: 210*scaleCoeff
        height: 297*scaleCoeff
        color: "#EEEEEE"
        border.color: "black"
        border.width: 2
        radius: 5

        Image{
            source: robotComm.kidnapped ? "../assets/redHexagon.svg" : "../assets/greenHexagon.svg"
            rotation: robotComm.theta
            x: robotComm.x*parent.scaleCoeff - width/2
            y: robotComm.y*parent.scaleCoeff - height/2
            sourceSize.width: 2*parent.robotHalf
        }
    }

    Item{ //Dummy item to provide margin for bottom
        anchors.top: page.bottom
        anchors.margins: page.robotHalf
    }

    Item{ //Dummy item to provide margin for right
        anchors.left: page.right
        anchors.margins: page.robotHalf
    }

    CelluloRobot{
        id: robotComm
        onConnectionStatusChanged:{
            if(connectionStatus === CelluloBluetoothEnums.ConnectionStatusConnected)
                setPoseBcastPeriod(0);
        }
    }
}
