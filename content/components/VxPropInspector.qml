import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    id: inspectorRoot
    color: "#202020"

    property var inspectorModel: null

    ListView {
        anchors.fill: parent
        anchors.margins: 10
        spacing: 10
        clip: true

        model: inspectorRoot.inspectorModel

        delegate: RowLayout {
            width: ListView.view.width
            spacing: 15

            Text {
                text: propName
                color: "#cccccc"
                font.pixelSize: 14
                Layout.preferredWidth: 100
                Layout.alignment: Qt.AlignVCenter
                elide: Text.ElideRight
            }

            Loader {
                Layout.fillWidth: true
                Layout.preferredHeight: 30

                sourceComponent: {
                    if (propType === "int" || propType === "float" || propType === "double") return numberEditor;
                    return stringEditor;
                }

                onLoaded: {
                    item.originalValue = propValue
                }

                Connections {
                    target: inspectorRoot.inspectorModel
                    function onDataChanged() {
                        if (item) item.originalValue = propValue
                    }
                }
            }

            Component {
                id: numberEditor
                TextField {
                    property var originalValue
                    text: originalValue.toString()
                    color: "white"
                    font.pixelSize: 14
                    background: Rectangle {
                        color: "#333333"
                        radius: 4
                        border.color: parent.activeFocus ? "#a82828" : "transparent"
                    }

                    onEditingFinished: {
                        propValue = Number(text)
                    }
                }
            }

            Component {
                id: stringEditor
                TextField {
                    property var originalValue
                    text: originalValue.toString()
                    color: "white"
                    font.pixelSize: 14
                    background: Rectangle {
                        color: "#333333"
                        radius: 4
                        border.color: parent.activeFocus ? "#a82828" : "transparent"
                    }

                    onEditingFinished: {
                        propValue = text
                    }
                }
            }
        }
    }

    Text {
        anchors.centerIn: parent
        text: "No Selection"
        color: "#505050"
        visible: !inspectorRoot.inspectorModel || inspectorRoot.inspectorModel.rowCount() === 0
    }
}