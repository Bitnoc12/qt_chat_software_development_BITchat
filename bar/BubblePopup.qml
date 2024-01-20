import QtQuick 2.12
import QtQuick.Controls 2.12
import Qt.Singleton 1.0
import "qrc:/common"
IndicatorPopup {
       id: bubble_popup
       width : 100
       height: 100
       modal: true
       dim: false
       focus: true
       parent: Overlay.overlay
       backgroundColor: "#FFF"
       horizontalPosBase : IndicatorPopup.PosBaseToRight
       verticalPosBase : IndicatorPopup.TopAlign
       backgroundRadius: 4
       targetSpacing: -4
       SwipeView {
            id: view
            anchors.fill: parent
            clip: true
            currentIndex: AppCfg.bubbleIndex
            Rectangle {
                Text {
                    anchors.centerIn: parent
                    text: "默认气泡"
                    font.pixelSize: 15
                    font.family: "Microsoft Yahei"
                    color: SkinSingleton.skins[AppCfg.skinIndex].popupColor
                }
            }
            Rectangle {
                Text {
                    anchors.centerIn: parent
                    text: "蓝色气泡"
                    font.pixelSize: 15
                    font.family: "Microsoft Yahei"
                    color: "#12B7F5"
                }
            }
            Rectangle {
                Text {
                    anchors.centerIn: parent
                    text: "黑色气泡"
                    font.pixelSize: 15
                    font.family: "Microsoft Yahei"
                    color: "#535353"
                }
            }
            Rectangle {
                Text {
                    anchors.centerIn: parent
                    text: "粉红气泡"
                    font.pixelSize: 15
                    font.family: "Microsoft Yahei"
                    color: "#F5A4FF"
                }
            }
            Rectangle {
                Text {
                    anchors.centerIn: parent
                    text: "随机气泡"
                    font.pixelSize: 15
                    font.family: "Microsoft Yahei"
                    color: "#000000"
                }
            }
            onCurrentIndexChanged: {
                AppCfg.bubbleIndex = currentIndex;
            }
       }
       PageIndicator {
         count: view.count
         currentIndex: view.currentIndex
         anchors.horizontalCenter: parent.horizontalCenter
         anchors.bottom: parent.bottom
      }

      closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside
}
