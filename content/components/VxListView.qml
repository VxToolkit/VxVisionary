import QtQuick
import QtQuick.Controls

Rectangle {
    id: root

    property alias model: internalList.model

    signal itemClicked(int index, string name)
    signal itemDoubleClicked(int index, string name)

    property color listBackgroundColor: "#444444"
    property color borderColor: "#a82828"
    property color highlightColor: "#db2828"
    property color textColor: "white"
    property color selectedTextColor: "black"

    width: 300
    height: 150
    color: listBackgroundColor
    border.width: 2
    border.color: borderColor
    clip: true

    ListView {
        id: internalList
        anchors.fill: parent

        ScrollBar.vertical: ScrollBar { }

        delegate: Rectangle {
            id: rowDelegate
            width: internalList.width
            height: 20

            readonly property bool isSelected: ListView.isCurrentItem

            color: isSelected ? root.highlightColor : "transparent"

            Text {
                text: model.name
                color: isSelected ? root.selectedTextColor : root.textColor
                anchors.centerIn: parent
                font.bold: isSelected
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    internalList.currentIndex = index
                    root.itemClicked(index, model.name)
                }
                onDoubleClicked: {
                    root.itemDoubleClicked(index, model.name)
                }
            }
        }
    }
}