import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    id: inspectorRoot
    color: "#252525"

    PropertyModel {
        id: propModel
        targetObject: inspectorRoot.target
    }

    ListView {
        anchors.fill: parent
        anchors.margins: 10
        spacing: 10
        model: propModel
        clip: true

        delegate: RowLayout {
            width: ListView.view.width
            spacing: 10

            Text {
                text: propName
                color: "#cccccc"
                Layout.preferredWidth: 100
                elide: Text.ElideRight
            }

            Loader {
                Layout.fillWidth: true

                sourceComponent: {
                    if (propType === "bool") return boolComponent;
                    if (propType === "int" || propType === "float" || propType === "double") return numberComponent;
                    if (propType === "QColor") return colorComponent;
                    return stringComponent;
                }

                onLoaded: {
                    item.currentValue = propValue
                    item.isReadOnly = !isWritable
                }

                Connections {
                    target: inspectorRoot
                    function onTargetChanged() { if (item) item.currentValue = propValue }
                }
            }

            Component {
                id: boolComponent
                CheckBox {
                    checked: propValue
                    onToggled: propValue = checked
                }
            }

            Component {
                id: numberComponent
                TextField {
                    text: propValue.toString()
                    onEditingFinished: propValue = Number(text)
                    color: "white"
                    background: Rectangle { color: "#333" }
                }
            }

            Component {
                id: stringComponent
                TextField {
                    text: propValue.toString()
                    onEditingFinished: propValue = text
                    color: "white"
                    background: Rectangle { color: "#333" }
                }
            }
        }
    }
}