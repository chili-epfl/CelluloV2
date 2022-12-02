import QtQuick 2.0

import Cellulo 1.0

HexTile{
    id: tile

    property string name: "Unnamed"

    Text{
        text: tile.name
        anchors.verticalCenter: tile.verticalCenter
        anchors.horizontalCenter: tile.horizontalCenter
        font.pixelSize: tile.height/6
    }
}
