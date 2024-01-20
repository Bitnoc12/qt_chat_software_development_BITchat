import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import Qt.Singleton 1.0
import Qt.ChatModel 1.0
import "qrc:/common"
Item {
    id : chatlist
    property variant pointer : null
    ColumnLayout {
        width: parent.width
        height: parent.height - bottom.height
        ChatTopBar {
            id: top
            Layout.fillWidth: true
            z: 2
            title: pointer == null ? "" : pointer.title()
        }
        ListView {
            id: chatView
            Layout.fillHeight: true
            Layout.fillWidth: true
            delegate: ChatDelegate {
                isChatRoom: pointer.isChatRoom()
            }
            model: pointer
            onCountChanged: {
                chatView.positionViewAtEnd();
                moveEndPosTime.restart();
            }
            clip: true
            ScrollBar.vertical: ScrollBar {
               policy: size<1.0? ScrollBar.AlwaysOn : ScrollBar.AlwaysOff
            }
        }
    }
    ChatInputBottomBar {
        id: bottom
        anchors.bottom: parent.bottom
        width: parent.width

        onStateChanged: moveEndPosTime.restart();
    }

    Timer {
        id: moveEndPosTime
        interval: 300
        onTriggered: {
            chatView.positionViewAtEnd();
        }
    }
    function close() {
        if (pointer != null) {
            Model.closeChat(pointer)
        }
        chatlist.destroy()
    }

}
