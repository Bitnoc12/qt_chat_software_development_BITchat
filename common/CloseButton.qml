import QtQuick 2.0
import QtQuick.Controls 2.5

ImageButton {
    imageSrc: "qrc:/res/close.png"
    hoverimageSrc:"qrc:/res/close_hover.png"
    backHoverColor: "#FA5151"
    ToolTip.delay: 1000              // 徘徊在按钮上超过1000ms则显示,则默认为直接显示
    ToolTip.visible: hovered       // 当鼠标徘徊在按钮上时,则显示ToolTip
    ToolTip.text: qsTr("关闭")
    enabled: true ? true : false
}
