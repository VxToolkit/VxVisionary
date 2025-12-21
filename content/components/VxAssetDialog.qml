import QtQuick
import QtQuick.Controls
import VxtVisionary.Models

Window {
    id: assetDialog

    property alias assetModel: assetList.model
    property string selectedAsset: ""

    title: "Select Asset"
    flags: Qt.Dialog | Qt.WindowStaysOnTopHint
    modality: Qt.WindowModal
    width: 400
    height: 300

    Rectangle {
        anchors.fill: parent
        color: "#202020"
    }

    signal assetSelected(string assetName)

    VxListView {
        id: assetList
        anchors.fill: parent
        anchors.margins: 10

        onItemClicked: (index, name) => {
            selectedAsset = name
        }

        onItemDoubleClicked: (index, name) => {
            selectedAsset = name
            assetSelected(name)
        }
    }
}