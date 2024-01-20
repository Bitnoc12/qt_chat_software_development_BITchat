import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import Qt.Singleton 1.0
import "qrc:/common"
Rectangle {
    id: chatListPage
    property var currentObj : defaultComp.createObject(chatListPage)
    property var nextObj : null

    clip: true

    Component {
        id: defaultComp
        Item {
            id: page
            width: chatListPage.width
            height: chatListPage.height
            function close() {
                page.destroy()
            }
        }
    }

    Component {
        id: chatListComp
        ChatList {
            id: page
            width: chatListPage.width
            height: chatListPage.height
        }
    }

    function show(chatPoint) {
        nextObj = chatListComp.createObject(chatListPage,{pointer: chatPoint});
        showAnim.start()
    }

    SequentialAnimation {
        id: showAnim
        ScriptAction {
            script: {
                nextObj.x = chatListPage.width;
            }
        }
        NumberAnimation {
            target: currentObj
            properties: "opacity"
            from: 1.0
            to: 0
            duration: 200
        }
        ScriptAction {
            script: {
                currentObj.close();
            }
        }
        NumberAnimation {
            target: nextObj
            property: "x"
            from: chatListPage.width
            to: 0
            duration: 200
        }
        ScriptAction {
            script: {
                currentObj = nextObj;
                nextObj = null;
            }
        }
    }

    Connections {
        target: Model
        onChatChangeStartAnimation: {
            if (showAnim.running) {
                return;
            }
            show(Model.currentChat());
        }
        onRemoveFriendcloseChat: {
            currentObj.destroy();
            currentObj = defaultComp.createObject(chatListPage)
        }
        onInitialize: {
            currentObj.destroy();
            currentObj = defaultComp.createObject(chatListPage)
        }

    }

}
