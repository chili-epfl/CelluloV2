import QtQuick 2.2
import QtQuick.Window 2.1
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.2
import QtQuick.Controls.Private 1.0
import QtQuick.Controls.Styles 1.3
import Cellulo 1.0
import QMLCache 1.0

ApplicationWindow {
    id: root
    visible: true

    minimumHeight: 100
    minimumWidth: 200

    property real mapHalfWidth: 210/2
    property real mapHalfHeight: 297/2
    property real mapHalfBorder: 60/2

    property variant currentHaptics: {
        "xyAssist": 0.0,
        "thetaAssist": 0.0,
        "vibrateIntensity": 0.0,
        "logText": ""
    }

    property variant zoneHaptics: {
        "ICE": {
            "xyAssist": 0.9,
            "thetaAssist": 0.9,
            "vibrateIntensity": 0.0
        },
        "MUD": {
            "xyAssist": 0.3,
            "thetaAssist": 0.3,
            "vibrateIntensity": 0.0
        },
        "SANDPAPER": {
            "xyAssist": -0.3,
            "thetaAssist": -0.3,
            "vibrateIntensity": 0.0
        },
        "STONES": {
            "xyAssist": 0.6,
            "thetaAssist": 0.6,
            "vibrateIntensity": 1.0
        }
    }

    property variant zoneDistances: {
        "ICE": -1,
        "MUD": -1,
        "SANDPAPER": -1,
        "STONES": -1
    }

    CelluloRobot{
        id: robotComm

        onConnectionStatusChanged: {
            if(connectionStatus == CelluloBluetoothEnums.ConnectionStatusConnected)
                root.setupHaptics();
        }

        onBootCompleted: root.setupHaptics()

        Component.onCompleted: zoneEngine.addNewClient(robotComm)

        onZoneValueChanged:{
            var temp = zoneDistances;
            temp[zone.name] = value;
            zoneDistances = temp;
        }
    }

    function setupHaptics(){
        robotComm.setGestureEnabled(false);
        robotComm.setCasualBackdriveAssistEnabled(false);
        robotComm.setExposureTime(900);
        robotComm.setPoseBcastPeriod(0);
    }

    function updateHaptics(){
        var xyAssistTotal = 0;
        var thetaAssistTotal = 0;
        var vibrateIntensityTotal = 0;
        var weightSum = 0;
        var logTextTotal = "";
        var temp;
        for(var zoneName in zoneDistances){
            var dist = zoneDistances[zoneName];

            //Completely inside one zone
            if(dist === 0.0){
                xyAssistTotal = zoneHaptics[zoneName].xyAssist;
                thetaAssistTotal = zoneHaptics[zoneName].thetaAssist;
                vibrateIntensityTotal = zoneHaptics[zoneName].vibrateIntensity;
                logTextTotal = zoneName + " only";
                weightSum = 0;
                break;
            }

            //Between zones
            else if(0 < dist && dist < root.mapHalfBorder*2){

                //Add this zone to haptics
                var weight = 1.0/dist;
                xyAssistTotal += weight*zoneHaptics[zoneName].xyAssist;
                thetaAssistTotal += weight*zoneHaptics[zoneName].thetaAssist;
                vibrateIntensityTotal += weight*zoneHaptics[zoneName].vibrateIntensity;
                weightSum += weight;

                //Add this zone to haptics text
                if(logTextTotal)
                    logTextTotal += " + " + zoneName;
                else
                    logTextTotal = zoneName;
            }
        }

        if(weightSum > 0){
            xyAssistTotal /= weightSum;
            thetaAssistTotal /= weightSum;
            vibrateIntensityTotal /= weightSum;
        }

        temp = currentHaptics;
        temp.xyAssist = xyAssistTotal;
        temp.thetaAssist = thetaAssistTotal;
        temp.vibrateIntensity = vibrateIntensityTotal;
        temp.logText = logTextTotal;
        currentHaptics = temp;
    }

    Timer{
        running: robotComm.connectionStatus === CelluloBluetoothEnums.ConnectionStatusConnected
        repeat: true
        interval: 100
        onTriggered: {
            updateHaptics();
            log.append(currentHaptics.logText);
            robotComm.setHapticBackdriveAssist(currentHaptics.xyAssist, currentHaptics.xyAssist, currentHaptics.thetaAssist);
            robotComm.vibrateOnMotion(currentHaptics.vibrateIntensity, 40);

            console.info(currentHaptics.xyAssist + " " + currentHaptics.thetaAssist + " " + currentHaptics.vibrateIntensity);
        }
    }

    CelluloZoneEngine{
        id: zoneEngine

        CelluloZoneRectangleDistance{
            name: "ICE"
            x: 0
            y: 0
            width: root.mapHalfWidth - root.mapHalfBorder
            height: root.mapHalfHeight - root.mapHalfBorder
        }
        CelluloZoneRectangleDistance{
            name: "MUD"
            x: root.mapHalfWidth + root.mapHalfBorder
            y: 0
            width: root.mapHalfWidth - root.mapHalfBorder
            height: root.mapHalfHeight - root.mapHalfBorder
        }
        CelluloZoneRectangleDistance{
            name: "SANDPAPER"
            x: 0
            y: root.mapHalfHeight + root.mapHalfBorder
            width: root.mapHalfWidth - root.mapHalfBorder
            height: root.mapHalfHeight - root.mapHalfBorder
        }
        CelluloZoneRectangleDistance{
            name: "STONES"
            x: root.mapHalfWidth + root.mapHalfBorder
            y: root.mapHalfHeight + root.mapHalfBorder
            width: root.mapHalfWidth - root.mapHalfBorder
            height: root.mapHalfHeight - root.mapHalfBorder
        }
    }

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

            TextArea{
                id: log
                readOnly: true
            }
        }
    }
}
