import QtQuick 2.12
import Qt.labs.folderlistmodel 2.12
import QtQuick.Layouts 1.12
import Qt.Singleton 1.0
import "qrc:/common"
Rectangle {
    id: container
    ColumnLayout {
        width: parent.width
        height: parent.height
        FriendTopBar {
            Layout.fillWidth: true
            friendCnt: _view.count - 1
            z: 2
        }
        ListView {
            id: _view
            Layout.fillHeight: true
            Layout.fillWidth: true
            clip: true
            delegate: FriendDelegate {
                onClicked: Model.enterChat(name);
            }
            model: Model
            currentIndex: -1


            add: Transition {
                      NumberAnimation { properties: "scale";  from: 0; to: 1.0; duration: 90 }
            }
            remove: Transition {
                      NumberAnimation { properties: "scale";  from: 1.0; to: 0.0; duration: 90 }
            }
            displaced: Transition {
                      NumberAnimation { properties: "x,y"; duration: 100 }
            }
        }
    }

    Connections {
        target: Model;
        onInitialize: {
            _view.currentIndex = -1
        }
    }

}
