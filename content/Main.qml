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
                    if (appController.confirmCloseWithUnsavedChanges()) {
                        Qt.quit()
                    }
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

        onItemClicked: (index, name, path) => {
            console.log("Selected: " + name + " at " + path)
        }

        onItemDoubleClicked: (index, name, path) => {
            appController.loadProject(path)
        }
    }

    VxDialog {
        id: projectOnboardingDialog
        dialogTitle: "Project Creation"
        transientParent: window
        width: 1024
        height: 600
        visible: false

        onVisibleChanged: {
            if (!visible) {
                projectStack.pop(null)
                tempProjectName = ""
            }
        }

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
                        onTextChanged: projectOnboardingDialog.tempProjectName = text
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
        id: customTemplatePage
        Item {
            width: projectStack.width
            height: projectStack.height

            property string projectName: ""

            Text {
                text: "Configure custom template"
                color: "white"
                font.pixelSize: 24
                anchors.top: parent.top
                anchors.horizontalCenter: parent.horizontalCenter
            }

            Text {
                text: "Please enter a valid git repository URL or local directory path for your custom template."
                color: "#888888"
                font.pixelSize: 14
                anchors.top: parent.top
                anchors.topMargin: 50
                anchors.horizontalCenter: parent.horizontalCenter
            }

            Row {
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 70
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: 20
                VxButton {
                    text: "Create Project"
                    onVxClicked: {
                        var templateSource = templateSourceField.text.trim()
                        if (templateSource === "") {
                            console.log("Template source cannot be empty.")
                            return
                        }

                        var type = templateSourceDropdown.currentText === "Local Directory" ? "Directory" : "Url"
                        appController.makeProject(projectName, "Custom", templateSource, type)

                        window.projectOnboardingDialog.visible = false
                    }
                }

                VxButton {
                    text: "Cancel"
                    onVxClicked: projectStack.pop()
                }
            }

            VxDropdown {
                id: templateSourceDropdown
                anchors.top: parent.top
                anchors.topMargin: 120
                anchors.horizontalCenter: parent.horizontalCenter
                width: parent.width * 0.6
                placeholderText: "Select Template Source"
                model: ["Git Repository", "Local Directory"]
            }

            TextField {
                id: templateSourceField
                anchors.top: templateSourceDropdown.bottom
                anchors.topMargin: 20
                anchors.horizontalCenter: parent.horizontalCenter
                width: parent.width * 0.6
                placeholderText: templateSourceDropdown.currentText === "Git Repository" ? "Enter Git Repository URL..." : "Enter Local Directory Path..."
                color: "white"
                rightPadding: templateSourceDropdown.currentText === "Local Directory" ? 40 : 10
                background: Rectangle {
                    color: "#333333"
                    radius: 4
                    border.color: templateSourceField.activeFocus ? "#db3434" : "#555555"
                }
            }

            Rectangle {
                id: folderSelectButton
                visible: templateSourceDropdown.currentText === "Local Directory"
                width: 32
                height: 32

                anchors.left: templateSourceField.right
                anchors.leftMargin: 4
                anchors.verticalCenter: templateSourceField.verticalCenter
                color: folderMouseArea.containsMouse ? "#555555" : "#434343"
                radius: 3

                Image {
                    source: "../../assets/Icons/folder.png"
                    width: 20
                    height: 20
                    anchors.centerIn: parent
                }

                MouseArea {
                    id: folderMouseArea
                    anchors.fill: parent
                    hoverEnabled: true
                    cursorShape: Qt.PointingHandCursor
                    onClicked: {
                        var dir = appController.openDirectoryDialog()
                        if (dir) {
                            templateSourceField.text = dir
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
            property string selectedMode: "vxtemplate"

            ColumnLayout {
                anchors.fill: parent
                spacing: 20

                ColumnLayout {
                    spacing: 5
                    Text {
                        text: "Select Base Template"
                        font.pixelSize: 24
                        font.bold: true
                        color: "white"
                    }
                    Text {
                        text: "Project: " + (projectName || "Untitled") + "  •  Blueprint: " + presetName
                        color: "#888888"
                        font.pixelSize: 14
                    }
                }

                Rectangle {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 1
                    color: "#3d3d3d"
                }

                ColumnLayout {
                    Layout.fillWidth: true
                    spacing: 15

                    VxSelectorFrame {
                        Layout.fillWidth: true
                        Layout.preferredHeight: 90
                        title: "VxTemplate"
                        description: "The template build specifically for use with other VX tools."
                        badgeText: "Recommended"
                        imageSource: "../../assets/Logo.png"
                        accentColor: "#4CAF50"

                        selected: selectedMode === "vxtemplate"
                        onClicked: selectedMode = "vxtemplate"
                    }

                    VxSelectorFrame {
                        Layout.fillWidth: true
                        Layout.preferredHeight: 90
                        title: "Custom Template"
                        description: "Choose your own git repository/local directory for full control over your project template."
                        imageSource: "../../assets/Icons/custom.png"
                        accentColor: "#2196F3"

                        selected: selectedMode === "custom"
                        onClicked: selectedMode = "custom"
                    }
                }

                Item { Layout.fillHeight: true }

                Row {
                    Layout.alignment: Qt.AlignHCenter
                    spacing: 20


                    VxButton {
                        text: "Create Project"
                        onVxClicked: {
                            console.log("Creating Project:", projectName)
                            console.log("Blueprint:", presetName)
                            console.log("Custom Template Mode:", selectedMode)

                            if (selectedMode === "custom") {
                                projectStack.push(customTemplatePage, {
                                    "projectName": projectName
                                })
                            }
                            else {
                                appController.makeProject(projectName, presetName, "https://github.com/VxToolkit/VxTemplate.git", "Url")

                                window.projectOnboardingDialog.visible = false
                                selectedMode = "vxtemplate"
                            }
                        }
                    }

                    VxButton {
                        text: "Back"
                        onVxClicked: projectStack.pop()
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