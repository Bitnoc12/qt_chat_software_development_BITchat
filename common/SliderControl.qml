import QtQuick 2.12
import QtQuick.Controls 2.5
import QtGraphicalEffects 1.12
Slider {
    id: _slider
    padding: 0

    background: Rectangle {
         x: _slider.leftPadding
         y: _slider.topPadding + _slider.availableHeight / 2 - height / 2
         implicitWidth: 120
         implicitHeight: 5
         width: _slider.availableWidth
         height: implicitHeight
         radius: 2
         color: "#3F3F40"
         Rectangle {
             width: _slider.visualPosition * parent.width
             height: parent.height
             gradient: Gradient {
                 orientation: Gradient.Horizontal;
                 GradientStop { position: 0.0; color: "#1A570D" }
                 GradientStop { position: 1.0; color: "#79D434" }
             }
             radius: parent.height / 2
         }
     }

     handle: Rectangle {
         x: _slider.leftPadding + _slider.visualPosition * (_slider.availableWidth - width)
         y: _slider.topPadding + _slider.availableHeight / 2 - height / 2
         implicitWidth: 14
         implicitHeight: 14
         radius: 7
         color: "#FFF"
         layer.enabled: true
         layer.effect: DropShadow {
            color: "#79D434"
         }
         visible: enabled

     }
}
