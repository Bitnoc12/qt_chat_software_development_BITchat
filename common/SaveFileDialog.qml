import QtQuick 2.12
import Qt.labs.platform 1.0
FileDialog {
    title: "另存为"
    acceptLabel: "确定"
    rejectLabel: "取消"
    fileMode: FileDialog.SaveFile
    onAccepted: {
        console.log("选中的文件有:")
        for (var i in files) {
            console.log(files[i])
        }
    }
}
