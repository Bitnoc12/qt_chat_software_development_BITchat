import QtQuick 2.12
import QtQuick.Window 2.12
import Qt.Singleton 1.0
import QtQuick.Controls 2.5
import "qrc:/common" as Common
import Qt.Window 1.0

Window {
    id: rootWindow
    visible: true
    property bool isLogin: true
    property var mainObj: null

    width: 1000
    height: 600
    minimumWidth: 600
    minimumHeight: 600

    title: qsTr("微信局域网聊天")

    Component.onCompleted: {
        x = Screen.desktopAvailableWidth/2 - width/2
        y = Screen.desktopAvailableHeight/2 - height/2

    }
    SwipeView {
        id: swipe
        anchors.fill: parent
        currentIndex: 0
        interactive: false
        Rectangle {
            color: "#F5F5F5"
            Login {
              id: login
              width: 400
              height: 600
              anchors.centerIn: parent
              onCloseRequest: closeFunc();
              onLoginRequest: {
                  swipe.currentIndex = 1
              }
            }
        }
        Item {
            MainWindow {
              id: window
              anchors.fill: parent
              onCloseRequest: closeFunc();
              onReturnLoginRequest: {
                  Model.returnLogin();
                  swipe.currentIndex = 0
              }
            }
        }
       onCurrentIndexChanged: {
           console.log("SwipeView",count,currentIndex)
       }
    }



    function closeFunc() {
        close();
    }

}
