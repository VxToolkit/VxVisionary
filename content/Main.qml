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

    VxButton {
        text: "New Project"
        onVxClicked: {
            appController.createNewProject()
        }
        anchors.top: projectList.bottom
        anchors.topMargin: 10
        anchors.horizontalCenter: parent.horizontalCenter

    }
}