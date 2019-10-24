import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Dialogs 1.2

ListView{

    model: myModel

    FileDialog {
        id: fileDialog
        title: "Please choose a directory"
        selectFolder: true
        folder: shortcuts.home
        visible: false

        onAccepted: {

            var path = fileDialog.fileUrl.toString();
            // remove prefixed "file:///"
            path = path.replace(/^(file:\/{3})/,"");
            var cleanPath = decodeURIComponent(path);
            console.log(cleanPath)

            myModel.update_image(cleanPath)

        }
        onRejected: {
            console.log("Canceled")

        }

    }


    Column {
        width: 100

        anchors.centerIn: parent
        spacing: 2

        Button {

            text: qsTr("Select folder")
            background: Rectangle {

                color: "darkgray"
            }
            onClicked: fileDialog.visible = true
        }
        Button {

            text: qsTr("Remove last image")
            background: Rectangle {

                color: "darkgray"
            }
            onClicked: myModel.delete_last_image();
        }

    }
}
