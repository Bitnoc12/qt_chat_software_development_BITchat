import QtQuick 2.12
import QtQuick.Controls 2.12
import Qt.GifText 1.0
TextArea {
    id: area
    property int gifWidth: 28
    property int gifHeight: 28
    antialiasing: true

    color: "#0E0E0E"
    font.pixelSize: 14
    font.family: "Microsoft Yahei"
    wrapMode: Text.WrapAnywhere
    textFormat: TextEdit.PlainText
    selectByMouse: true
    selectByKeyboard: true

    GifTextHandler {
        id : textObject
        document: area.textDocument
        cursorStart: area.selectionStart
        cursorEnd: area.selectionEnd
        width: gifWidth
        height: gifHeight
        onNeedUpdate: {
            let alpha = area.color.a;
            area.color.a = alpha - 0.01;
            area.color.a = alpha;
        }
    }

    function insertGif(gif) {
        textObject.inset(gif);
    }

    function coding() {
      //  console.log("send:",area.text);
        return textObject.coding();
    }

    function encoding(text) {
        return textObject.encoding(text)
    }

    function debug() {
        return textObject.debug();  // 调试,打印文本内容
    }

}
