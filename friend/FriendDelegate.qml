import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import QtQml 2.12
import Qt.Singleton 1.0
import "qrc:/common"
Rectangle {
    id: container
    property int headSize : 40
    signal clicked()
    height: headSize + 30
    width: ListView.view.width
    color: container.ListView.view.currentIndex == index ? "#22667788" : "transparent";
    clip: true
    RowLayout {
        id: layout
        anchors.fill: parent
        spacing: 10
        anchors.margins: 15
        HeadImage {
            id: icon
            Layout.preferredWidth : headSize
            Layout.preferredHeight : headSize
            headUrl: head == null ? "" : head
            antialiasingValue: false
        }
        ColumnLayout {
            id: _column
            spacing: 5
            Layout.fillWidth : true
            Layout.preferredHeight : headSize
            RowLayout {
                Layout.fillWidth : true
                Text {
                    id: _name
                    text: name == null ? null : name
                    color: "#000000"
                    font.pixelSize:  Qt.platform.os == "windows" ? 16 : 14
                    font.family: "Microsoft Yahei"
                    Layout.fillWidth : true
                    Layout.fillHeight: true
                    Layout.preferredHeight: font.pixelSize +6
                    verticalAlignment: Text.AlignBottom
                    elide: Text.ElideRight
                    clip: true
                }
                Text {
                    id: _recodeDate
                    text: getRecentDate()
                    color: "#6F6F6F"
                    font.pixelSize: 12
                    font.family: "Microsoft Yahei"
                    horizontalAlignment: Text.AlignRight
                    Layout.alignment: Qt.AlignRight
                    Layout.preferredWidth: 50
                    elide: Text.ElideRight
                }
            }
            Text {
                id: _recode
                text: recentMsg
                color: "#6F6F6F"
                font.pixelSize: 12
                font.family: "Microsoft Yahei"
                wrapMode: Text.NoWrap
                verticalAlignment: Text.AlignTop
                Layout.fillWidth : true
                Layout.preferredHeight: font.pixelSize +6
                elide: Text.ElideRight
                textFormat: Text.PlainText
            }
        }
    }

    Rectangle {
        id: _unreadCnt
        color: "#FA5151"
        width: 17
        height: 17
        radius: 8
        x: 15 + 40 - radius - 2
        y: 15 - radius + 2
        visible: unreadCnt != 0
        YaheiText {
            id: _unreadCntText
            text: unreadCnt > 99 ? "..." : unreadCnt
            anchors.centerIn: parent
            font.pixelSize: 12
            color: "#FFF"
        }
    }

    function getRecentDate() {
        var currentDate = new Date();
        var date =  new Date(recentMsgDate);
        var total = currentDate-date;

        if (total < (1000*60*60*24)) {
            return date.toLocaleTimeString(Qt.locale(), "hh:mm");
        } else if (total < (1000*60*60*24) * 2) {
            return "昨天";
        } else if (total < (1000*60*60*24) * 3) {
            return "前天";
        } else {
            return date.toLocaleDateString(Qt.locale(), "MM/dd");
        }
    }


    Connections {
        target: Model
        onDataChanged: {
            if (rowId == index) {
                _recode.text = recentMsg
                _recodeDate.text = getRecentDate()
                _unreadCnt.visible = unreadCnt != 0
                _unreadCntText.text =  unreadCnt > 99 ? "..." : unreadCnt
            }
        }
    }


    Rectangle {
        height: 1
        color: "#33777777"
        anchors.bottom: parent.bottom
        x:   headSize + 20
        width: parent.width - x
    }
    MouseArea {
        anchors.fill: parent
        onClicked: {
            container.clicked();
            container.ListView.view.currentIndex = index;
        }
    }


}
