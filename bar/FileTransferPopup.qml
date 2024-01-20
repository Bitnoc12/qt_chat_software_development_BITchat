import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import Qt.Singleton 1.0

import QtGraphicalEffects 1.12
import "qrc:/common"
IndicatorShadowHorizontalPopup {
       id: content
       width : 480
       height: 550
       modal: true
       dim: false
       focus: true
       parent: Overlay.overlay
       backgroundColor: "#FFFFFF"
       horizontalPosBase : IndicatorShadowHorizontalPopup.PosBaseToRight
       backgroundRadius: 4
       targetSpacing: 0

       property var tabModel: [
              ["传输列表",  underwayModel.count],
              ["完成列表",  finishModel.count],
       ]

       ListModel {
            id: underwayModel
       }
       ListModel {
            id: finishModel
       }

       Connections {
           target: FileManager
           onUnderwayCntChanged:{
               refrenshUnderwayModel();
           }
           onFinishDataRefrensh: {
               if(finishModel.count == list.length ) {
                   for(let i = 0; i < list.length; i++) {
                      finishModel.set(i, list[i]);
                   }
               } else {
                   finishModel.clear()
                   for(let i = 0; i < list.length; i++) {
                      finishModel.append(list[i]);
                   }

               }
           }
       }

       Component.onCompleted:  {
          FileManager.requesetFinishDataRerensh();
       }


       function refrenshUnderwayModel() {
           var underwayMap = FileManager.getUnderwayData();
           if(underwayModel.count == underwayMap.length ) {
               for(let i = 0; i < underwayMap.length; i++) {
                  underwayModel.set(i, underwayMap[i]);
               }
           } else {
               underwayModel.clear()
               for(let i = 0; i < underwayMap.length; i++) {
                  underwayModel.append(underwayMap[i]);
               }

           }
       }

       Timer {
         repeat: true
         interval: content.opened ? 500 : 3000
         running: true
         onTriggered: {
            refrenshUnderwayModel();
         }
       }

       ColumnLayout {
           anchors.fill: parent
           spacing: 6
           anchors.margins: 10

           TabBar {
               id: bar
               Layout.preferredWidth: 100 * tabModel.length
               Layout.preferredHeight: 32
               background: Rectangle {
                   color: "transparent"
               }
               currentIndex: 0


               Repeater {
                   id: barBtns
                   model: tabModel.length

                   TabButton {
                       id: menuButton
                       text: qsTr(tabModel[index][0])
                       font.pixelSize: 16
                       font.family: "Microsoft Yahei"
                       focus: true

                       hoverEnabled: true
                       contentItem: Label {
                             id: _tabLabel
                             text: menuButton.text
                             font: menuButton.font

                             verticalAlignment: Text.AlignVCenter
                             horizontalAlignment: Text.AlignHCenter
                             color:  checked || hovered ? SkinSingleton.skins[AppCfg.skinIndex].popupColor  :  "#666"

                             Rectangle {
                                 color: checked || hovered ? SkinSingleton.skins[AppCfg.skinIndex].popupColor  :  "#666"
                                 width: 16
                                 height: 16
                                 radius: 8
                                 x: _tabLabel.width - width/2
                                 y: -width*0.5

                             //    visible: tabModel[index][1] != 0
                                 YaheiText {
                                     text: tabModel[index][1] > 99 ? "..." : tabModel[index][1]
                                     anchors.centerIn: parent
                                     font.pixelSize: tabModel[index][1] > 9 ? 12 : 13
                                     color: "#FFF"
                                 }
                             }
                        }
                        background: Item {
                             implicitWidth:80
                             implicitHeight: 32


                             Rectangle {
                                 height: 2
                                 anchors.bottom: parent.bottom
                                 x:  0
                                 width: parent.width
                                 color: checked  ? SkinSingleton.skins[AppCfg.skinIndex].popupColor : hovered ? "#666" : "transparent"
                                 radius: 1
                             }
                        }
                    }

                 }
            }

           Rectangle {
               height: 1
               color: "#25777777"
               Layout.fillWidth: true
           }

            StackLayout {

                Layout.fillWidth: true
                Layout.fillHeight: true
                id: stack
                currentIndex: bar.currentIndex

                ListView {
                    width: parent.width
                    height: parent.height
                    model: underwayModel
                    delegate: FileTransferDelegate {


                    }
                    clip: true
                    ScrollBar.vertical: ScrollBar {
                       policy: size<1.0? ScrollBar.AlwaysOn : ScrollBar.AlwaysOff
                    }
                }

                ListView {
                    width: parent.width
                    height: parent.height
                    model: finishModel
                    delegate: FileFinishedDelegate {
                        onRequestDelete: {
                            var idTemp = id;
                            finishModel.remove(index);
                            dataBase.deleteFinishFileLog(idTemp);
                        }
                        onRequestPos: {
                            if(!FileManager.openExplorer(local)) {
                              message('error', "目录已不存在,定位失败!");
                            }
                        }

                        onRequestOpen: {
                            if(!FileManager.openFile(local)) {
                              message('error', "文件已不存在,定位失败!");
                            }
                        }

                    }
                    clip: true
                    ScrollBar.vertical: ScrollBar {
                       policy: size<1.0? ScrollBar.AlwaysOn : ScrollBar.AlwaysOff
                    }

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


       }

      closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside
}
