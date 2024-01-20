import QtQuick 2.12
import QtQuick.Controls 2.5

/* 用来解决外部mousearea时,无法相应鼠标事件 */
Rectangle {
    id: container
    property string btnText: ""
    property string bckcolor: "#1AAD19"
    property string bckHoverColor: Qt.lighter(bckcolor, 0.8)
    property int fontSize: 20
    property int backRadius: 0
    property bool containsMouse: area.containsMouse
    signal clicked();

    radius: backRadius
    color:  area.containsMouse ? bckHoverColor : bckcolor

    implicitWidth: 100
    implicitHeight: 40
    YaheiText {
        text: btnText
        font.pixelSize: fontSize
        color: "#FFF"
        anchors.centerIn: parent
    }

    MouseArea {
        id: area
        anchors.fill: parent
        propagateComposedEvents: true
        onClicked: {
            container.clicked();
        }
    }
}
