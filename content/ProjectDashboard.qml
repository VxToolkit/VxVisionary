import QtQuick
import QtQuick.Controls
import QtQuick.Effects
import QtQuick.Layouts

Window {
    id: window
    width: 850
    height: 500

    maximumHeight: 600
    maximumWidth: 350

    minimumHeight: 600
    minimumWidth: 350
    visible: true

    title: qsTr("Dashboard")

    Rectangle {
        anchors.fill: parent
        color: "#202020"
    }

    MenuBar {
        Menu {
            title: "File"
            MenuItem {
                text: "New Project"
                onTriggered: {
                    appController.createNewProject()
                }
            }
            MenuItem {
                text: "Exit"
                onTriggered: {
                    Qt.quit()
                }
            }
            MenuItem {
                text: "Save"
                onTriggered: {
                    appController.saveCurrentProject()
                }
            }
        }
    }

    Column {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 20

        Image {
            id: logoImage
            source: "../assets/VXT-Logo-Large.png"
            anchors.horizontalCenter: parent.horizontalCenter

            width: 400
            height: 100
            transformOrigin: Item.Center
        }

        Text {
            id: titleText
            text: appController.projectName
            color: "white"
            font.pixelSize: 24
            font.bold: true
            horizontalAlignment: Text.AlignHCenter
            anchors.horizontalCenter: parent.horizontalCenter
        }

        Rectangle {
            id: projectWorkspacesContainer
            width: parent.width * 0.9
            height: 300
            anchors.horizontalCenter: parent.horizontalCenter

            color: "#333333"
            border.width: 2
            border.color: "#a82828"
            radius: 10

            RowLayout {
                spacing: 65
                VxWorkspaceSelector {
                    imageSource: "../../assets/Workspaces/arena-editor.png"
                    buttonSubtext: "Arena Editor"
                    onVxClicked: {
                        appController.openWorkspace("Arena Editor")
                    }
                }
                VxWorkspaceSelector {
                    imageSource: "../../assets/Workspaces/mission-editor.png"
                    buttonSubtext: "Mission Editor"
                    onVxClicked: {
                        appController.openWorkspace("Mission Editor")
                    }
                }
            }
        }
    }
}