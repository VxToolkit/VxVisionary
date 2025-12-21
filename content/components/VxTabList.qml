import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: root

    property var tabNames: []
    property int currentIndex: 0

    signal tabSelected(int index)
    signal tabClosed(int index)

    implicitHeight: 35
    implicitWidth: 600

    Rectangle {
        anchors.fill: parent
        color: "#1e1e1e"

        Rectangle {
            anchors.bottom: parent.bottom
            width: parent.width
            height: 1
            color: "#333333"
        }
    }

    ListView {
        id: tabView
        anchors.fill: parent
        orientation: ListView.Horizontal
        clip: true
        spacing: 1

        model: root.tabNames

        delegate: Rectangle {
            id: tabDelegate

            property bool isSelected: index === root.currentIndex
            property bool isHovered: mouseArea.containsMouse

            width: 150
            height: root.height

            color: isSelected ? "#2d2d2d" : (isHovered ? "#262626" : "#1e1e1e")

            Rectangle {
                width: parent.width
                height: 2
                anchors.top: parent.top
                color: "#a82828"
                visible: tabDelegate.isSelected
            }

            Text {
                text: modelData
                color: tabDelegate.isSelected ? "white" : "#909090"
                font.pixelSize: 12
                font.bold: tabDelegate.isSelected

                anchors.centerIn: parent
                anchors.horizontalCenterOffset: closeBtn.visible ? -10 : 0

                width: parent.width - 40
                elide: Text.ElideRight
                horizontalAlignment: Text.AlignHCenter
            }

            Rectangle {
                id: closeBtn
                width: 20
                height: 20
                radius: 3
                color: closeHover.containsMouse ? "#a82828" : "transparent"

                anchors.right: parent.right
                anchors.rightMargin: 8
                anchors.verticalCenter: parent.verticalCenter

                opacity: (tabDelegate.isHovered || tabDelegate.isSelected) ? 1.0 : 0.0
                visible: opacity > 0

                Behavior on opacity { NumberAnimation { duration: 150 } }

                Text {
                    text: "×"
                    color: "white"
                    font.pixelSize: 18
                    anchors.centerIn: parent
                    anchors.verticalCenterOffset: -1
                }

                MouseArea {
                    id: closeHover
                    anchors.fill: parent
                    hoverEnabled: true

                    onClicked: {
                        root.tabClosed(index)
                    }
                }
            }
            MouseArea {
                id: mouseArea
                anchors.fill: parent
                anchors.rightMargin: 30
                hoverEnabled: true

                onClicked: {
                    root.currentIndex = index
                    root.tabSelected(index)
                }
            }

            Rectangle {
                width: 1
                height: parent.height * 0.6
                color: "#333333"
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                visible: !tabDelegate.isSelected
            }
        }
    }
}