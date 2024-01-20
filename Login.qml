import QtQuick 2.12
import QtGraphicalEffects 1.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12
import Qt.Singleton 1.0
import "qrc:/common"
Rectangle {
    id: container
    signal closeRequest();
    signal loginRequest();

    //color: "#F5F5F5"
    //Image {
    //            anchors.fill: parent
    //            source:"qrc:/res/qq.jpg"
    //            fillMode: Image.PreserveAspectCrop  // 根据需要设置适应方式
    //        }//自定义背景图片
    gradient: Gradient {
        GradientStop {
            position: 0
            color: "#66cdfc"
        }

        GradientStop {
            position: 0.205
            color: "#67c4dc"
        }

        GradientStop {
            position: 1
            color: "#ffffff"
        }

    }


    property color color2: "#F5F5F5"



    Component.onCompleted: {
    }


    HintPopup {
        id: popup
        width : 180
        height: 120
        anchors.centerIn: container
        hint: "用户名已被注册!"
    }
    SelectFileDialog {
        id: _file
        nameFilters: ["image files (*.png *.jpg)"]
        folder: AppCfg.fileOpenUrl
        onAccepted: {
            console.log("选择了:  "+files[0])
            if (!AppCfg.updateHeadImage(files[0])) {
                popup.hint = "图片格式读取失败!"
                popup.open();

            } else {

            }
        }
    }

    KaiText {
        text: "BITchat"
        font.pixelSize: 15
        x: 10
        y: 6
        color: "#8E8E8E"
        visible: true ? true : false
    }

    Row {
        z: 2
        anchors.right: container.right
        visible: true ? true : false
        MinimizeButton{
        }
        CloseButton{
            onClicked: closeRequest();
        }
    }

    layer.enabled: true ? true : false
    layer.effect: DropShadow {

    }


    ColumnLayout {
        id: _center
        spacing: 20
        anchors.centerIn: parent
        opacity:  true ? 0.0 : 1.0
        HeadImage {
            id: _head
            radius: width/2
            width : 100
            height : 100
            headUrl: AppCfg.headImage
            Layout.alignment: Qt.AlignCenter
            Button {
                id: _headBtn
                anchors.fill: parent
                contentItem: Rectangle {
                    color: "transparent"
                }
                background: Rectangle {
                    id: _headBtnBack
                    color: "transparent"
                    border.width:  2
                    border.color: _headBtn.hovered||_loggingAnimation.running ? SkinSingleton.skins[AppCfg.skinIndex].loginColor :  "transparent"
                    radius: _head.radius
                }
                onClicked: {
                    _file.open();
                }
                ToolTip.delay: 1000
                ToolTip.visible: hovered
                ToolTip.text: qsTr("点击更换图片")
            }

        }
        TextField {
            id: _name
            placeholderText : "请输入昵称"
            text: AppCfg.currentUser
            font.family: "Microsoft Yahei"
            font.pixelSize: 18
            horizontalAlignment: TextField.AlignHCenter
            color: "#666"
            selectedTextColor: "#fff"
            background: Rectangle {
                border.width: 0
                border.color: "red"
                radius: 4
                color: "#FFFFFF"
                opacity: 0.3
                implicitHeight: 40
                implicitWidth: 280
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
            onTextChanged: {
                AppCfg.currentUser = _name.text;
            }
        }
        TextField {
            id: _pwd
            placeholderText : "请输入密码"
            text: ""
            font.family: "Microsoft Yahei"
            font.pixelSize: 18
            Layout.topMargin: -20
            horizontalAlignment: TextField.AlignHCenter
            echoMode: TextInput.Password
            color: "#666"
            selectedTextColor: "#fff"
            background: Rectangle {
                border.width: 0
                border.color: "red"
                radius: 4
                color: "#FFFFFF"
                opacity: 0.3
                implicitHeight: 40
                implicitWidth: 280
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
            Keys.onPressed: {
                if ((event.key == Qt.Key_Return || event.key == Qt.Key_Enter ) ) {
                      _loggingAnimation.restart();
                      event.accepted = true;
                }
            }
        }

        BaseButton {
            id: _login
            btnText: "登录/注册"
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignCenter
            bckcolor: SkinSingleton.skins[AppCfg.skinIndex].loginColor
            onClicked: {
                _loggingAnimation.restart();

            }
        }
        YaheiText {
            id: _loginHint
            Layout.alignment: Qt.AlignCenter
            text: ""
            font.pixelSize: 15
            color: SkinSingleton.skins[AppCfg.skinIndex].loginColor
        }

    }

    KaiText {
        anchors.horizontalCenter: container.horizontalCenter
        anchors.bottom: container.bottom
        anchors.bottomMargin: 10
        text: " Tip: 用户名唯一  \n"+"若未注册将自动注册"
        font.pixelSize: 13
        color: "#CCCCCC"
    }



    SequentialAnimation {
      running: true ? true : false
      NumberAnimation { target: container;
          properties: "scale"; from: 0.3; to: 1.0; easing.type: Easing.InOutQuad; duration: 200 }
      NumberAnimation { target: _center;
          properties: "opacity"; from: 0; to: 1.0; easing.type: Easing.InOutQuad; duration: 300 }
    }



    SequentialAnimation {
        id: _loggingAnimation
        loops: Animation.Infinite
        ScriptAction {
            script: _loginHint.text = "登录中."
        }
        NumberAnimation {
            target: _headBtnBack;
            property: "opacity";
            from: 1.0; to: 0.0; duration: 200
        }
        ScriptAction {
            script: _loginHint.text = "登录中.."
        }
        NumberAnimation {
            target: _headBtnBack;
            property: "opacity";
            from: 0.0; to: 1.0; duration: 200
        }
        ScriptAction {
            script: {
                _loginHint.text = "登录中..."
                var retStr = Model.login(_name.text, _pwd.text);
                if(retStr !== "") {
                    _loggingAnimation.stop();
                    _loginHint.text = retStr
                    popup.hint =retStr
                    popup.open();
                }
            }
        }
        PauseAnimation { duration: 100 }

    }

    SequentialAnimation {
        id: _loginOkAnimation

        ScriptAction {
            script: {
                _loginHint.text = "登录成功!"
                dataBase.insertLog(_name.text + "登录上线");
            }
        }
        PauseAnimation { duration: 1000 }
        ScriptAction {
            script: {
                loginRequest();
            }
        }

    }

    Connections {
        target: Model;
        onLoginResult:{
            console.log("RESULT",result);
            if (result) {
                _loggingAnimation.stop();
                _loginHint.text = "用户名已被注册!"
                popup.hint = "用户名已被注册!"
                popup.open();
            } else {
                _loggingAnimation.stop();
                _loginOkAnimation.start();
            }
        }
        onInitialize: {
            _loginHint.text = "";
            _headBtnBack.opacity = 1.0;
        }
    }


}
