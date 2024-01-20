var BubbleTailWitdh = 10        // 气泡尾巴
var BubbleRadius = 6
var BubbleTailHeight = 3

function drawBubble(context) {

    var bubbleTailY = container.iconHeight/2 + BubbleTailWitdh * 0.6

    context.beginPath()
    context.clearRect(0,0,container.width, container.height)
    context.roundedRect(isSend ? 0 : BubbleTailWitdh, 0,
                        container.width - BubbleTailWitdh, container.height,
                        BubbleRadius, BubbleRadius);
    // 最上 中间 最下
    context.moveTo(isSend ? container.width - BubbleTailWitdh : BubbleTailWitdh, bubbleTailY - BubbleTailHeight*0.5);
    context.quadraticCurveTo(isSend ? container.width - BubbleTailWitdh / 4 : BubbleTailWitdh / 2, bubbleTailY ,
                             isSend ? container.width : 0, container.iconHeight * 0.4 );
    context.quadraticCurveTo(isSend ? container.width - BubbleTailWitdh / 4 : BubbleTailWitdh / 2, bubbleTailY + BubbleTailHeight ,
                             isSend ? container.width - BubbleTailWitdh : BubbleTailWitdh, bubbleTailY + BubbleTailHeight *2 );
    context.closePath()
    context.fillStyle = isSend ?  "#12B7F5" : "#22445566"

    context.fill()
}

