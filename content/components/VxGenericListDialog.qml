import QtQuick
import QtQuick.Controls
import VxtVisionary.Models
import QtQuick.Layouts

Window {
    id: genericDialog

    property alias listModel: assetList.model
    property string selectedElement: ""
    property string dialogTitle: "Select an Element"

    flags: Qt.Dialog | Qt.WindowStaysOnTopHint
    modality: Qt.WindowModal
    width: 400
    height: 300
    title: dialogTitle

    Rectangle {
        anchors.fill: parent
        color: "#202020"
    }

    signal elementSelected(string elementName)

    ColumnLayout {
        anchors.fill: parent
        spacing: 20
        Text {
            Layout.fillWidth: true
            id: title
            text: dialogTitle
            color: "white"
            font.pixelSize: 32
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }
        VxStrListView {
            Layout.fillWidth: true
            Layout.fillHeight: true
            id: assetList

            onItemClicked: (index, name) => {
                selectedElement = name
            }

            onItemDoubleClicked: (index, name) => {
                selectedElement = name
                elementSelected(name)
            }
        }
    }
}