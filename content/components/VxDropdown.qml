import QtQuick
import QtQuick.Controls

Item {
    id: vxDropdown

    property alias model: comboBox.model
    property alias currentIndex: comboBox.currentIndex
    property alias currentText: comboBox.currentText
    property string placeholderText: "Select..."

    signal activated(int index)

    width: 200
    height: 40

    ComboBox {
        id: comboBox
        anchors.fill: parent

        onActivated: function(index) {
            vxDropdown.activated(index)
        }

        delegate: ItemDelegate {
            width: comboBox.width
            contentItem: Text {
                text: modelData
                color: highlighted ? "white" : "#cccccc"
                font.pixelSize: 14
                verticalAlignment: Text.AlignVCenter
                elide: Text.ElideRight
            }
            background: Rectangle {
                color: highlighted ? "#db3434" : "#2a2a2a"

                Rectangle {
                    width: parent.width
                    height: 1
                    anchors.bottom: parent.bottom
                    color: "#3d3d3d"
                }
            }
            highlighted: comboBox.highlightedIndex === index
        }

        indicator: Canvas {
            id: canvas
            x: comboBox.width - width - 10
            y: comboBox.topPadding + (comboBox.availableHeight - height) / 2
            width: 12
            height: 8
            contextType: "2d"

            Connections {
                target: comboBox
                function onPressedChanged() { canvas.requestPaint() }
            }

            onPaint: {
                context.reset()
                context.moveTo(0, 0)
                context.lineTo(width, 0)
                context.lineTo(width / 2, height)
                context.closePath()
                context.fillStyle = comboBox.pressed ? "#db4242" : "#db3434"
                context.fill()
            }
        }

        contentItem: Text {
            leftPadding: 15
            rightPadding: comboBox.indicator.width + comboBox.spacing

            text: comboBox.displayText || vxDropdown.placeholderText
            font.pixelSize: 14
            color: comboBox.displayText ? "white" : "#888888"
            verticalAlignment: Text.AlignVCenter
            elide: Text.ElideRight
        }

        background: Rectangle {
            color: "#333333"
            radius: 4
            border.color: comboBox.activeFocus ? "#db3434" : "#555555"
            border.width: comboBox.activeFocus ? 2 : 1

            Behavior on border.color {
                ColorAnimation { duration: 150 }
            }
            Behavior on border.width {
                NumberAnimation { duration: 150 }
            }
        }

        popup: Popup {
            y: comboBox.height + 2
            width: comboBox.width
            implicitHeight: contentItem.implicitHeight
            padding: 0

            contentItem: ListView {
                clip: true
                implicitHeight: contentHeight
                model: comboBox.popup.visible ? comboBox.delegateModel : null
                currentIndex: comboBox.highlightedIndex

                ScrollIndicator.vertical: ScrollIndicator {
                    active: true
                    contentItem: Rectangle {
                        implicitWidth: 6
                        color: "#db3434"
                        radius: 3
                    }
                }
            }

            background: Rectangle {
                color: "#2a2a2a"
                border.color: "#db3434"
                border.width: 1
                radius: 4

                layer.enabled: true
                layer.effect: Item {
                    Rectangle {
                        anchors.fill: parent
                        color: "transparent"
                        radius: 4
                        border.color: "#000000"
                        border.width: 1
                        opacity: 0.3
                    }
                }
            }

            enter: Transition {
                NumberAnimation { property: "opacity"; from: 0.0; to: 1.0; duration: 150 }
                NumberAnimation { property: "y"; from: comboBox.height - 10; to: comboBox.height + 2; duration: 150; easing.type: Easing.OutCubic }
            }

            exit: Transition {
                NumberAnimation { property: "opacity"; from: 1.0; to: 0.0; duration: 100 }
            }
        }
    }
}
