import QtQuick
import QtQuick.Controls
import VxtVisionary.Models

Dialog {
    id: assetDialog

    property alias assetModel: assetList.model
    property string selectedAsset: ""

    title: "Select Asset"
    modal: true
    standardButtons: Dialog.Cancel

    signal assetSelected(string assetName)

    onAccepted: () => {
        assetSelected(selectedAsset)
    }

    onRejected: () => {
        assetSelected("")
    }

    width: 400
    height: 300

    VxListView {
        id: assetList
        anchors.fill: parent
        anchors.margins: 10

        onItemClicked: (index, name) => {
            selectedAsset = name
        }

        onItemDoubleClicked: (index, name) => {
            selectedAsset = name
            assetDialog.accept()
        }
    }
}