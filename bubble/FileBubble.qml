import QtQuick 2.12
import QtQuick.Controls 2.5
import "BubbleNormal.js" as BubbleNormal
import "BubbleBlue.js" as BubbleBlue
import "BubbleBlack.js" as BubbleBlack
import Qt.Singleton 1.0
import "qrc:/common"
import QtQuick.Layouts 1.12
Item {
    id: container

    property string msgText: ""
    property bool isSend: true
    property int iconHeight: 40
    property int maxWidth: 0
    property string fileName : ""
    property var size: 0
    property string desc: ""
    signal save(var imageObj);
    height: 90
    width: 200
    Canvas {
        id: canvas
        parent: container
        anchors.fill: container
        onPaint: {
            bubble().drawBubble(getContext('2d'));

        }
    }

    z: 2

    Rectangle {
        anchors.fill: parent
        anchors.margins: 6
        anchors.leftMargin: isSend ? 6 : 20
        anchors.rightMargin: isSend ? 20 : 6
        color: "#FFFFFF"
        border.color : "#77FFFFFF"
        radius: 4
        ColumnLayout {
            anchors.fill: parent
            RowLayout {
                Layout.leftMargin: 8
                Image {
                    id: _image
                    fillMode: Image.PreserveAspectFit
                    source: fileContent.getIcon(fileName)
                    width: 30
                    height: width
                }

                ColumnLayout {
                    YaheiText {
                        text: fileName
                        font.pixelSize: 13
                        color: "#333"
                        elide: Text.ElideMiddle
                        clip: true
                        Layout.preferredWidth: 175

                    }
                    YaheiText {
                        text: fileContent.getFileSize(size)
                        font.pixelSize: 11
                        color: "#777"
                    }
                }

            }
            Rectangle {
                height: 1
                color: "#ddd"
                Layout.fillWidth: true
            }

            RowLayout {
                Layout.margins: 4
                Layout.topMargin: 1
                Layout.leftMargin: 8
                YaheiText {
                    text: desc
                    font.pixelSize: 11
                    Layout.fillWidth: true
                    color: "#888"
                }
                BaseButton {
                    id: _save
                    btnText: "另存为"
                    visible: !isSend
                    fontSize: 11
                    Layout.preferredWidth: 44
                    Layout.preferredHeight: 18
                    backRadius: 3
                    bckcolor: SkinSingleton.skins[AppCfg.skinIndex].popupColor
                    onClicked: {
                        save(_image);
                    }
                    focusPolicy: Qt.NoFocus
                }

                BaseButton {
                    id: _copy
                    btnText: "复制"
                    fontSize: 11
                    Layout.preferredWidth: 44
                    Layout.preferredHeight: 18
                    backRadius: 3
                    bckcolor: SkinSingleton.skins[AppCfg.skinIndex].popupColor
                    onClicked: {
                        Model.clipboard(fileName);
                        message('success','复制文件名成功!');
                    }
                    focusPolicy: Qt.NoFocus
                }
            }

        }

    }




    function bubble() {
        switch (AppCfg.bubbleIndex) {
            case "0" :  return BubbleNormal
            case "1" :  return BubbleBlue
            case "2" :  return BubbleBlack
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
