import QtQuick 2.0

Item {
    function padding(size) {
        return Math.floor(size * 100) / 100;
    }
    function getFileSize(size) {
        if (size < 0) return '--';
        if (size == 0) return '0B';
        if (size < 1024) return padding(size) + 'B';
        size /= 1024;
        if (size < 1024) return padding(size) + 'KB';
        size /= 1024;
        if (size < 1024) return padding(size) + 'MB';
        size /= 1024;
        return padding(size) + 'GB';
    }

    function getSpeedSize(speed) {
        if (speed <= 1) return '0B/s';
        if (speed < 1024) return padding(speed) + 'B/s';
        speed /= 1024;
        if (speed < 1024) return padding(speed) + 'KB/s';
        speed /= 1024;
        if (speed < 1024) return padding(speed) + 'MB/s';
        speed /= 1024;
        return padding(speed) + 'GB/s';
    }


    function getExt(file) {
        var arr = file.split('.');
        return arr[arr.length - 1].toLowerCase();
    }
    function getIcon(file) {
        var ext = getExt(file);
        var type = {
            '安装包': ['exe', 'apk', 'msi'],
            '代码': ['php', 'js', 'css', 'py', 'php', 'java', 'xml', 'json', 'md', "qml"],
            '视频': ['mp4', 'flv', 'avi', 'mkv'],
            '图片': ['jpg', 'jpeg', 'png', 'bmp', 'gif'],
            '音频': ['mp3', 'wav', 'ogg'],
            CAD: ['cad'],
            H: ['html', 'htm'],
            map: ['map'],
            flash: ['swf'],
            pdf: ['pdf'],
            ipa: ['ipa'],
            ppt: ['ppt', 'pptx'],
            psd: ['psd'],
            txt: ['txt'],
            word: ['doc', 'docx'],
            excel: ['xls', 'xlsx'],
            zip: ['zip', 'rar', '7zip'],
        };
        var keys = Object.keys(type);
        for (var i = 0; i < keys.length; ++i) {
            var key = keys[i];
            if (type[key].indexOf(ext) !== -1) {
                return "qrc:/fileContent/" +key + '.png';
            }
        }
        return "qrc:/fileContent/未知.png";
    }

}
