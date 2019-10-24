import QtQuick 2.9
import QtQuick.Controls 1.4


TableView {

    id: tableView
    model: myModel

    anchors.horizontalCenter: parent.horizontalCenter

    width: parent.width
    height: parent.width

    TableViewColumn {

        role:"imgSrc"
        title: "Image"
        width: parent.width / 2

        resizable: true;
        delegate: Rectangle {

            border.color: "black"
            border.width: 2
            width: 400
            height: 500



            Image {


                anchors.margins: 10
                anchors.centerIn: parent
                height: parent.height / 2
                width: parent.width / 2
                asynchronous: true
                fillMode: Image.PreserveAspectFit


                source: styleData.value


            }
        }



    }


    TableViewColumn {

        role: "imgSrc"
        title: "NameFile"
        width: parent.width / 2

        resizable: true;
        delegate: Component {

            Text {

                text: qsTr(styleData.value)
            }

        }



    }


    rowDelegate: Rectangle {
        border.color: "black"
        anchors.margins: 10
        border.width: 1
        width: 400
        height: 400
    }



}




