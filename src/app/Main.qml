import QtQuick
import QtQuick.Window
import QtQuick.Layouts
import QtQuick.Controls.Material

ApplicationWindow
{
    id: root

    required property QtObject deviceif

    width: 640
    height: 640
    visible: true
    title: qsTr("Device Controller")

    Material.theme: Material.Dark
    Material.accent: Material.LightBlue

    ColumnLayout
    {
        anchors.centerIn: parent

        RowLayout
        {
            id: stateRow

            Frame
            {
                implicitWidth: controllerFrame.implicitWidth/2 - stateRow.spacing/2

                ColumnLayout
                {
                    anchors.fill: parent

                    Text
                    {
                        Layout.bottomMargin: 8

                        text: qsTr("Last measurement")
                        color: "white"
                    }

                    TextField
                    {
                        Layout.fillWidth: true

                        placeholderText: qsTr("pressure")
                        readOnly: true
                        text: root.deviceif.pressure
                    }

                    TextField
                    {
                        Layout.fillWidth: true

                        placeholderText: qsTr("temperature")
                        readOnly: true
                        text: root.deviceif.temperature
                    }

                    TextField
                    {
                        Layout.fillWidth: true

                        placeholderText: qsTr("velocity")
                        readOnly: true
                        text: root.deviceif.velocity
                    }
                }
            }

            Frame
            {
                implicitWidth: controllerFrame.implicitWidth/2 - stateRow.spacing/2

                ColumnLayout
                {
                    anchors.fill: parent

                    Text
                    {
                        Layout.bottomMargin: 8

                        text: qsTr("Current configuration")
                        color: "white"
                    }

                    TextField
                    {
                        Layout.fillWidth: true

                        placeholderText: qsTr("frequency")
                        readOnly: true
                        text: root.deviceif.frequency
                    }

                    TextField
                    {
                        Layout.fillWidth: true

                        placeholderText: qsTr("debug")
                        readOnly: true
                        text: root.deviceif.debug
                    }

                    TextField
                    {
                        Layout.fillWidth: true

                        placeholderText: qsTr("Last response")
                        readOnly: true
                        text: root.deviceif.lastResponse
                    }
                }
            }
        }

        Frame
        {
            id: controllerFrame

            RowLayout
            {
                ColumnLayout
                {
                    Button
                    {
                        text: qsTr("Start")
                        onClicked: root.deviceif.start()
                    }

                    Button
                    {
                        text: qsTr("Stop")
                        onClicked: root.deviceif.stop()
                    }
                }

                ColumnLayout
                {
                    RowLayout
                    {
                        TextField
                        {
                            id: frequencyInput

                            implicitWidth: 220
                            implicitHeight: 38

                            Material.accent: frequencyInput.acceptableInput ? Material.Green : Material.Red
                            placeholderText: qsTr("Frequency(0-255)")
                            validator: IntValidator
                            {
                                bottom: 1
                                top: 255
                            }
                        }

                        CheckBox
                        {
                            id: debugCheckbox

                            text: qsTr("Debug")
                            checked: false
                        }
                    }

                    Button
                    {
                        Layout.alignment: Qt.AlignRight
                        text: qsTr("Configure")
                        onClicked: root.deviceif.configure(frequencyInput.text, debugCheckbox.checked)
                        enabled: frequencyInput.acceptableInput
                    }
                }
            }
        }
    }
}
