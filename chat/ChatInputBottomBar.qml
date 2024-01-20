import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12
import Qt.Singleton 1.0
import "qrc:/common"
import "qrc:/emoji"
Rectangle {
    id: bottom

    SelectFileDialog {
        id: _file
        nameFilters: ["*"]
        onAccepted: {
            console.log("选择了:  "+files[0])

            Model.sendFile(files[0]);
        }
    }

    color: area.activeFocus ? "#5FFFFFFF" : "#22FFFFFF"
    ColumnLayout {
        spacing: 0
        anchors.fill: parent
        RowLayout {
            Layout.fillWidth: true
            Layout.preferredHeight: 30
            Layout.leftMargin: 12
            Layout.rightMargin: 12
            spacing: 5
            ImageButton {
                id: emoji
                imageSrc: "qrc:/res/emoji.png"
                backHoverColor:"#20334455"
                ToolTip.delay: 700              // 徘徊在按钮上超过500ms则显示,则默认为直接显示
                ToolTip.visible: hovered       // 当鼠标徘徊在按钮上时,则显示ToolTip
                ToolTip.text: qsTr("选择表情")
                Layout.preferredWidth: 21
                Layout.preferredHeight: 21
                Layout.alignment : Qt.AlignVCenter | Qt.AlignLeft
                onClicked: {
                    emoji_popup.posOpen(emoji);
                }
                focusPolicy: Qt.NoFocus
            }

            ImageButton {
                id: fileSend
                imageSrc: "qrc:/res/fileSend.png"
                backHoverColor:"#20334455"
                ToolTip.delay: 700              // 徘徊在按钮上超过500ms则显示,则默认为直接显示
                ToolTip.visible: hovered       // 当鼠标徘徊在按钮上时,则显示ToolTip
                ToolTip.text: qsTr("文件发送")
                Layout.preferredWidth: 21
                Layout.preferredHeight: 21
                Layout.alignment : Qt.AlignVCenter | Qt.AlignLeft
                onClicked: {
                    _file.open();

                }
                focusPolicy: Qt.NoFocus
            }

            ScaleImageButton {
                id: scaleInput
                backHoverColor:"#20334455"
                ToolTip.delay: 700
                ToolTip.visible: hovered
                ToolTip.text: shrink ? "放大" : "缩小"
                Layout.preferredWidth: 21
                Layout.preferredHeight: 21
                Layout.alignment : Qt.AlignVCenter | Qt.AlignLeft
                focusPolicy: Qt.NoFocus

                onShrinkChanged: {
                    bottom.state = shrink ? "" : "amplification"
                }
            }
            Item {
               Layout.fillWidth: true
            }
            ImageButton {
                imageSrc: "qrc:/res/clean.png"
                backHoverColor:"#20334455"
                ToolTip.delay: 700              // 徘徊在按钮上超过500ms则显示,则默认为直接显示
                ToolTip.visible: hovered       // 当鼠标徘徊在按钮上时,则显示ToolTip
                ToolTip.text: qsTr("清空数据")
                Layout.preferredWidth: 21
                Layout.preferredHeight: 21
                Layout.alignment : Qt.AlignVCenter | Qt.AlignLeft
                onClicked: Model.cleanChatData()
                focusPolicy: Qt.NoFocus
            }
        }
        ScrollView {
              id: scrollview
              Layout.fillWidth: true
              Layout.fillHeight: true
              ScrollBar.vertical.policy: ScrollBar.vertical.size<1.0? ScrollBar.AlwaysOn : ScrollBar.AlwaysOff
              GifText {
                  id: area
                  textFormat: TextArea.MarkdownText
                  placeholderText: qsTr("按回车发送,按CTRL+回车换行")
                  Keys.onPressed: {
                      if ((event.key == Qt.Key_Return || event.key == Qt.Key_Enter ) && (event.modifiers & Qt.ControlModifier)) {
                          area.insert(area.cursorPosition, "\r\n")
                          console.log("ENter",area.text)
                          event.accepted = true

                      } else if ((event.key == Qt.Key_Return || event.key == Qt.Key_Enter ) ) {
                          event.accepted = true
                          sendMsg()
                      }
                  }

                  function inserEmoji(file) {
                      console.log(file)
                      area.insertGif(file)
                  }

                  function sendMsg() {
                      if (area.length > 300) {
                          popup.open();
                      } else {
                          Model.sendMsg(area.coding());
                          area.text = ""
                      }
                  }
              }
        }
    }

    height: 150
    states: State {
        name: "amplification"
        PropertyChanges { target: bottom; height: 300 }
    }

    transitions: Transition {
        NumberAnimation { properties: "height";  }
    }

    BaseButton {
        id: _login
        btnText: "发送(S)"
        fontSize: 13
        width: 78
        height: 24
        x: bottom.width - width - 20
        y: bottom.height  - height - 10
        backRadius: 3
        bckcolor: SkinSingleton.skins[AppCfg.skinIndex].popupColor
        ToolTip.delay: 1000             // 徘徊在按钮上超过500ms则显示,则默认为直接显示
        ToolTip.visible: hovered       // 当鼠标徘徊在按钮上时,则显示ToolTip
        ToolTip.text: qsTr("按回车发送,按CTRL+回车换行")
        onClicked: area.sendMsg();
        focusPolicy: Qt.NoFocus
    }

    Rectangle {
      height: 1
      color: "#25777777"
      anchors.top: parent.top
      width: parent.width
    }


    EmojiPopup {
       id: emoji_popup
       horizontalPosBase: EmojiPopup.PosBaseToHorizontalLeft
       verticalPosBase: EmojiPopup.PosBaseToTop

       onSelectEmoji: area.inserEmoji(emojiFile)
    }


    HintPopup {
        id: popup
        width : 300
        height: 95
        parent: Overlay.overlay
        x: Math.round((parent.width - width) / 2)
        y: Math.round((parent.height - height) / 2)
        hintColor: SkinSingleton.skins[AppCfg.skinIndex].popupColor
        hint: "发送消息内容超长，请分条发送!"
        modal: true
        dim: false
        bckRadius: 1
    }

}
