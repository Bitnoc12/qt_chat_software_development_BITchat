import QtQuick 2.12
import QtGraphicalEffects 1.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12
import Qt.Singleton 1.0
import "qrc:/common"

Item {
    signal returnRequest();
    HintPopup {
        id: popup
        width : 330
        height: 120
        parent: Overlay.overlay
        x: Math.round((parent.width - width) / 2)
        y: Math.round((parent.height - height) / 2)
        hintColor: SkinSingleton.skins[AppCfg.skinIndex].popupColor
        hint: "你确定要退出,更换用户吗?"
        onAccept: returnRequest();
        modal: true
        dim: false
    }
    HintPopup {
        id: bit_popup
        width : 330
        height: 120
        parent: Overlay.overlay
        x: Math.round((parent.width - width) / 2)
        y: Math.round((parent.height - height) / 2)
        hintColor: SkinSingleton.skins[AppCfg.skinIndex].popupColor
        hint: "进入北理工https://bit.edu.cn/"
        onAccept: Qt.openUrlExternally("https://bit.edu.cn/")
        modal: true
        dim: false
    }
    ColumnLayout {
        y: 12
        anchors.horizontalCenter:  parent.horizontalCenter
        spacing: 30
        HeadImage {
            id: _head
            radius: 4
            width : 40
            height : _head.width
            headUrl: AppCfg.headImageSmall
            Layout.alignment: Qt.AlignHCenter
            Button {
                id: _headBtn
                anchors.fill: parent
                contentItem: Rectangle {
                    color: "transparent"
                }
                background: Rectangle {
                    id: _headBtnBack
                    color: "transparent"
                    border.width:  2
                    border.color: _headBtn.hovered ? "#2B90F3" :  "transparent"
                    radius: _head.radius
                }
                ToolTip.delay: 200            // 徘徊在按钮上超过200ms则显示,则默认为直接显示
                ToolTip.visible: hovered       // 当鼠标徘徊在按钮上时,则显示ToolTip
                ToolTip.text: qsTr("用户名: "+ AppCfg.currentUser)
            }
        }

        ImageButton {
            id: _bubbleBtn
            imageSrc: "qrc:/res/bubble1.png"
            backHoverColor: "#22FFFFFF"
            ToolTip.delay: 1000            // 徘徊在按钮上超过200ms则显示,则默认为直接显示
            ToolTip.visible: hovered       // 当鼠标徘徊在按钮上时,则显示ToolTip
            ToolTip.text: qsTr("气泡")
            onClicked: {
               bubble_popup.indicatorOpen(_bubbleBtn);
            }
            Layout.preferredWidth: 30
            Layout.preferredHeight: 30
            Layout.alignment: Qt.AlignHCenter
        }
        ImageButton {
            imageSrc: "qrc:/res/skin1.png"
            backHoverColor: "#22FFFFFF"
            ToolTip.delay: 1000            // 徘徊在按钮上超过500ms则显示,则默认为直接显示
            ToolTip.visible: hovered       // 当鼠标徘徊在按钮上时,则显示ToolTip
            ToolTip.text: qsTr("背景皮肤")
            onClicked:  skin_popup.open();
            Layout.preferredWidth: 30
            Layout.preferredHeight: 30
            Layout.alignment: Qt.AlignHCenter
        }



        ImageButton {
            id: _transferBtn
            imageSrc: "qrc:/res/fileTransfer.png"
            backHoverColor: "#22FFFFFF"
            ToolTip.delay: 1000            // 徘徊在按钮上超过500ms则显示,则默认为直接显示
            ToolTip.visible: hovered       // 当鼠标徘徊在按钮上时,则显示ToolTip
            ToolTip.text: qsTr("传输列表")
            onClicked: {
               file_popup.indicatorOpen(_transferBtn);
            }
            Layout.preferredWidth: 30
            Layout.preferredHeight: 30
            Layout.alignment: Qt.AlignHCenter
            clip: false
            //FileManager
            Rectangle {
                color: "#FF0000"
                width: 16
                height: 16
                radius: 8
                x: _transferBtn.width - width/2
                y: -width/2+3
                z: 80
                //visible: FileManager.underwayCnt != 0
                Text {
                    text: FileManager.underwayCnt > 99 ? "..." : FileManager.underwayCnt
                    anchors.centerIn: parent
                    font.pixelSize: 11
                    color: "#FFF"
                    font.family: "Microsoft Yahei"
                }
            }

            Component.onCompleted: {
                downloadFileAnimation.transferIcnObj = _transferBtn
            }
        }

        ImageButton {
            imageSrc: "qrc:/res/return.png"
            backHoverColor: "#22FFFFFF"
            ToolTip.delay: 1000            // 徘徊在按钮上超过500ms则显示,则默认为直接显示
            ToolTip.visible: hovered       // 当鼠标徘徊在按钮上时,则显示ToolTip
            ToolTip.text: qsTr("退出")
            onClicked: popup.open();
            Layout.preferredWidth: 30
            Layout.preferredHeight: 30
            Layout.alignment: Qt.AlignHCenter
        }

        ImageButton {

            imageSrc: "qrc:/res/addfriend.png"
            backHoverColor: "#22FFFFFF"
            ToolTip.delay: 1000            // 徘徊在按钮上超过500ms则显示,则默认为直接显示
            ToolTip.visible: hovered       // 当鼠标徘徊在按钮上时,则显示ToolTip
            ToolTip.text: qsTr("添加好友")
            onClicked:
               addFriend_popup.open();
            Layout.preferredWidth: 30
            Layout.preferredHeight: 30
            Layout.alignment: Qt.AlignHCenter
        }

        ImageButton {

            imageSrc: "qrc:/res/about2.png"
            backHoverColor: "#22FFFFFF"
            ToolTip.delay: 1000            // 徘徊在按钮上超过500ms则显示,则默认为直接显示
            ToolTip.visible: hovered       // 当鼠标徘徊在按钮上时,则显示ToolTip
            ToolTip.text: qsTr("点击北理工")
            onClicked:
               bit_popup.open();
            Layout.preferredWidth: 30
            Layout.preferredHeight: 30
            Layout.alignment: Qt.AlignHCenter
        }
    }

    BubblePopup {
           id: bubble_popup
    }

    AddFriendPopup {
           id: addFriend_popup
    }

    SkinPopup {
           id: skin_popup
    }

    FileTransferPopup {
           id: file_popup
    }


}
