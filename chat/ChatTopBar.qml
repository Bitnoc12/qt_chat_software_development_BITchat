import QtQuick 2.12
import QtQuick.Controls 2.12
import "qrc:/common"

Item {
    height: 60
    property string title: ""

    Text {
        id: _text
        text: title
        anchors.verticalCenter: parent.verticalCenter
        x: 40
        font.pixelSize: 22
        font.family: "Microsoft Yahei"
        color: "#BB000000"
    }

    Rectangle {
        height: 1
        color: "#25777777"
        anchors.bottom: parent.bottom
        x:   0
        width: parent.width
    }

}
