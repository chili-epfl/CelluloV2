import QtQuick 2.5
import QtQuick.Window 2.0
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.2
import Cellulo 1.0

ApplicationWindow{
    id: window
    visible: true

    width: root.width
    height: root.height

    maximumHeight: height
    maximumWidth: width

    minimumHeight: height
    minimumWidth: width

    Item{
        id: root

        width: childrenRect.width
        height: childrenRect.height

        Column{
            spacing: 5

            Text{
                text: "Be careful on Windows, the input and output files must be inside the below directories!"
                visible: Qt.platform.os === "winrt"
            }

            Row{
                spacing: 5

                Text{ text: "Input file:" }
                TextField{
                    id: inFile
                    width: 500
                    anchors.verticalCenter: parent.verticalCenter
                    placeholderText: "Input .svg file"
                    text: Qt.platform.os === "winrt" ? CelluloSystemUtil.getAppDataLocation() : ""
                }
                Button{
                    text: "Choose file"
                    anchors.verticalCenter: parent.verticalCenter
                    onClicked: inputFilePicker.open()
                    enabled: Qt.platform.os !== "winrt"
                }
            }

            Row{
                spacing: 5

                Text{ text: "Output file:" }
                TextField{
                    id: outFile
                    width: 500
                    anchors.verticalCenter: parent.verticalCenter
                    placeholderText: "Output .json file"
                    text: Qt.platform.os === "winrt" ? CelluloSystemUtil.getAppDataLocation() : ""
                }
                Button{
                    text: "Choose file"
                    anchors.verticalCenter: parent.verticalCenter
                    onClicked: outputFilePicker.visible = true
                    enabled: Qt.platform.os !== "winrt"
                }
            }

            Row{
                spacing: 5

                Text{
                    text: "Name:"
                    anchors.verticalCenter: parent.verticalCenter
                }
                TextField{
                    id: name
                    width: 500
                    anchors.verticalCenter: parent.verticalCenter
                    placeholderText: "Prefix for all zone names"
                    text: "SVG_DUMP_ZONE"
                }
            }

            Row{
                spacing: 5

                Text{
                    text: "DPI:"
                    anchors.verticalCenter: parent.verticalCenter
                }
                TextField{
                    id: dpi
                    width: 500
                    anchors.verticalCenter: parent.verticalCenter
                    placeholderText: "(inkscape default is 90)"
                    text: "90"
                    inputMethodHints: Qt.ImhFormattedNumbersOnly
                }
            }

            Row{
                spacing: 5

                CheckBox{
                    id: optimize
                    anchors.verticalCenter: parent.verticalCenter
                    text: "Optimize, i.e try to extract as lines, circles (TODO), points (TODO), rectangles and polygons?"
                    checked: true
                }
            }

            Button{
                text: "Go"
                onClicked: toast.show(CelluloSVGUtil.dumpAllPathsToJSON(inFile.text, outFile.text, "POLYBEZIERCLOSESTT", name.text, parseFloat(dpi.text), optimize.checked))
            }
        }

        FileDialog{
            id: inputFilePicker
            nameFilters: [ "SVG files (*.svg)", "All files (*)" ]
            title: "Choose input file"
            folder: shortcuts.home
            selectFolder: false
            selectExisting: true
            selectMultiple: false
            onAccepted: inFile.text = fileUrl.toString()
        }

        FileDialog{
            id: outputFilePicker
            nameFilters: [ "JSON file (*.json)", "All files (*)" ]
            title: "Choose output file"
            folder: shortcuts.home
            selectFolder: false
            selectExisting: false
            selectMultiple: false
            onAccepted: outFile.text = fileUrl.toString()
        }

        ToastManager{ id: toast }
    }
}
