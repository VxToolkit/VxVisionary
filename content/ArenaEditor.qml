import QtQuick
import QtQuick.Controls
import QtQuick.Effects
import QtQuick.Layouts

Window {
    id: window
    width: 1920
    height: 1080

    visible: true

    title: qsTr("Arena Editor")

    Rectangle {
        anchors.fill: parent
        color: "#202020"
    }

    VxEditorToolbar {
        title: "Arena Editor"

        // push to the right
        Item { Layout.fillWidth: true }

        VxToolButton {
            checkable: false
            imageSource: "../../assets/Icons/open-file.png"
            onVxClicked: {
                arenaEditor.openArena();
            }
        }

        VxToolButton {
            checkable: false
            imageSource: "../../assets/Icons/new-icon.png"
            onVxClicked: {
                arenaEditor.newArena();
            }

        }
    }

}