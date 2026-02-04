import QtQuick
import QtQuick.Controls

Window {
    id: root

    // Custom Properties
    property alias dialogTitle: titleLabel.text
    default property alias content: container.children
    property color accentColor: "#ff4444"

    width: 400
    height: 300
    flags: Qt.Dialog | Qt.FramelessWindowHint
    modality: Qt.WindowModal
    visible: false

    Rectangle {
        anchors.fill: parent
        color: "#202020"
        border.color: "#333333"
        border.width: 1

        DragHandler {
            onActiveChanged: if (active) root.startSystemMove()
        }

        Item {
            id: header
            width: parent.width
            height: 40

            Text {
                id: titleLabel
                text: "Dialog Title"
                color: "#aaaaaa"
                font.pixelSize: 14
                anchors.left: parent.left
                anchors.leftMargin: 15
                anchors.verticalCenter: parent.verticalCenter
            }

            Rectangle {
                width: 40
                height: 40
                color: closeMouse.containsMouse ? accentColor : "transparent"
                anchors.right: parent.right

                Text {
                    text: "✕"
                    color: "white"
                    anchors.centerIn: parent
                }

                MouseArea {
                    id: closeMouse
                    anchors.fill: parent
                    hoverEnabled: true
                    onClicked: root.close()
                }
            }
        }

        Item {
            id: container
            anchors.top: header.bottom
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.margins: 20
        }
    }
}