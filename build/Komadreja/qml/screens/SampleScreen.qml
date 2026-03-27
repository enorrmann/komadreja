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
            
            // Check if this pad has a sample assigned
            property bool hasAudio: audioEngine.hasSample(model.padIndex)
            
            // Empty pad is slightly darker, filled pad uses its normal color
            // Pressing an empty pad shows red, pressing a filled pad shows a darker version of its color
            color: hasAudio ? (padArea.pressed ? Qt.darker(model.padColor, 1.2) : model.padColor) 
                            : (padArea.pressed ? "#e74c3c" : Qt.darker(model.padColor, 2.0))
            
            // Add a clear border when empty vs filled
            border.color: hasAudio ? "#FFFFFF" : "#555555"
            border.width: 1
            
            Text {
                anchors.centerIn: parent
                text: model.padName
                color: hasAudio ? "white" : "#AAAAAA"
                font.bold: true
                font.pixelSize: 18
            }

            MouseArea {
                id: padArea
                anchors.fill: parent
                property bool isRecordingOnThisPad: false

                onPressed: {
                    if (!parent.hasAudio) {
                        // It's empty: Start recording
                        audioEngine.startRecording()
                        isRecordingOnThisPad = true
                    } else {
                        // It's full: Play the sample
                        audioEngine.playSample(model.padIndex, 1.0)
                        scaleAnim.restart()
                    }
                }

                onCanceled: {
                    if (isRecordingOnThisPad) {
                        audioEngine.stopRecording()
                        isRecordingOnThisPad = false
                    }
                }

                onReleased: {
                    if (isRecordingOnThisPad) {
                        audioEngine.stopRecording()
                        audioEngine.assignRecordingToPad(model.padIndex)
                        isRecordingOnThisPad = false
                        // Update visual status
                        parent.hasAudio = audioEngine.hasSample(model.padIndex)
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
