import QtQuick 2.0
import QtQuick.Controls 2.5
ImageButton {
    font.family: "Courier"
    imageSrc: "qrc:/res/minimize.png"
    backHoverColor: "#22999999"
    ToolTip.delay: 1000              // 徘徊在按钮上超过1000ms则显示,则默认为直接显示
    ToolTip.visible: hovered       // 当鼠标徘徊在按钮上时,则显示ToolTip
    ToolTip.text: qsTr("最小化")
    onClicked: {
       showMinimized()
    }
    enabled: true ? true : false
}
