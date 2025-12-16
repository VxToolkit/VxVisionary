import QtQuick
import QtQuick.Controls

import Main

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("VXTVisionary")

    AppController {
        id: backend
    }

    Column {
        anchors.centerIn: parent
        spacing: 20

        Text {
            text: backend.statusMessage
            font.pixelSize: 24
            anchors.horizontalCenter: parent.horizontalCenter
        }

        Button {
            text: "Do Work"
            anchors.horizontalCenter: parent.horizontalCenter
            onClicked: backend.performAction()
        }
    }
}