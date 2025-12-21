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

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        VxEditorToolbar {
            title: "Arena Editor"
            Layout.fillWidth: true


            // push to the right
            Item {
                Layout.fillWidth: true
            }

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

        VxTabList {
            id: tabList
            Layout.fillWidth: true
            tabNames: arenaEditor.tabs
            height: 35

            onTabSelected: (index) => {
                arenaEditor.setCurrentTab(index);
            }

            onTabClosed: (index) => {
                arenaEditor.deleteTab(index);
            }
        }

        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true
        }
    }
}