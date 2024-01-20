import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtGraphicalEffects 1.12
import Qt.Singleton 1.0
import "qrc:/common"
Popup {
  id: _popup

  background: Rectangle {
      implicitWidth: 650
      implicitHeight: 420
      radius: 1
      layer.enabled: true
      layer.effect: DropShadow {
         visible: !_enterAnima.running & !_exitAnima.running
      }

      CloseButton{
          anchors.right: parent.right
          onClicked: close();
      }

      YaheiText {
          text: "☺个性装扮"
          font.pixelSize: 15
          x: 14
          y: 4
          color: SkinSingleton.skins[AppCfg.skinIndex].popupColor
      }
      Rectangle {
          height: 1
          color: "#DEDEDE";
          width: parent.width
          y: 33
      }
      Row {
          spacing: 12
          anchors.bottom: parent.bottom
          anchors.left: parent.left
          anchors.leftMargin: 23
          anchors.bottomMargin: 5
          YaheiText {
              text: "透明度"
              font.pixelSize: 13
              color: SkinSingleton.skins[AppCfg.skinIndex].popupColor
              anchors.verticalCenter: parent.verticalCenter
          }
          BaseSlider {
              id: _slider
              styleColor: SkinSingleton.skins[AppCfg.skinIndex].popupColor
              from: 0.4
              to: 1.0
              value: AppCfg.skinOpacity
              onMoved: {
                AppCfg.skinOpacity = value;
              }
          }
      }
      Row {
          anchors.bottom: parent.bottom
          anchors.right: parent.right
          anchors.rightMargin: 23
          anchors.bottomMargin: 8
          BaseButton {
              btnText: "恢复默认"
              height: 21
              width: 75
              fontSize: 12
              bckcolor: SkinSingleton.skins[AppCfg.skinIndex].popupColor
              anchors.verticalCenter: parent.verticalCenter
              backRadius: 5
              onClicked: {
                    AppCfg.skinIndex = 3
                    AppCfg.skinOpacity = 0.9
              }
          }
      }
  }
  Component {
      id: _contentDelegate
      Item {
          id: block
          width: _content.cellWidth; height: _content.cellHeight
          clip: true
          Column {
              anchors.fill: parent
              spacing: 6
              anchors.topMargin: 10
              Rectangle {
                implicitWidth: block.width - 20
                implicitHeight: implicitWidth
                gradient: SkinSingleton.skins[index]
                anchors.horizontalCenter: parent.horizontalCenter
                radius: 5
              }
              YaheiText {
                  text: name
                  font.pixelSize: 14
                  color: "#676767"
                  anchors.horizontalCenter: parent.horizontalCenter
              }
          }
          MouseArea {
            anchors.fill: parent
            onClicked: {
                console.log("onCurrentIndexChanged",index)
                AppCfg.skinIndex = index;
            }
          }
      }
  }

  GridView {
      id: _content
      anchors.fill: parent
      anchors.topMargin: 30
      anchors.bottomMargin: 30
      model: SkinSingleton.skins
      cellWidth: parent.width/4
      cellHeight: cellWidth + 20
      delegate: _contentDelegate
      clip: true
      currentIndex: AppCfg.skinIndex
      highlight: Rectangle { color: "lightsteelblue";  radius: 5 }
  }

  modal: true
  dim: false
  focus: true

  parent: Overlay.overlay
  x: Math.round((parent.width - width) / 2)
  y: Math.round((parent.height - height) / 2)

  closePolicy: Popup.CloseOnEscape

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
