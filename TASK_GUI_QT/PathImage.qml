import QtQuick 2.0


PathView {
    id: view
    anchors.fill: parent
    anchors.bottomMargin: 50
    anchors.topMargin: 10
    pathItemCount: 3
    preferredHighlightBegin: 0.5
    preferredHighlightEnd: 0.5
    highlightRangeMode: PathView.StrictlyEnforceRange
    model: myModel
    clip: true
    delegate: Item {

        width: 300
        height: 300


        Image {


            fillMode: Image.Stretch
            anchors.fill: parent
            asynchronous: true

            source: imgSrc


        }
    }
    path: Path {
        startX: 0
        startY: 0

        PathQuad {
            x: view.width
            y: 0
            controlX: view.width / 2
            controlY: view.height
        }
    }
}
