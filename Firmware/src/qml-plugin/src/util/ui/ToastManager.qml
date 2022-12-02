import QtQuick 2.0

/**
 * @addtogroup util
 * @{
 */

/**
 * @brief Creates on-screen banner texts, like Android's `Toast`s. Ensures that multiple toasts show correctly together.
 */
Column{

    /**
     * Public
     */

    /**
     * @brief Shows a Toast
     *
     * @param type:string text Text to show
     * @param type:real duration Duration to show in milliseconds, defaults to 3000
     */
    function show(text, duration){
        var toast = toastComponent.createObject(root);
        toast.selfDestroying = true;
        toast.show(text, duration);
    }

    /**
     * Private
     */

    /** @cond DO_NOT_DOCUMENT */

    id: root

    z: Infinity
    spacing: 5
    anchors.centerIn: parent

    property var toastComponent

    Component.onCompleted: toastComponent = Qt.createComponent("Toast.qml")

    /** @endcond */
}

/** @} */
