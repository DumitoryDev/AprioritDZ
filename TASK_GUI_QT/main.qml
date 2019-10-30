import QtQuick 2.9
import QtQuick.Controls 1.4
import QtQuick.Window 2.2


Window {
    id: root
    visible: true
    width: Screen.width /2
    height: Screen.height /2
    title: qsTr("Show images")

    TabView {
        id: tabView
        width: parent.width
        height: parent.height

        Tab{
            title: "Select folder"

            SelectImage{
                anchors.fill: parent
            }



        }

        Tab {

            title: "Path"

            width: parent.width
            height: parent.height

            PathImage{
                anchors.fill: parent
            }

        }

        Tab {
            id: tabTwo
            title: "List"
            width: parent.width
            height: parent.height

            ListImage{
                anchors.fill: parent
            }


        }


        Tab {
            title: "Table"
            width: parent.width
            height: parent.height

            TableImage{

                anchors.fill: parent
            }
        }


    }

}
