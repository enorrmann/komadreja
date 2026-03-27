import QtQuick
import QtQuick.Controls

Rectangle {
    id: padRect
    property int padIndex: -1
    property color padColor: "gray"
    property string padName: ""
    
    radius: 8
    color: padArea.pressed ? Qt.darker(padColor, 1.2) : padColor
    
    border.color: "#FFFFFF"
    border.width: 1
    
    Text {
        anchors.centerIn: parent
        text: padName
        color: "white"
        font.bold: true
        font.pixelSize: 18
    }

    MouseArea {
        id: padArea
        anchors.fill: parent
        onPressed: {
            audioEngine.playSample(padIndex, 1.0)
            scaleAnim.restart()
        }
    }

    SequentialAnimation on scale {
        id: scaleAnim
        running: false
        NumberAnimation { to: 0.9; duration: 50; easing.type: Easing.OutQuad }
        NumberAnimation { to: 1.0; duration: 50; easing.type: Easing.InQuad }
    }
}
