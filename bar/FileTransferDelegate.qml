
import QtQuick 2.12
import QtQml 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import Qt.Singleton 1.0
import "qrc:/common"
Rectangle {
    id: chatDelegate
    property var isChatRoom: false
    width: ListView.view.width
    height: 80

    color: area.containsMouse || _cancel.containsMouse ? "#20888888" : "transparent"


    MouseArea {
        id: area
        anchors.fill: parent
        hoverEnabled: true

    }

    RowLayout {
        anchors.fill: parent
        spacing: 5
        anchors.leftMargin: 10
        anchors.rightMargin: 10

        Image   {
            id: _image
            fillMode: Image.PreserveAspectFit
            source: fileContent.getIcon(fileName)
            width: 30
            height: width

        }
        ColumnLayout {
            Layout.fillHeight: true
            Layout.fillWidth: true
            RowLayout {
                YaheiText {
                    text: fileName
                    font.pixelSize: 13
                    color: "#333"
                    elide: Text.ElideMiddle
                    clip: true
                    Layout.fillWidth: true
                    Layout.preferredWidth: 250
                }


                RectButton {
                    id: _cancel
                    btnText: "移除"
                    fontSize: 11
                    Layout.preferredWidth: 44
                    Layout.preferredHeight: 18
                    backRadius: 3
                    z: 3
                    bckcolor: SkinSingleton.skins[AppCfg.skinIndex].popupColor
                    onClicked: {
                        FileManager.removeUnderwayTask(id, fileUrl);
                    }
                }

            }

            RowLayout {
                Layout.fillWidth: true
                YaheiText {
                    text: fileContent.getFileSize(currentByte) +"/"+fileContent.getFileSize(fileSize)
                    font.pixelSize: 11
                    color: "#777"
                }

                YaheiText {
                    Layout.fillWidth: true
                    text: "("+ tagetName +")"
                    font.pixelSize: 11
                    color: SkinSingleton.skins[AppCfg.skinIndex].popupColor
                    elide: Text.ElideRight
                    clip: true
                }

                YaheiText {
                    Layout.alignment: Qt.AlignRight
                    text: status
                    font.pixelSize: 11
                    color: status.indexOf("ERR")>=0 ? "#F00":"#777"
                }

                YaheiText {
                    Layout.alignment:  Qt.AlignRight
                    Layout.preferredWidth: 75
                    horizontalAlignment: Text.AlignRight
                    visible: text.length > 0
                    text: status.indexOf("正在")>=0 ? fileContent.getSpeedSize(speed) : ""
                    font.pixelSize: 11
                    color: status.indexOf("ERR")>=0 ? "#F00":"#777"
                }

            }

            ProgressBar {
                id: control
                value: currentByte/fileSize
                Layout.fillWidth: true
                Layout.preferredHeight: 6

                background: Rectangle {
                  implicitWidth: 200
                  implicitHeight: 6
                  color: "#e6e6e6"
                  radius: 3
                }

                contentItem: Item {
                  implicitWidth: 200
                  implicitHeight: 6

                  Rectangle {
                      width: control.visualPosition * parent.width
                      height: parent.height
                      radius: 2
                      gradient: SkinSingleton.skins[AppCfg.skinIndex]
                  }
                }
            }


        }
        Rectangle {
            height: 1
            color: "#15777777"
            Layout.fillWidth: true
        }

    }


}
