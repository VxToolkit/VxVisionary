import QtQuick
import QtQuick.Controls

Button {
    id: vxWorkspaceButton

    property alias buttonSubtext: buttonSubtext.text
    property alias imageSource: buttonImage.source

    signal vxClicked()

    width: 100
    height: 100

    Rectangle {
        id: buttonBackground
        width: 100
        height: 100
        color: "#6e6e6e"
        border.color: "#c2c2c2"
        border.width: 1
        radius: 10

        states: [
            State {
                name: "PRESSED"
                when: mouseArea.pressed
                PropertyChanges {
                    target: buttonBackground
                    color: "#db4242"
                }
            },
            State {
                name: "HOVERED"
                when: mouseArea.containsMouse
                PropertyChanges {
                    target: buttonBackground
                    color: "#aa2c2c"
                }
            }
        ]

        transitions: [
            Transition {
                from: "*";
                to: "*"

                ColorAnimation {
                    duration: 40
                }

                NumberAnimation {
                    properties: "scale, border.width"
                    duration: 200
                    easing.type: Easing.OutQuad
                }
            }
        ]
        Column {
            anchors.centerIn: parent
            spacing: 5
            Image {
                id: buttonImage
                anchors.horizontalCenter: parent.horizontalCenter
                width: 48
                height: 48
            }
            Text {
                id: buttonSubtext
                anchors.horizontalCenter: parent.horizontalCenter
                color: "white"
                font.pixelSize: 14
                text: "Workspace"
            }
        }

        MouseArea {
            id: mouseArea
            anchors.fill: parent
            onClicked: vxWorkspaceButton.vxClicked()
            hoverEnabled: true
        }
    }
}
