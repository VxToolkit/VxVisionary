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

    Text {
        id: titleText
        text: "Select a project to continue"
        color: "white"
        font.pixelSize: 16
        anchors.top: logoImage.bottom
        anchors.topMargin: -10
        anchors.horizontalCenter: parent.horizontalCenter
    }

    Rectangle {
        id: listMask
        width: parent.width * 0.8
        height: 150

        anchors.top: logoImage.bottom
        anchors.topMargin: 30

        anchors.horizontalCenter: parent.horizontalCenter

        color: "#444444"
        border.width: 2
        border.color: "#a82828"
        clip: true

        ListView {
            id: projectsList
            anchors.fill: parent
            clip: true

            delegate: Rectangle {
                width: parent.width
                height: 20

                color: ListView.isCurrentItem ? "#db2828" : "transparent"

                Text {
                    color: ListView.isCurrentItem ? "black" : "white"
                    text: model.name
                    anchors.centerIn: parent
                    font.bold: ListView.isCurrentItem
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        projectsList.currentIndex = index
                    }
                    onDoubleClicked: {
                        appController.loadProject(model.name)
                    }
                }
            }

            model: appController.projectModel

        }
    }
    VxButton {
        text: "New Project"
        onVxClicked: {
            appController.createNewProject()
        }
        anchors.top: listMask.bottom
        anchors.topMargin: 10
        anchors.horizontalCenter: parent.horizontalCenter

    }
}