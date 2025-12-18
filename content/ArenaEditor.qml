import QtQuick
import QtQuick.Controls
import QtQuick.Effects
import QtQuick.Layouts

Window {
    id: window
    width: 1920
    height: 1080

    visible: true

    title: qsTr("Arena Editor" - appController.projectName)

    Rectangle {
        anchors.fill: parent
        color: "#202020"
    }
}