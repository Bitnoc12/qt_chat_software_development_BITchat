
import QtQuick 2.10

Item {
    id: messageBox
    width: msgBox.width
    y: 10
    anchors {
        horizontalCenter: parent.horizontalCenter
    }
    //提示框内容
    property alias text: msg.text
    property bool show: false
    property var type: "info"
    visible: show
    //执行定时器
    function openTimer() {
        timerId.start()
        messageBox.y = 30
    }
    Behavior on y {
        NumberAnimation {
            duration: 200
        }
    }
    Timer {
        id: timerId
        interval: 3000
        repeat: false
        onTriggered: {

             messageBox.y = 10
             timerHide.start()
        }
    }
    Timer {
        id: timerHide
        interval: 300
        repeat: false
        onTriggered: {
            show = false
        }
    }
    //$Utils.boxWidth封装的一个函数 计算元素的宽度  自身宽度+leftMargin+rightMargin
    Rectangle {
        id: msgBox
        clip: true
        width: 450
        height: 35
        color:  type=== "info" ? "#fdf6ec" : type=== "success" ? "#f0f9eb" : "#fef0f0"
        border.color:type=== "info" ? "#faecd8" : type=== "success" ? "#e1f3d8" : "#fde2e2"
        radius: 5
        Image {
            id: img
            source: type
                    === "info" ? "remind.png" : type
                                 === "success" ? "true.png" : "error.png"
            width: 24
            height: 24
            anchors {
                verticalCenter: parent.verticalCenter
                left: parent.left
                leftMargin: 15
            }
        }
        Text {
            id: msg
            color: type=== "info" ? "#e6a23c" : type=== "success" ? "#67c23a" : "#f56c6c"
            font.pixelSize: 14
            font.family: "Microsoft Yahei"
            anchors {
                verticalCenter: img.verticalCenter
                left: img.right
                leftMargin: 10
            }
        }
    }
}
