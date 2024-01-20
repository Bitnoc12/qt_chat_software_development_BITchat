import QtQuick 2.12
import QtQuick.Controls 2.12
import Qt.Singleton 1.0
import "qrc:/common"
Item {
    height: 60
    property int friendCnt: 0

    Column {
        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter
        anchors.leftMargin: 25
        spacing: 4
        KaiText {
            text: "用户名: " + AppCfg.currentUser
            font.pixelSize: 16
            color: "#BB223344"
        }
        KaiText {
            text: "在线好友: " +friendCnt
            font.pixelSize: 16
            color: "#BB223344"
        }
    }


    Rectangle {
        height: 1
        color: "#25777777"
        anchors.bottom: parent.bottom
        x:   0
        width: parent.width
    }
}
