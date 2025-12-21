import QtQuick
import QtQuick.Controls

Item {
    id: vxButton

    property alias text: buttonLabel.text
    property bool isHighlighted : true

    signal vxClicked()

    width: 150
    height: 50
    scale: 0.75

    Rectangle {
        id: buttonBackground
        anchors.fill: parent
        color: "#db3434"
        radius: 10

        MouseArea {
            id: mouseArea
            anchors.fill: parent
            onClicked: vxButton.vxClicked()
            hoverEnabled: true
        }

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
                from: "*"; to: "*"

                ColorAnimation { duration: 40 }

                NumberAnimation {
                    properties: "scale, border.width"
                    duration: 200
                    easing.type: Easing.Bezier
                }
            }
        ]

        Text {
            id: buttonLabel
            anchors.centerIn: parent
            color: "white"
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }
    }
}