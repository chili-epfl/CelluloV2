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

    CelluloLocalRelayServer{
        id: server

        onListenError: errorText.visible = true
        onListeningChanged:{
            if(listen)
                errorText.visible = false;
        }
        onClientConnected: clientConnectedText.visible = true
        onClientDisconnected: clientConnectedText.visible = false

        Component.onCompleted: listen = true
    }

    Column{
        spacing: 5

        Text{ text: server.listen ? "Listening..." : "Not listening, already connected or error." }

        Text{
            id: clientConnectedText
            text: "Client connected."
            visible: false
        }

        Text{
            id: errorText
            visible: false
            color: "red"
            text: "Could not start listening!"
        }
    }
}
