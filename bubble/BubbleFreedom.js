//.import Qt.labs.colorfunctions 1.0
.import Qt.Singleton 1.0 as SING
var BubbleTailWitdh = 10        // 气泡尾巴
var BubbleRadius = 6
var BubbleTailHeight = 3

function drawBubble(context) {

    // 生成一个随机的颜色
    var h = Math.random(); // 色相
    var s = Math.random(); // 饱和度
    var l = Math.random() * 0.8; // 亮度
    var a = 1.0; // 透明度
    var colorRandom = hsla(h, s, l, a); // 创建颜色对象

    var bubbleTailY = container.height/2

    context.clearRect(0,0,container.width, container.height)
    context.beginPath()
    context.roundedRect(isSend ? 0 : BubbleTailWitdh, 0,
                        container.width - BubbleTailWitdh, container.height,
                        BubbleRadius, BubbleRadius);

    context.moveTo(isSend ? container.width - BubbleTailWitdh : BubbleTailWitdh, bubbleTailY - BubbleTailHeight);
    context.lineTo(isSend ? container.width : 0, bubbleTailY );
    context.lineTo(isSend ? container.width - BubbleTailWitdh : BubbleTailWitdh, bubbleTailY + BubbleTailHeight);
    context.closePath()


    context.fillStyle = isSend ? colorRandom  : "#22445566"
    context.fill()
}
//SING.SkinSingleton.skins[SING.AppCfg.skinIndex].popupColor
