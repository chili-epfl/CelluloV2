import QtQuick 2.2
import QtQuick.Window 2.1
import QtQuick.Dialogs 1.3
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.4
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
        title: "Robot Address"

        Column{
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
        id: controlsBox
        title: "Controls"
        anchors.top: addressBox.bottom

        Column{
            Row{
                spacing: 5
                CheckBox{
                    id: autoBuildCheckbox
                    text: "Autobuild"
                    checked: true
                    anchors.verticalCenter: parent.verticalCenter
                }
                Button{
                    text: "Clear Map"
                    anchors.verticalCenter: parent.verticalCenter
                    onClicked: map.clearTiles()
                }
                Button{
                    text: "Save Map"
                    anchors.verticalCenter: parent.verticalCenter
                    onClicked: dumpDialog.open()
                }
                Button{
                    text: "Load Map"
                    anchors.verticalCenter: parent.verticalCenter
                    onClicked: loadDialog.open()
                }
            }

            CheckBox{
                id: walkCheckbox
                text: "Walk?"
                checked: false
            }
        }
    }

    Rectangle{
        id: page
        anchors.top: controlsBox.bottom
        anchors.left: parent.left
        anchors.margins: 50

        property real rectSize: Math.min((Screen.width*0.7), (Screen.height*0.7 - addressBox.height))

        width: rectSize
        height: rectSize
        color: "#EEEEEE"
        border.color: "black"
        border.width: 2
        radius: 5

        HexTileMap{
            id: map

            property real scale: Math.min(parent.width/physicalSize.x, parent.height/physicalSize.y)

            width:  scale*physicalSize.x
            height: scale*physicalSize.y

            tileComponent: Qt.createComponent("HexTileWithName.qml")

            autoBuild: autoBuildCheckbox.checked

            onTileAdded: console.info("Tile added at " + newTile.coords.q + " " + newTile.coords.r)
            onTileRemoved: console.info("Tile removed from " + oldTileQ + " " + oldTileR)

            onTileClicked: {
                editTileMenu.tile = tile;
                editTileMenu.popup();
            }

            Image{
                property vector2d screenSize: parent.toScreenSize.convert(Qt.vector2d(75, 75*2/Math.sqrt(3)))
                property vector2d screenCoords: parent.toScreenCoords.convert(Qt.vector2d(robotComm.x, robotComm.y))

                source: robotComm.kidnapped ? "../assets/redHexagon.svg" : "../assets/greenHexagon.svg"
                rotation: robotComm.theta
                width: screenSize.x
                height: screenSize.y
                sourceSize.width: screenSize.x
                sourceSize.height: screenSize.y
                x: screenCoords.x - width/2
                y: screenCoords.y - height/2
                visible: true
            }
        }
    }

    Item{ //Dummy item to provide margin for bottom
        anchors.top: page.bottom
        anchors.margins: 50
    }

    Item{ //Dummy item to provide margin for right
        anchors.left: page.right
        anchors.margins: 50
    }

    RandomWalkerRobot{
        id: robotComm
        hexMap: map
        walk: walkCheckbox.checked
    }

    Menu{
        id: editTileMenu

        property HexTile tile: null

        onAboutToShow: tile.borderColor = "gray"
        onAboutToHide: tile.borderColor = "#323232"

        MenuItem{ text: editTileMenu.tile ? "Edit tile at " + editTileMenu.tile.coords.q + "," + editTileMenu.tile.coords.r : "Invalid tile"; enabled: false }
        MenuSeparator{}
        MenuItem{
            text: "Delete"
            onTriggered: map.removeTile(editTileMenu.tile)
        }
        MenuItem{
            text: "Rename"
            onTriggered: nameDialog.open()
        }
    }

    FileDialog{
        id: dumpDialog
        title: "Choose a new file to save map to"
        folder: shortcuts.desktop
        selectExisting: false
        selectMultiple: false
        nameFilters: ["*.json"]
        onAccepted: {
            var filename = fileUrl.toString();
            if(!filename.endsWith(".json")) //defaultSuffix doesn't work
                filename += ".json";
            map.dumpTilesToJSON(filename);
        }
    }

    FileDialog{
        id: loadDialog
        title: "Choose a map description to load"
        folder: shortcuts.desktop
        selectExisting: true
        selectMultiple: false
        nameFilters: ["*.json"]
        onAccepted: map.loadTilesFromJSON(fileUrl)
    }

    Dialog{
        id: nameDialog
        title: "Choose a name"
        standardButtons: StandardButton.Ok | StandardButton.Cancel
        onAccepted: editTileMenu.tile.name = nameField.text

        TextField{ id: nameField }
    }
}
