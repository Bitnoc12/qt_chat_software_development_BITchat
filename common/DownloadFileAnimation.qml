import QtQuick 2.12

Image {
    id: _image

    property var imageObj: null
    property var transferIcnObj: null
    property point transferIcnPos: Qt.point(0, 0)
    property point imagePos: Qt.point(0, 0)

    fillMode: Image.PreserveAspectFit
    width: 40
    height: width
    visible: numberAnimationX.running

    onTransferIcnObjChanged: {
        let targetObject = getGlobalPos(transferIcnObj)
        transferIcnPos = Qt.point(targetObject.x + targetObject.width/2,
                                  targetObject.y + targetObject.height/2)
       // console.log("********onTransferIcnObjChanged:",transferIcnPos)
    }

    onImageObjChanged: {
        let targetObject = getGlobalPos(imageObj)
        imagePos = Qt.point(targetObject.x + targetObject.width/2,
                                  targetObject.y + targetObject.height/2)
      //  console.log("********onImageObjChanged:",imagePos)
    }


    function start() {
        numberAnimationX.start();
    }

    NumberAnimation on x {
        id: numberAnimationX
        duration: 800
        from: imagePos.x -width/2
        to: transferIcnPos.x -width/2
        running: false
    }
    NumberAnimation on y {
        id: numberAnimationY
        duration: 800
        from: imagePos.y -height/2
        to: transferIcnPos.y -height/2
        running: numberAnimationX.running
    }
    NumberAnimation on scale {
        id: numberAnimationScale
        duration: 800
        from: 1.0
        to: 0.7
        running: numberAnimationX.running
    }

    NumberAnimation on rotation {
        id: numberAnimationRotation
        duration: 800
        from: 0
        to: 360
        running: numberAnimationX.running
    }

    NumberAnimation on opacity {
        id: numberAnimationOpacity
        duration: 800
        from: 1.0
        to: 0.7
        running: numberAnimationX.running
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
