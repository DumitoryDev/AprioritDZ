import QtQuick 2.0

Rectangle {
    id: rectangle
    property string text: "Список" // переменная, содержащая текст, который будет отображаться в кнопке
    width: 100 // ширина
    height: 30 // высота
    color: marea.pressed ?  "orange" : "lightblue" // зависимость цвета кнопки состояния нажатия MouseArea
    Text
    {
        verticalAlignment: Text.AlignVCenter // выравнивание текста по вертикали внутри поля TEXT
        horizontalAlignment: Text.AlignHCenter // выравнивание текста по горизонтали внутри поля TEXT
        anchors.fill: parent // растягиваем птекстовое поле во всю ширину родительского объекта (т.е. во весь прямоугольник/кнопку)
        text: parent.text
    }
    MouseArea
    {
        id: marea // id для работы с этим полем (как имя объекта в С++)
        anchors.fill: parent // и поле тоже растягиваем на всю кнопку
    }
    states: [
        State {
            name: "State1"
        }
    ]
}

