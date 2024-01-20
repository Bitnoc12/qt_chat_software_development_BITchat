import QtQuick 2.12
import QtQml 2.12
import Qt.ChatModel 1.0
import Qt.ChatModelData 1.0
import QtQuick.Layouts 1.12
import Qt.Singleton 1.0
import "qrc:/common"
import "qrc:/bubble"
Item {
    id: chatDelegate
    property var obj: null
    property var isChatRoom: false
    width: chatListPage.width
    height: obj.height

    state: msgType

    states: [
        State {
              name: FriendChatData.TypeHint
              StateChangeScript {
                  script: {
                     obj = hintComponent.createObject(chatDelegate);
                  }
              }
        },
        State {
              name: FriendChatData.TypeDateHint
              StateChangeScript {
                  script: {
                     obj = hintComponent.createObject(chatDelegate);
                  }
              }
        },
        State {
              name: FriendChatData.TypeRecv
              StateChangeScript {
                  script: {
                     obj = msgComponent.createObject(chatDelegate);
                  }
              }

        },
        State {
              name: FriendChatData.TypeSend
              StateChangeScript {
                  script: {
                      obj = msgComponent.createObject(chatDelegate);
                  }
              }
        },

        State {
              name: FriendChatData.TypeSendFile
              StateChangeScript {
                  script: {
                      obj = fileComponent.createObject(chatDelegate);
                  }
              }
        },
        State {
              name: FriendChatData.TypeRecvFile
              StateChangeScript {
                  script: {
                      obj = fileComponent.createObject(chatDelegate);
                  }
              }
        }

    ]

    Component {
        id: msgComponent
        RowLayout {
            anchors.verticalCenter: chatDelegate.verticalCenter;
            anchors.left: msgType == FriendChatData.TypeSend ? undefined : chatDelegate.left
            anchors.right: msgType == FriendChatData.TypeSend ?  chatDelegate.right : undefined
            anchors.leftMargin: 30
            anchors.rightMargin: 30
            anchors.topMargin: 10
            layoutDirection : msgType == FriendChatData.TypeSend ? Qt.RightToLeft : Qt.LeftToRight
            height: _msg.height+35
            spacing: 5
            HeadImage {
                id: _head
                width : 40
                height : 40
                headUrl: msgType == FriendChatData.TypeSend ? AppCfg.headImageSmall : AppCfg.friendHeadDir+srcUser+".jpg"
                Layout.topMargin: 10
            }
            Column {
                spacing: 3
                YaheiText {
                    visible: isChatRoom && msgType ==  FriendChatData.TypeRecv
                    text: srcUser
                    font.pixelSize: 13
                    width: 220
                    elide: Text.ElideRight
                    height: 18
                    color: "#636363"
                    leftPadding: msgType == FriendChatData.TypeSend ? 0 : 10
                    rightPadding: msgType == FriendChatData.TypeSend ? 10 : 0
                }
                ChatBubble {
                    id: _msg
                    msgText: msg
                    iconHeight: _head.height
                    isSend: msgType == FriendChatData.TypeSend ? true : false
                    maxWidth: chatDelegate.width - _head.width * 2 - 60 -  5

                }
            }

        }
    }

    Component {
        id: fileComponent
        RowLayout {
            anchors.verticalCenter: chatDelegate.verticalCenter;
            anchors.left: msgType == FriendChatData.TypeSendFile ? undefined : chatDelegate.left
            anchors.right: msgType == FriendChatData.TypeSendFile ?  chatDelegate.right : undefined
            anchors.leftMargin: 30
            anchors.rightMargin: 30
            anchors.topMargin: 10
            layoutDirection : msgType == FriendChatData.TypeSendFile ? Qt.RightToLeft : Qt.LeftToRight
            height: _file.height+35
            spacing: 5
            HeadImage {
                id: _head
                width : 40
                height : 40
                headUrl: msgType == FriendChatData.TypeSendFile ? AppCfg.headImageSmall : AppCfg.friendHeadDir+srcUser+".jpg"
                Layout.topMargin: 10
            }
            Column {
                spacing: 3
                YaheiText {
                    visible: isChatRoom
                    text: srcUser
                    font.pixelSize: 13
                    width: 220
                    elide: Text.ElideRight
                    height: 18
                    color: "#636363"
                    leftPadding: msgType == FriendChatData.TypeSend ? 0 : 10
                    rightPadding: msgType == FriendChatData.TypeSend ? 10 : 0
                }
                FileBubble {
                    id: _file
                    height: 95
                    width: 265
                    iconHeight: _head.height
                    isSend: msgType == FriendChatData.TypeSendFile ? true : false
                    fileName: msg
                    size:  fileSize
                    desc: addr +"::" +port.toString()
                    onSave: {
                        newDownloadFileTask(imageObj, msg,   fileLocal, fileSize, srcUser, addr, port);

                    }
                }
            }

        }
    }


    Component {
        id: hintComponent
        YaheiText {
            id: _hint
            text: msgType == FriendChatData.TypeDateHint ? getChatDate() : msg
            color: "#858585"
            font.pixelSize: 14
            elide: Text.ElideRight
            anchors.centerIn: chatDelegate
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            anchors.verticalCenterOffset: -5
            height: 40
        }
    }

    function getChatDate() {
        var msgDate = new Date(date);
        var total = new Date() - msgDate;
        if (total < (1000*60*60*24)) {
            return msgDate.toLocaleTimeString(Qt.locale(), "hh:mm");
        } else if (total < (1000*60*60*24) * 2) {
            return "昨天 "+msgDate.toLocaleTimeString(Qt.locale(), "hh:mm");
        } else if (total < (1000*60*60*24) * 3) {
            return "前天 "+msgDate.toLocaleTimeString(Qt.locale(), "hh:mm");
        } else {
            return msgDate.toLocaleString(Qt.locale(), "yyyy年M月d日 hh:mm");
        }
    }
}
