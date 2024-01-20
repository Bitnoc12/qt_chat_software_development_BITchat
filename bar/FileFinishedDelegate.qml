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

    color: area.containsMouse || _delete.containsMouse || _pos.containsMouse ? "#20888888" : "transparent"

    signal requestDelete();
    signal requestPos();
    signal requestOpen();

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

        Image {
            id: _image
            fillMode: Image.PreserveAspectFit
            source: fileContent.getIcon(file)
            width: 30
            height: width

        }
        ColumnLayout {
            Layout.fillHeight: true
            Layout.fillWidth: true
            RowLayout {
                YaheiText {
                    text: file
                    font.pixelSize: 13
                    color: "#333"
                    elide: Text.ElideMiddle
                    clip: true
                    Layout.fillWidth: true
                    Layout.preferredWidth: 250
                }

                RectButton {
                    id: _delete
                    btnText: "清理"
                    fontSize: 11
                    Layout.preferredWidth: 44
                    Layout.preferredHeight: 18
                    backRadius: 3
                    z: 3
                    bckcolor: SkinSingleton.skins[AppCfg.skinIndex].popupColor

                    onClicked: {
                      requestDelete();
                    }
                }

                RectButton {
                    id: _pos
                    btnText: "定位"
                    fontSize: 11
                    Layout.preferredWidth: 44
                    Layout.preferredHeight: 18
                    backRadius: 3
                    z: 3
                    bckcolor: SkinSingleton.skins[AppCfg.skinIndex].popupColor
                    onClicked: {
                      requestPos();
                    }
                }

                RectButton {
                    id: _open
                    btnText: "打开"
                    fontSize: 11
                    Layout.preferredWidth: 44
                    Layout.preferredHeight: 18
                    backRadius: 3
                    z: 3
                    bckcolor: SkinSingleton.skins[AppCfg.skinIndex].popupColor
                    onClicked: {
                      requestOpen();
                    }
                }

            }

            RowLayout {
                Layout.fillWidth: true
                YaheiText {
                    text: fileContent.getFileSize(fileSize)
                    font.pixelSize: 11
                    color: "#777"
                }
                YaheiText {
                    Layout.preferredWidth: 128
                    text: "(好友:"+ tagetName +")"
                    font.pixelSize: 11
                    color: SkinSingleton.skins[AppCfg.skinIndex].popupColor
                    elide: Text.ElideRight
                    clip: true
                }

                YaheiText {
                    Layout.fillWidth: true
                    text: date
                    font.pixelSize: 11
                    color: "#777"
                }

                YaheiText {
                    text: type
                    font.pixelSize: 12
                    color: SkinSingleton.skins[AppCfg.skinIndex].popupColor
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
