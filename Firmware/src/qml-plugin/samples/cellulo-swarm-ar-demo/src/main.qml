import QtQuick 2.9
import QtQuick.Controls 1.4
import QtQuick.Window 2.2
import Cellulo 1.0
import ch.epfl.chili.qmlar 1.0

Window{
    id: window
    visible: true

    minimumWidth: arComponent.width
    minimumHeight: arComponent.height

    property var client: poolButton.checked ? poolClient : hubClient

    CelluloRobotPoolClient{
        id: poolClient

        robotComponent: Qt.createComponent("Robot.qml")

        autoConnect: poolButton.checked
        onAutoConnectChanged: {
            if(!autoConnect)
                disconnectFromServer();
        }
    }

    CelluloRobotHubClient{
        id: hubClient

        robotComponent: Qt.createComponent("Robot.qml")

        serverAddress: hubAddress.text
        port: parseInt(hubPort.text)

        autoConnect: hubButton.checked
        onAutoConnectChanged: {
            if(!autoConnect)
                disconnectFromServer();
        }
    }

    GroupBox {
        id: serverBox
        title: "Controls"

        Row{
            spacing: 5

            Column{
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
        }
    }

    Item{
        anchors.top: serverBox.bottom
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right

        ARComponent{
            id: arComponent
            markersFilename: ":assets/cellulo-empty-ar-sheet-coarse-markers.json"
            arSceneComponent: Qt.createComponent("3DScene.qml")
            arSceneParameters: { "client": Qt.binding(function(){ return client; }) }
            anchors.fill: parent
            show_fps: true
            show_blobs: false
            show_markers: false
        }
    }

    Text{
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        textFormat: Text.RichText
        text: "<a href=\"https://chili-epfl.github.io/privacy-policies/cellulo-swarm-ar-demo-privacy-policy.html\">Privacy policy</a>"
        onLinkActivated: Qt.openUrlExternally(link)
    }
}
