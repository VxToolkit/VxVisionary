import QtQuick
import QtQuick.Controls
import QtQuick.Effects

Window {
    id: window
    width: 600
    height: 350

    maximumHeight: 600
    maximumWidth: 350

    minimumHeight: 600
    minimumWidth: 350
    visible: true

    title: qsTr("VXTVisionary")

    Rectangle {
        anchors.fill: parent
        color: "#202020"
    }

    Image {
        id: logoImage
        source: "../assets/VXT-Logo-Large.png"
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter

        anchors.topMargin: 0

        width: 400
        height: 100
        transformOrigin: Item.Top

    }

    MenuBar {
        Menu {
            title: "File"
            MenuItem {
                text: "Exit"
                onTriggered: {
                    Qt.quit()
                }
            }
            MenuItem {
                text: "Load from file..."
                onTriggered: {
                    appController.loadProjectFromFile()
                }
            }
        }
    }

    Text {
        id: titleText
        text: "Select a project to continue"
        color: "white"
        font.pixelSize: 16
        anchors.top: logoImage.bottom
        anchors.topMargin: -10
        anchors.horizontalCenter: parent.horizontalCenter
    }

    VxListView {
        id: projectList

        width: parent.width * 0.8
        height: 150
        anchors.top: logoImage.bottom
        anchors.topMargin: 30
        anchors.horizontalCenter: parent.horizontalCenter

        model: appController.projectModel

        onItemClicked: (index, name) => {
            console.log("Selected: " + name)
        }

        onItemDoubleClicked: (index, name) => {
            appController.loadProject(name)
        }
    }

    VxDialog {
        id: projectOnboardingDialog
        dialogTitle: "Project Creation"
        transientParent: window
        width: 1024
        height: 600

        Column {
            width: parent.width - 4
            height: parent.height - 40
            anchors.centerIn: parent
            spacing: 15

            Text {
                text: "Enter Project Name:"
                color: "white"
            }

            TextField {
                width: parent.width
                placeholderText: "Project Name..."
            }

            Text {
                text: "Templates"
                font.pixelSize: 20
                font.bold : true
                color: "white"
            }

            VxSelectorFrame {
                anchors.horizontalCenter: parent.horizontalCenter
                title: "Blank"
                description: "Create a project from scratch."
                imageSource: "../../assets/Icons/blank-icon.png"
                badgeText: ""
                accentColor: "#db3434"
                onClicked: projectOnboardingDialog.visible = false
            }

            Rectangle {
                width: parent.width
                height: 1
                color: "#3d3d3d"
            }

            Text {
                text: "Blueprints"
                font.pixelSize: 20
                font.bold : true
                color: "white"
            }

            ScrollView {
                id: blueprintScroll
                width: parent.width
                height: 250
                clip: true

                Flow {
                    id: blueprintFlow
                    width: blueprintScroll.width
                    spacing: 15

                    VxSelectorFrame {
                        width: (parent.width / 2) - 10
                        title: "Push Back"
                        description: "A blueprint for the 2025-2026 VRC Push Back game."
                        imageSource: "../../assets/Icons/pushback-icon.png"
                    }

                    VxSelectorFrame {
                        width: (parent.width / 2) - 10
                        title: "template1"
                        description: "yo"
                        imageSource: "../../assets/Icons/open-file.png"
                    }
                }
            }
        }
    }

    VxButton {
        text: "New Project"
        onVxClicked: {
            projectOnboardingDialog.visible = true
        }
        anchors.top: projectList.bottom
        anchors.topMargin: 10
        anchors.horizontalCenter: parent.horizontalCenter

    }
}