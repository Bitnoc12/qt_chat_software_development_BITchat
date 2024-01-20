QT += quick sql
QT += gui
QT += core
QT += qml
QT += widgets
QT += network
CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Refer to the documentation for the
# deprecated API to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
include(./common/AppCfg/appcfg.pri)
include(./common/Frameless/frameless.pri)
include(./common/DataBase/database.pri)
#include(./common/WindowMove/windowmove.pri)
# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0



SOURCES += \
        fileManager.cpp \
        fileTcpHandler/fileclient.cpp \
        fileTcpHandler/fileserver.cpp \
        giftexthandler.cpp \
        friend_model.cpp \
        friendchat_data.cpp \
        friendchat_model.cpp \
        main.cpp \
        udpthread.cpp

RESOURCES += qml.qrc \
    res.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES +=

#win32 {
#    RC_FILE += Icon.rc
#}

#win64 {
#    RC_FILE += Icon.rc
#}


RC_FILE = back.rc

HEADERS += \
    fileManager.h \
    fileTcpHandler/fileTask.h \
    fileTcpHandler/fileclient.h \
    fileTcpHandler/fileserver.h \
    giftexthandler.h \
    friend_model.h \
    friendchat_data.h \
    friendchat_model.h \
    udpthread.h
