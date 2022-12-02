import QtQuick 2.2
import QtQuick.Window 2.1
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.4
import QtQuick.Controls.Private 1.0
import QtQuick.Controls.Styles 1.3

import Cellulo 1.0

ApplicationWindow {
    id: window
    visible: true

    minimumHeight: height
    minimumWidth: width

    CelluloLocalRelayClient{
        id: client
        autoConnect: true
    }

    Component.onCompleted: {
        CelluloCommUtil.startRobotPoolDaemon();
        client.connectToServer();
    }

    Column{
        spacing: 5
        anchors.margins: 10
        anchors.verticalCenter: parent.verticalCenter

        Text{ text: "Cellulo robot pool service for Android" }

        Button{
            text: "Start service"
            onClicked: {
                CelluloCommUtil.startRobotPoolDaemon();
                client.connectToServer();
            }
        }

        Button{
            text: "Stop service"
            onClicked: CelluloCommUtil.stopRobotPoolDaemon()
        }

        Text{
            text: {
                var string;
                var needInfo = false;
                switch(client.connectionStatus){
                case CelluloCommUtil.RelayConnectionStatusDisconnected: return "Disconnected (must click \"Start service\" manually).";
                case CelluloCommUtil.RelayConnectionStatusConnecting: return "Connecting to server (must click \"Start service\" manually)...";
                case CelluloCommUtil.RelayConnectionStatusConnected: return "Connected to server. You must now kill this app and launch the pool GUI or the desired app.";
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
}
