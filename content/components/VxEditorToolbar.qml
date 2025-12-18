import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    id: root

    default property alias content: layout.data

    property string title: ""

    width: parent.width
    height: 50
    color: "#2a2a2a"

    Rectangle {
        width: parent.width; height: 1
        anchors.bottom: parent.bottom
        color: "#111111"
    }

    RowLayout {
        id: layout
        anchors.fill: parent
        anchors.leftMargin: 15
        anchors.rightMargin: 15
        spacing: 10

        Text {
            text: root.title
            visible: root.title !== ""
            color: "#cccccc"
            font.bold: true
            font.pixelSize: 14
            Layout.rightMargin: 10
            verticalAlignment: Text.AlignVCenter
        }
    }

    SplitView {
        Rectangle {
            color: "#3a3a3a"
            anchors.centerIn: parent
            anchors.fill: parent
        }
        Rectangle {
            color: "#3a3a3a"
            anchors.centerIn: parent
            anchors.fill: parent
        }
    }
}