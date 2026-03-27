import QtQuick
import QtQuick.Controls

Item {
    id: sampleScreen

    // Display bank selector
    Row {
        id: bankSelector
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.margins: 10
        spacing: 10
        
        Repeater {
            model: ["A", "B", "C", "D"]
            Button {
                text: modelData
                width: 40
                height: 40
            }
        }
    }

    // Pad Grid
    GridView {
        id: gridView
        anchors.top: bankSelector.bottom
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: 10
        cellWidth: width / 4
        cellHeight: width / 4
        interactive: false // disable scrolling
        
        model: padModel // provided via C++ context

        delegate: Rectangle {
            width: gridView.cellWidth - 10
            height: gridView.cellHeight - 10
            radius: 8
            color: padArea.pressed ? Qt.darker(model.padColor, 1.2) : model.padColor
            
            border.color: "#FFFFFF"
            border.width: 1
            
            Text {
                anchors.centerIn: parent
                text: model.padName
                color: "white"
                font.bold: true
                font.pixelSize: 18
            }

            MouseArea {
                id: padArea
                anchors.fill: parent
                onPressed: {
                    if (mainWindow.assignRecordingMode) {
                        audioEngine.assignRecordingToPad(model.padIndex)
                        mainWindow.assignRecordingMode = false
                    } else {
                        audioEngine.playSample(model.padIndex, 1.0)
                        scaleAnim.restart()
                    }
                }
            }

            SequentialAnimation on scale {
                id: scaleAnim
                running: false
                NumberAnimation { to: 0.9; duration: 50; easing.type: Easing.OutQuad }
                NumberAnimation { to: 1.0; duration: 50; easing.type: Easing.InQuad }
            }
        }
    }
}
