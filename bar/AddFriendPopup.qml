import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import Qt.Singleton 1.0

import QtGraphicalEffects 1.12
import "qrc:/common"
Popup {
    id:popup
    width : 330
    height: 180
    parent: Overlay.overlay
    x: Math.round((parent.width - width) / 2)
    y: Math.round((parent.height - height) / 2)
    CloseButton{
        anchors.right: parent.right
        onClicked: close();
    }
       ColumnLayout {
           anchors.fill: parent
           spacing: 6
           anchors.margins: 10


           KaiText {
               text: "添加好友"
               font.pixelSize: 20
               color: SkinSingleton.skins[AppCfg.skinIndex].popupColor
               Layout.alignment: Qt.AlignCenter
           }
           TextField {
               id: _name
               placeholderText : "请输入好友昵称"
               font.family: "Microsoft Yahei"
               font.pixelSize: 18
               horizontalAlignment: TextField.AlignHCenter
               color: "#666"
               selectedTextColor: "#fff"
               background: Rectangle {
                    border.width: 0
                    color: "transparent"
                    Rectangle {
                      height: 1
                      color: SkinSingleton.skins[AppCfg.skinIndex].loginColor
                      anchors.bottom: parent.bottom
                      width: parent.width
                    }
                }
               Layout.fillWidth: false
               Layout.alignment: Qt.AlignCenter
               Layout.preferredWidth: 200
           }
           BaseButton{
               Layout.alignment: Qt.AlignCenter
               btnText: "确认"
               onClicked: {
                   popup.close();
               }
           }
       }

      closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside

      enter: Transition {
            id: _enterAnima
            NumberAnimation { property: "scale"; from: 0.0; to: 1.0; duration: 200; easing.type : Easing.OutBack }
            NumberAnimation { property: "opacity"; from: 0.0; to: 1.0; duration: 300;  }
            ScriptAction {
                script: _content.positionViewAtIndex( AppCfg.skinIndex, GridView.Center);
            }
      }
      exit: Transition {
            id: _exitAnima
            NumberAnimation { property: "opacity"; from: 1.0; to: 0.0; duration: 300; }
      }
}

