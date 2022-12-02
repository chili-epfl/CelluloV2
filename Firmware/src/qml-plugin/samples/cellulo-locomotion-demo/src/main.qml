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

    GroupBox {
        id: locomotionBox
        title: "Locomotion"

        anchors.top:  addressBox.bottom

        Column{
            spacing: 5

            Text{
                text: "X=" + robotComm.x.toFixed(2) + " Y=" + robotComm.y.toFixed(2) + " Theta=" + robotComm.theta.toFixed(1)
            }

            Row{
                Label{
                    text: "X goal (mm):"
                }
                TextField{
                    id: xGoalField
                    text: "105"
                }
            }

            Row{
                Label{
                    text: "Y goal (mm):"
                }
                TextField{
                    id: yGoalField
                    text: "148.5"
                }
            }

            Row{
                Label{
                    text: "Theta goal (deg):"
                }
                TextField{
                    id: thetaGoalField
                    text: "30"
                }
            }

            Row{
                Label{
                    text: "Linear max vel (mm/s):"
                }
                TextField{
                    id: linMaxVelField
                    text: "150"
                }
            }

            Row{
                Label{
                    text: "Angular max vel (rad/s):"
                }
                TextField{
                    id: angMaxVelField
                    text: "5"
                }
            }

            Button{
                text: "Go"
                onClicked: robotComm.setGoalPose(
                               parseFloat(xGoalField.text), parseFloat(yGoalField.text), parseFloat(thetaGoalField.text),
                               parseFloat(linMaxVelField.text), parseFloat(angMaxVelField.text));
            }
        }
    }

    CelluloRobot{
        id: robotComm
        onConnectionStatusChanged: {
            if(connectionStatus === CelluloBluetoothEnums.ConnectionStatusConnected)
                setVisualEffect(CelluloBluetoothEnums.VisualEffectConstAll, "#0F0F0F", 0);
        }
        onBootCompleted: setVisualEffect(CelluloBluetoothEnums.VisualEffectConstAll, "#0F0F0F", 0)
        onTrackingGoalReached: setVisualEffect(CelluloBluetoothEnums.VisualEffectConstAll, Qt.rgba(Math.random(), Math.random(), Math.random(), 1.0), 0)
    }
}
