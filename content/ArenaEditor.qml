import QtQuick
import QtQuick.Controls
import QtQuick.Effects
import QtQuick.Layouts

Window {
    id: window
    width: 1920
    height: 1080

    visible: true

    title: qsTr("Arena Editor")

    Rectangle {
        anchors.fill: parent
        color: "#202020"
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        VxEditorToolbar {
            title: "Arena Editor"
            Layout.fillWidth: true


            // push to the right
            Item {
                Layout.fillWidth: true
            }

            VxToolButton {
                checkable: false
                imageSource: "../../assets/Icons/open-file.png"
                onVxClicked: {
                    arenaEditor.openArena();
                }
            }

            VxToolButton {
                checkable: false
                imageSource: "../../assets/Icons/new-file.png"
                onVxClicked: {
                    arenaEditor.newArena();
                }

            }
        }

        VxTabList {
            id: tabList
            Layout.fillWidth: true
            tabNames: arenaEditor.tabs
            height: 35

            onTabSelected: (index) => {
                arenaEditor.setCurrentTab(index);
            }

            onTabClosed: (index) => {
                arenaEditor.deleteTab(index);
            }
        }

        Rectangle {
            id: arenaEditorContainer
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "#181818"

            SplitView {
                anchors.fill: parent
                orientation: Qt.Horizontal

                Rectangle {
                    id: canvasContainer
                    color: "#000000"
                    SplitView.fillWidth: true
                    SplitView.minimumWidth: 400

                    Text {
                        text: "Arena Viewport"
                        color: "grey"
                        anchors.centerIn: parent
                    }

                    Canvas {
                        id: arenaCanvas
                        Layout.fillHeight: true
                    }
                }

                ColumnLayout {
                    Layout.fillHeight: true
                    width: 300

                    implicitWidth: 300
                    SplitView.minimumWidth: 200
                    SplitView.maximumWidth: 500

                    TreeView {
                        id: arenaElementsList
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        model: arenaEditor.currentElementsModel

                        columnWidthProvider: function (column) { return width }

                        delegate: Item {
                            width: arenaElementsList.width
                            implicitHeight: label.implicitHeight * 1.5

                            readonly property real indent: 20
                            readonly property real padding: 5

                            required property bool selected
                            required property bool expanded
                            required property int depth
                            required property string display

                            Rectangle {
                                id: bgRect
                                anchors.fill: parent
                                color: parent.selected ? "#a82828" : "transparent"
                            }

                            Text {
                                id: label
                                text: parent.display
                                color: "white"
                                x: padding + (parent.depth * parent.indent)
                                anchors.verticalCenter: parent.verticalCenter
                            }

                            MouseArea {
                                anchors.fill: parent
                                onClicked: {
                                }
                            }
                        }
                    }
                    Rectangle {
                        id: arenaElementButtonContainer
                        Layout.fillWidth: true
                        Layout.preferredHeight: 30
                        color: "#282828"

                        RowLayout {
                            anchors.fill: parent
                            spacing: 5

                            VxButton {
                                text: "+"
                                Layout.fillWidth: true
                                Layout.fillHeight: true
                                onVxClicked: {
                                    arenaEditor.addElementToCurrentArena();
                                }
                            }

                            VxButton {
                                text: "-"
                                Layout.fillWidth: true
                                Layout.fillHeight: true
                                onVxClicked: {
                                    arenaEditor.removeSelectedElementFromCurrentArena();
                                }
                            }
                        }
                    }
                }
            }

        }
    }
}