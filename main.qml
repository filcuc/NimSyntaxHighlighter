import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Window 2.0
import QtQuick.Dialogs 1.2

ApplicationWindow {
    width: 800
    height: 600
    visible: true

    Component {
        id: fileDialogComponent
        FileDialog {
            id: fileDialog
            title: "Please choose a file"
            selectFolder: false
            selectMultiple: false
            selectExisting: true
            onAccepted: applicationLogic.openFile(fileDialog.fileUrl);
            Component.onCompleted: visible = true
        }
    }

    menuBar: MenuBar {
        Menu {
            title: "File"
            MenuItem {
                text: "Open"
                onTriggered: fileDialogComponent.createObject()
            }
            MenuItem {
                text: "Exit"
                onTriggered: Qt.quit()
            }
        }
    }

    Component.onCompleted: applicationLogic.openFile(":/TestFile.nim");


    TextArea {
        id: textArea
        anchors.fill: parent
        text: applicationLogic.text
        Component.onCompleted: {
            console.log(textArea.textDocument)
            applicationLogic.setDocument(textArea.textDocument)
        }
    }
}
