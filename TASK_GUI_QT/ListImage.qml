import QtQuick 2.0
import QtQuick.Controls 2.2



ListView {

    id: listview


    spacing: 1

    model: myModel
    clip: true
    anchors.fill: parent
    anchors.margins: 5


    delegate: Item {

        width: parent.width
        height: 400


        Image {

            fillMode: Image.PreserveAspectFit
            height: 300

            asynchronous: true
            anchors.centerIn: parent
            source: imgSrc

        }


    }


    ScrollBar.vertical: ScrollBar {
        active: true
        policy: ScrollBar.AlwaysOn
        width: 10
    }


    highlightMoveDuration: 50
}
