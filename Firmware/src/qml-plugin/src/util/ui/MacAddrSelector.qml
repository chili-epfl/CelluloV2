import QtQuick 2.2
import QtQuick.Controls 1.2
import QtQuick.Controls.Private 1.0

import QMLBluetoothExtras 1.0
import Cellulo 1.0

/**
 * @addtogroup util
 * @{
 */

/**
 * @brief MAC address selection GUI
 *
 * TODO: Replace selectedAddress, connectionStatus, connectRequested() and disconnectRequested() with property CelluloBluetooth robot: null
 */
Row{

    /*
     * Public
     */

    /** type:bool Whether local adapter selection is enabled */
    property bool enableLocalAdapters: true

    /** type:list<string> List of possible MAC addresses, set by the user */
    property var addresses: []

    /** type:list<string> List of possible ids mapped to MAC addresses, set by the user */
    property var ids: []

    /** type:string Currently selected address. */
    readonly property string selectedAddress: selectionBox.currentText

    /** type:list<string> List of local adapter MAC addresses, set by the user */
    property var localAdapterAddresses: []

    /** type:string Currently selected local adapter address (only available in Linux). */
    readonly property string selectedLocalAdapterAddress: localAdapterSelectionBox.currentText

    /** type:enum Current connection status to display, set by the user, one of CelluloBluetoothEnums.ConnectionStatus */
    property var connectionStatus: CelluloBluetoothEnums.ConnectionStatusDisconnected

    /**
     * @brief Selects the given address if it exists in the list of addresses
     *
     * @param type:string address Address to look for in the list of addresses
     */
    function selectAddress(address){
        for(var i=0;i<addresses.length;i++)
            if(addresses[i] === address){
                selectionBox.currentIndex = i;
                return;
            }
        selectionBox.currentIndex = -1;
    }

    /**
     * @brief Selects the given local adapter address if it exists in the list of addresses
     *
     * @param type:string address Address to look for in the list of addresses
     */
    function selectLocalAdapterAddress(address){
        for(var i=0;i<localAdapterAddresses.length;i++)
            if(localAdapterAddresses[i] === address){
                localAdapterSelectionBox.currentIndex = i;
                return;
            }
        localAdapterSelectionBox.currentIndex = -1;
    }

    /** @brief Emitted when the connect button is pressed */
    signal connectRequested()

    /** @brief Emitted when the disconnect button is pressed */
    signal disconnectRequested()

    /** @brief User must call selectAddress() and selectLocalAdapterAddress() again when this signal is emitted */
    signal selectedAddrRefreshRequested()

    /*
     * Private
     */

    /** @cond DO_NOT_DOCUMENT */

    spacing: 5

    function em(x){ return Math.round(x*TextSingleton.font.pixelSize); }

    ComboBox{
        id: selectionBox

        anchors.verticalCenter: parent.verticalCenter

        editable: true
        width: em(12)
        model: addresses

        onModelChanged: selectedAddrRefreshRequested()

        onAccepted: {
            var tempModel = [];
            var exists = false;
            for(var i=0;i<model.length;i++){
                tempModel.push(model[i]);
                if(model[i] == editText)
                    exists = true;
            }
            if(!exists)
                tempModel.push(editText);
            model = tempModel;
            selectAddress(editText);
        }
    }

    Text{
        anchors.verticalCenter: parent.verticalCenter
        text: "through"
        visible: enableLocalAdapters
    }

    ComboBox{
        id: localAdapterSelectionBox

        anchors.verticalCenter: parent.verticalCenter

        visible: enableLocalAdapters
        enabled: enableLocalAdapters

        editable: true
        width: em(12)
        model: localAdapterAddresses

        onModelChanged: selectedAddrRefreshRequested()

        onAccepted: selectLocalAdapterAddress(editText)
    }

    Button{
        anchors.verticalCenter: parent.verticalCenter
        text: "Connect"
        onClicked: {
            if(selectionBox.acceptableInput)
                connectRequested();
        }
    }

    Button{
        anchors.verticalCenter: parent.verticalCenter
        text: "Disconnect"
        onClicked: disconnectRequested();
    }

    Text{
        anchors.verticalCenter: parent.verticalCenter
        text: CelluloBluetoothEnums.ConnectionStatusString(connectionStatus)
        color: {
            switch(connectionStatus){
            case CelluloBluetoothEnums.ConnectionStatusDisconnected:
                return "red";
            case CelluloBluetoothEnums.ConnectionStatusConnecting:
                return "yellow";
            case CelluloBluetoothEnums.ConnectionStatusConnected:
                return "green";
            }
        }
    }

    /** @endcond */
}

/** @} */
