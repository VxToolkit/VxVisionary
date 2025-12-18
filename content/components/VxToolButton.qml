import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: vxWorkspaceButton

    property alias imageSource: buttonImage.source

    signal vxClicked()

    property bool checkable: false
    property bool checked: false

    width: 32
    height: 32

    Rectangle {
        id: buttonBackground
        anchors.fill: parent
        color: "#6e6e6e"
        radius: 5

        states: [
            State {
                name: "PRESSED"
                when: mouseArea.pressed
                PropertyChanges { target: buttonBackground; color: "#db4242"; scale: 0.95 }
            },
            State {
                name: "HOVERED"
                when: mouseArea.containsMouse && !mouseArea.pressed
                PropertyChanges { target: buttonBackground; color: "#aa2c2c" }
            }
        ]

        transitions: Transition {
            ColorAnimation { duration: 100 }
            NumberAnimation { properties: "scale"; duration: 100; easing.type: Easing.OutQuad }
        }
    }

    Image {
        id: buttonImage
        anchors.centerIn: parent
        width: 26
        height: 26
        fillMode: Image.PreserveAspectFit
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true

        onClicked: vxWorkspaceButton.vxClicked()
    }
}