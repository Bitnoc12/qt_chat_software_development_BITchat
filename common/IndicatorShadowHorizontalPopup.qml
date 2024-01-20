
import QtQuick 2.12
import QtQuick.Controls 2.12
import QtGraphicalEffects 1.12
Popup {
     id: popup

     // 设置水平位置
     enum HorizontalPosBase {
         PosBaseToLeft,                 // popup位于目标水平左侧
         PosBaseToRight                 // popup位于目标水平右侧
     }


     property int horizontalPosBase: IndicatorShadowHorizontalPopup.PosBaseToLeft
//     property int verticalPosBase: IndicatorShadowHorizontalPopup.TopAlign

     property int backgroundRadius: 3       // popup圆边框
     property int indicatorLen: 10           // 指示器长度
     property var backgroundColor: "#44D2FF"   // 背景色
     property int targetSpacing: 3          // popup与目标的间隔
     property color shadowsColor : "#33000000"

     implicitWidth: 120
     implicitHeight: 120

     modal: true
     focus: true
     background: Item {
        id: back
        anchors.fill: parent
        Rectangle {
            anchors.leftMargin :horizontalPosBase === IndicatorShadowHorizontalPopup.PosBaseToLeft ? indicatorLen : 0
            anchors.rightMargin : horizontalPosBase === IndicatorShadowHorizontalPopup.PosBaseToLeft ? indicatorLen : 0
            color: backgroundColor
            anchors.fill: parent
            radius: backgroundRadius
            layer.enabled: true
            layer.effect: DropShadow {
                color: shadowsColor
            }
        }

     }

     leftPadding : horizontalPosBase === IndicatorShadowHorizontalPopup.PosBaseToLeft ? indicatorLen : 0
     rightPadding : horizontalPosBase === IndicatorShadowHorizontalPopup.PosBaseToLeft ? indicatorLen : 0


     property var myTarget
     property var myTargetCenterY: 0

     Canvas {
         id: canvas
         parent: back
         width: indicatorLen
         height: indicatorLen
         y: myTargetCenterY -height/2 - popup.y
         x: horizontalPosBase === IndicatorShadowHorizontalPopup.PosBaseToLeft ? parent.width : -canvas.width
         antialiasing: true
         rotation: horizontalPosBase === IndicatorShadowHorizontalPopup.PosBaseToLeft ? 0 : 180
         smooth: true
         onPaint: {
             var ctx = getContext("2d");
             ctx.clearRect(0,0, canvas.width, canvas.height);


             ctx.shadowBlur = 1
             ctx.shadowColor  = shadowsColor
             ctx.fillStyle = backgroundColor;
             ctx.beginPath()


             ctx.moveTo(0,0)
             ctx.lineTo(canvas.width-4, canvas.height/2)
             ctx.lineTo(0, canvas.height)
             ctx.closePath()
             ctx.fill()



         }
         onYChanged: canvas.requestPaint()
    }

     function indicatorOpen(target) {
         myTarget = getGlobalPos(target);
         myTargetCenterY = myTarget.y + myTarget.height/2
         updatePopupPos();
         popup.open();
     }



     function updatePopupPos() {

         switch (horizontalPosBase)
        {
            case IndicatorShadowHorizontalPopup.PosBaseToLeft:     // popup位于目标水平左侧

                x = myTarget.x - width - targetSpacing - indicatorLen;
                y = myTarget.y + myTarget.height/2 - height/2

                break;


            case IndicatorShadowHorizontalPopup.PosBaseToRight:   // popup位于目标水平右侧

                x = myTarget.x + myTarget.width + targetSpacing + indicatorLen;
                y =  myTarget.y + myTarget.height/2 - height/2

                break;
        }

       // console.log("updatePopupPos",x, y, width, height, myTargetCenterY, canvas.x,canvas.y)

     }


     function getGlobalPos(targetObject) {
        var positionX = 0
        var positionY = 0
        var obj = targetObject

        /* 遍历所有的父窗口 */
        while (obj != null) {
            positionX += obj.x
            positionY += obj.y

            obj = obj.parent
        }

        return {"x": positionX, "y": positionY, "width": targetObject.width, "height": targetObject.height}
    }

}
