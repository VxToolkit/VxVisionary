import QtQuick
import QtQuick.Controls

Item {
    id: root
    width: 450
    height: 120

    property string imageSource: ""
    property string title: "Item Title"
    property string description: "Description text goes here"
    property string badgeText: ""
    property color accentColor: "#ff4444"
    property bool selected: false

    signal clicked()

    Rectangle {
        id: mainFrame
        anchors.fill: parent
        color: "#202020"
        radius: 8
        border.width: root.selected ? 2 : 1

        border.color: root.selected ? root.accentColor : (mouseArea.containsMouse ? root.accentColor : "#3d3d3d")

        Behavior on border.color {
            ColorAnimation { duration: 200 }
        }

        Row {
            anchors.fill: parent
            anchors.margins: 10
            spacing: 15

            Rectangle {
                id: imageContainer
                width: parent.height - 20
                height: imageContainer.width
                radius: 6
                clip: true
                color: "#1a1a1a"

                Image {
                    id: thumbnail
                    source: root.imageSource
                    anchors.fill: parent
                    fillMode: Image.PreserveAspectCrop
                }

                Rectangle {
                    anchors.fill: parent
                    gradient: Gradient {
                        orientation: Gradient.Horizontal
                        GradientStop { position: 0.5; color: "transparent" }
                        GradientStop { position: 1.0; color: "#202020" }
                    }
                }
            }

            Column {
                width: parent.width - imageContainer.width - (root.badgeText !== "" ? 85 : 25)
                height: parent.height
                spacing: 6
                anchors.verticalCenter: parent.verticalCenter

                Text {
                    id: titleText
                    text: root.title
                    font.pixelSize: 16
                    font.bold: true
                    color: "white"
                    elide: Text.ElideRight
                    width: parent.width
                }

                Text {
                    id: descText
                    text: root.description
                    font.pixelSize: 13
                    color: "#aaaaaa"
                    wrapMode: Text.WordWrap
                    maximumLineCount: 3
                    lineHeight: 1.1
                    elide: Text.ElideRight
                    width: parent.width
                }
            }

            Item {
                width: 70
                height: parent.height
                visible: root.badgeText !== ""

                Rectangle {
                    width: parent.width
                    height: 20
                    color: root.accentColor
                    radius: 4
                    anchors.verticalCenter: parent.verticalCenter

                    Text {
                        text: root.badgeText
                        color: "white"
                        font.pixelSize: 9
                        font.bold: true
                        anchors.centerIn: parent
                        horizontalAlignment: Text.AlignHCenter
                    }
                }
            }
        }

        MouseArea {
            id: mouseArea
            anchors.fill: parent
            hoverEnabled: true
            onClicked: root.clicked()
        }
    }
}