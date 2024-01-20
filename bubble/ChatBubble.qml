import QtQuick 2.12
import QtQuick.Controls 2.5
import "BubbleNormal.js" as BubbleNormal
import "BubbleBlue.js" as BubbleBlue
import "BubbleBlack.js" as BubbleBlack
import "BubblePink.js" as BubblePink
import "BubbleFreedom.js" as BubbleFreedom
import Qt.Singleton 1.0
import "qrc:/common"
GifText {
    id: container

    property string msgText: ""
    property bool isSend: true
    property int iconHeight: 40
    property int maxWidth: 0

    textFormat: TextEdit.PlainText
    height: contentHeight+20
    Canvas {
        id: canvas
        parent: back
        anchors.fill: back
        onPaint: {
            bubble().drawBubble(getContext('2d'));
        }
    }

    background: Rectangle {
        id: back
        anchors.fill: parent
        color: "transparent"
    }
    z: 2
    MouseArea {
        anchors.fill: parent
        onClicked: {
            debug();
            Model.clipboard(coding());
            message('success','复制文本成功!');
        }
    }

    Component.onCompleted: {
       encoding(msgText);
       width = contentWidth > maxWidth ? maxWidth : contentWidth+28

    }

    font.pixelSize: 15
    font.family: "Microsoft Yahei"
    horizontalAlignment: Text.AlignLeft
    verticalAlignment: Text.AlignVCenter
    padding: 8
    leftPadding: isSend ? 8 : 20
    rightPadding: isSend ? 20 : 8
    readOnly: false
    color: isSend ? "#FFFFFF" : "#0E0E0E"

    function bubble() {
        switch (AppCfg.bubbleIndex) {
            case "0" :  return BubbleNormal
            case "1" :  return BubbleBlue
            case "2" :  return BubbleBlack
            case "3" :  return BubblePink
            //case "4" :  return BubbleFreedom
            default: return BubbleBlue
        }
    }

    Connections {
        target: AppCfg;
        onBubbleIndexChanged:{
             canvas.requestPaint();
        }
        onSkinIndexChanged: {
            if (AppCfg.bubbleIndex == "0") {
                 canvas.requestPaint();
            }
        }
    }

}
