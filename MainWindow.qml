import QtQuick 2.12
import QtGraphicalEffects 1.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12
import Qt.Singleton 1.0

import "qrc:/common"
import "qrc:/bar"
import "qrc:/friend"
import "qrc:/chat"
import "qrc:/common/Hint"
import "qrc:/fileContent"
Rectangle {
    id: mainWindow

    signal closeRequest();
    signal returnLoginRequest();

    gradient:  SkinSingleton.skins[AppCfg.skinIndex]
    clip: true


    Message{
        id: messageTip
        z: 9999
        parent: Overlay.overlay
    }

    FileContent{
        id: fileContent

    }

    DownloadFileAnimation {
        id: downloadFileAnimation
        z: 9999
        parent: Overlay.overlay

    }

    SaveFileDialog {
        id: saveFileDialog
        property string  fileUrl: ""
        property var  fileSize: 0
        property var  imageObj: null
        property string  targetName: ""
        property string  addr: ""
        property int  port: 0


        folder: AppCfg.fileOpenUrl
        onAccepted: {
            console.log("选择了:  "+files[0] +" "+ files.length )
            if (files.length > 0) {

                downloadFileAnimation.start();
                FileManager.newDownFileTask(files[0], fileUrl,
                                           fileSize, targetName, addr, port);
            }
        }
    }


    function message(type, message) {
        if(type!=='success'&&type!=='error'&&xstype!=='info'){
            return false
        }
        messageTip.show=true
        messageTip.text=message
        messageTip.type=type

        messageTip.openTimer()  //调用messageTip组件下定时器方法
    }

    function newDownloadFileTask(imageObj, fileName,  fileUrl,
                                 fileSize,   targetName,   addr,   port) {

        if(!Model.friendIsOnline(targetName)) {
            message('error', "好友已下线,无法下载!");
            return;
        }

        downloadFileAnimation.imageObj = imageObj
        downloadFileAnimation.source = fileContent.getIcon(fileName)


        saveFileDialog.fileUrl = fileUrl
        saveFileDialog.fileSize = fileSize
        saveFileDialog.targetName = targetName
        saveFileDialog.addr = addr
        saveFileDialog.port = port
        saveFileDialog.currentFile  = "file:///" + fileName
        saveFileDialog.open();

    }

    Connections {
        target: Model;
        onErrMsg:{
            message('error', msg);
        }
        onSuccessMsg: {
            message('success', msg);
        }
    }

    RowLayout {
        anchors.fill: parent
        spacing: 0
        MenuBar {
            Layout.preferredWidth: 60
            Layout.fillHeight: true
            onReturnRequest: mainWindow.returnLoginRequest()
        }
        FriendList {
            color: Qt.rgba(1,1,1, 0.45 + AppCfg.skinOpacity * 0.45 )
            Layout.preferredWidth: parseInt(mainWindow.width * 0.256)
            Layout.fillHeight: true
        }
        ChatListPage {
            id: rct
            color: Qt.rgba(1,1,1,0.50 + AppCfg.skinOpacity * 0.45)
            Layout.fillWidth: true
            Layout.fillHeight: true
            Row {
                z: 2
                anchors.right: rct.right
                visible: true ? true : false
                MinimizeButton{

                }
                ScaleImageButton {
                    id: scaleInput
                    backHoverColor:"#22999999"
                    normalImage: "qrc:/res/normal.png"
                    maxImage: "qrc:/res/max.png"
                    ToolTip.delay: 1000
                    ToolTip.visible: hovered
                    ToolTip.text: shrink ? "最大化" : "恢复"
                    Layout.preferredWidth: 21
                    Layout.preferredHeight: 21
                    Layout.alignment : Qt.AlignVCenter | Qt.AlignLeft
                    focusPolicy: Qt.NoFocus
                    onShrinkChanged: rootWindow.maximized = !shrink
                    enabled: true ? true : false
                }
                CloseButton{
                        onClicked: closeRequest();
                }
            }
        }
    }

    layer.enabled: true
    layer.effect: DropShadow {

    }

}
