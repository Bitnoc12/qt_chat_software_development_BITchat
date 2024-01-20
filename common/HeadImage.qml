import QtQuick 2.12
import QtGraphicalEffects 1.12

Rectangle {
    id: container
    property var headUrl : ""
    property bool antialiasingValue : true
    //radius: 5

    width: 100
    height: 100
    radius: width/2
    color: "white"

    Image {
        id: _image

        smooth: true

        anchors.fill:parent
        antialiasing: antialiasingValue
        fillMode: Image.PreserveAspectFit
        source: headUrl
        visible: false
        cache: false             // 取消缓存,避免文件内容改变后,无法刷新图片
    }
    Rectangle {
            id: _mask
            color: "black"
            anchors.fill: parent
            radius: container.radius
            visible: false
            antialiasing: true
            smooth: true

    }
    OpacityMask {
        id:mask_image

        activeFocusOnTab: false
        cached: false
        invert: false

        anchors.fill: container
        source: _image
        maskSource: _mask        // 将source使用icon的规则在icon上显示
        visible: true
        antialiasing: true
    }

}
