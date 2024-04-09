import QtQuick
import QtQuick.Window
import QtQuick.Layouts
import QtQuick.Controls.Material

ApplicationWindow
{
    id: root

    required property QtObject deviceif

    width: 640
    height: 480
    visible: true
    title: qsTr("Device Controller")

    Material.theme: Material.Dark
    Material.accent: Material.LightBlue

    ColumnLayout
    {
        anchors.centerIn: parent

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

        Button
        {
            text: qsTr("Configure")
            onClicked: root.deviceif.configure(frequencyInput.text, debugCheckbox.checked)
            enabled: frequencyInput.acceptableInput
        }
    }
}
