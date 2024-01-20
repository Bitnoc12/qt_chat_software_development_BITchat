import QtQuick 2.12
import QtQuick.Window 2.12
import QtGraphicalEffects 1.12
import Qt.Singleton 1.0
import QtQuick.Controls 2.5
import "qrc:/common" as Common
import Qt.Window 1.0
import Qt.labs.platform 1.1
Frameless {
    id: rootWindow
    visible: true
    property size loginSize: Qt.size(260, 390)
    property size mainSize: Qt.size(1200, 750)
    color: "transparent"
    property var loginMoveArea: [
            Qt.rect(0, 0, loginSize.width, loginSize.height)
    ]

    property var mainMoveArea: [
            Qt.rect(0, 0, 60, 99999),     // 左侧
            Qt.rect(0, 0, 99999, 60)      // 顶部
    ]
    width: flipable.side == Flipable.Front ? loginSize.width : mainSize.width
    height: flipable.side == Flipable.Front ? loginSize.height : mainSize.height
    minimumWidth: flipable.side == Flipable.Front ? loginSize.width : 900
    minimumHeight: flipable.side == Flipable.Front ? loginSize.height : 550
    resizable: flipable.side == Flipable.Front ? false : true
    moveArea:  flipable.side == Flipable.Front ? loginMoveArea : mainMoveArea

    title: qsTr("微信局域网聊天")

    onResizeUpdate: if (flipable.side != Flipable.Front) mainSize = size

    Component.onCompleted: {
        x = Screen.desktopAvailableWidth/2 - width/2
        y = Screen.desktopAvailableHeight/2 - height/2

    }

    Flipable {
          id: flipable
          anchors.fill: parent
          anchors.margins: rootWindow.maximized ? 0 : 8

          front: Login {
              id: login
              anchors.fill: parent
              onCloseRequest: closeFunc();
              onLoginRequest: {
                  loginAnimation.start();
              }
          }
          back: MainWindow {
              id: window
              anchors.fill: parent
              onCloseRequest: closeFunc();
              onReturnLoginRequest: {
                  Model.returnLogin();
                  returnAnimation.start();
              }
          }


          transform: Rotation {
              id: rotation
              origin.x: flipable.width/2
              origin.y: flipable.height/2
              axis { x: 0; y: 1; z: 0 }
              angle: 0
          }

          SequentialAnimation {
              id: loginAnimation
              ParallelAnimation{
                  NumberAnimation { target: rotation; property: "angle"; to: 90; duration: 200 ; }
                  NumberAnimation {target: login; property: "scale"; from: 1.0; to: 0.5;  duration: 120 }
              }
              ParallelAnimation {
                  NumberAnimation {target: rootWindow; property: "x";  to: rootWindow.x - (mainSize.width-loginSize.width)/2;  duration: 100 }
                  NumberAnimation {target: rootWindow; property: "y";  to: rootWindow.y - (mainSize.height-loginSize.height)/2;  duration: 100 }
                  NumberAnimation { target: rotation; property: "angle"; to: 180; duration: 200 ; }
                  NumberAnimation {target: window; property: "scale"; from: 0.5; to: 1.0;  duration: 200 }
              }
          }
          SequentialAnimation {
              id: returnAnimation
              ParallelAnimation{
                  NumberAnimation { target: rotation; property: "angle"; to: 90; duration: 200 ; }
                  NumberAnimation {target: window; property: "scale"; from: 1.0; to: 0.5;  duration: 120 }
              }
              ParallelAnimation {
                  NumberAnimation {target: rootWindow; property: "x";  to: rootWindow.x + (mainSize.width-loginSize.width)/2;  duration: 100 }
                  NumberAnimation {target: rootWindow; property: "y";  to: rootWindow.y + (mainSize.height-loginSize.height)/2;  duration: 100 }
                  NumberAnimation { target: rotation; property: "angle"; to: 0; duration: 200 ; }
                  NumberAnimation {target: login; property: "scale"; from: 0.5; to: 1.0;  duration: 200 }
              }

          }

       }

    function closeFunc() {
        close();
    }
}
