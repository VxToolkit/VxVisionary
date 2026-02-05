import QtQuick
import QtQuick.Controls
import QtQuick.Effects
import QtQuick.Layouts

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
        visible: false

        StackView {
            id: projectStack
            anchors.fill: parent
            anchors.margins: 30
            initialItem: selectionPage

            pushEnter: Transition {
                NumberAnimation { property: "opacity"; from: 0; to: 1; duration: 200 }
                NumberAnimation { property: "x"; from: 100; to: 0; duration: 200; easing.type: Easing.OutCubic }
            }
            popExit: Transition {
                NumberAnimation { property: "opacity"; from: 1; to: 0; duration: 200 }
                NumberAnimation { property: "x"; from: 0; to: 100; duration: 200; easing.type: Easing.InCubic }
            }
        }
    }

    Component {
        id: selectionPage
        Item {
            width: projectStack.width
            height: projectStack.height

            ColumnLayout {
                anchors.fill: parent
                spacing: 20

                ColumnLayout {
                    Layout.fillWidth: true
                    spacing: 5
                    Text {
                        text: "Enter Project Name:"
                        color: "white"
                        font.pixelSize: 14
                    }
                    TextField {
                        id: projectNameField
                        Layout.fillWidth: true
                        placeholderText: "Project Name..."
                        color: "white"
                        background: Rectangle {
                            color: "#333333"
                            radius: 4
                            border.color: projectNameField.activeFocus ? "#db3434" : "#555555"
                        }
                    }
                }

                Text {
                    text: "Presets"
                    font.pixelSize: 20
                    font.bold: true
                    color: "white"
                }

                VxSelectorFrame {
                    //Layout.fillWidth: true
                    anchors.horizontalCenter: parent.horizontalCenter
                    Layout.preferredHeight: 110
                    title: "Blank"
                    description: "Create a project from scratch."
                    imageSource: "../../assets/Icons/blank-icon.png"
                    onClicked: projectStack.push(templatePage, {
                        "presetName": "Blank",
                        "projectName": projectNameField.text
                    })
                }

                Rectangle {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 1
                    color: "#3d3d3d"
                }

                ScrollView {
                    id: presetsScroll
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    clip: true

                    Grid {
                        id: presetGrid
                        width: presetsScroll.availableWidth
                        columns: 2
                        columnSpacing: 15
                        rowSpacing: 15

                        property real itemWidth: (width - columnSpacing) / 2

                        VxSelectorFrame {
                            width: presetGrid.itemWidth
                            height: 110
                            title: "Push Back"
                            description: "VRC 2025-2026 blueprint."
                            imageSource: "../../assets/Icons/pushback-icon.png"
                            onClicked: projectStack.push(templatePage, {
                                "presetName": "Push Back",
                                "projectName": projectNameField.text
                            })
                        }

                        VxSelectorFrame {
                            width: presetGrid.itemWidth
                            height: 110
                            title: "Preset 1"
                            description: "User defined preset."
                            imageSource: "../../assets/Icons/open-file.png"
                            onClicked: projectStack.push(templatePage, {
                                "presetName": "Preset 1",
                                "projectName": projectNameField.text
                            })
                        }
                    }
                }
            }
        }
    }

    Component {
        id: templatePage
        Item {
            width: projectStack.width
            height: projectStack.height
            property string presetName: ""
            property string projectName: ""

            Column {
                anchors.fill: parent; spacing: 20
                Text { text: "Configure: " + presetName; font.pixelSize: 24; font.bold: true; color: "white" }
                Text { text: "Project: " + (projectName === "" ? "Untitled" : projectName); color: "#888888" }

                Rectangle {
                    width: parent.width; height: 200; color: "#2a2a2a"; border.color: "#3d3d3d"; radius: 4
                    Text { anchors.centerIn: parent; text: "Settings for " + presetName; color: "#666666" }
                }

                Row {
                    spacing: 10; anchors.horizontalCenter: parent.horizontalCenter
                    VxButton { text: "Back"; onVxClicked: projectStack.pop() }
                    VxButton {
                        text: "Create Project"
                        onVxClicked: {
                            console.log("Creating: " + projectName)
                            projectOnboardingDialog.visible = false
                        }
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