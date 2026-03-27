import QtQuick
import QtQuick.Window
import QtQuick.Controls

import "screens"

Window {
    id: mainWindow
    width: 360
    height: 640
    visible: true
    title: qsTr("Komadreja")
    color: "#1E1E1E"

    // Top Navigation (Stub)
    Rectangle {
        id: navBar
        width: parent.width
        height: 50
        color: "#2C2C2C"
        
        Row {
            anchors.centerIn: parent
            spacing: 20
            Button { text: "SAMPLE"; font.bold: true }
            Button { text: "SEQUENCE" }
            Button { text: "PERFORM" }
        }
    }

    // Main Content Area
    Item {
        id: contentArea
        width: parent.width
        anchors.top: navBar.bottom
        anchors.bottom: bottomBar.top

        SampleScreen {
            anchors.fill: parent
        }
    }

    // Bottom Bar (Stub)
    Rectangle {
        id: bottomBar
        width: parent.width
        height: 70
        anchors.bottom: parent.bottom
        color: "#2C2C2C"
        
        // Contextual buttons (Mic record, edit, etc.)
        Row {
            anchors.centerIn: parent
            spacing: 15
            Button { text: "MIC" }
            Button { text: "PITCH" }
            Button { text: "PAN" }
            Button { text: "VOL" }
        }
    }
}
