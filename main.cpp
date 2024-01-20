#include <QGuiApplication>
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "appcfg.h"
#include "friend_model.h"
#include "friendchat_model.h"
#include "friendchat_data.h"
#include "giftexthandler.h"
#include "frameless.h"
#include "database.h"
#include "fileManager.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    // 初始化应用程序
    QApplication app(argc, argv);

    // 创建 QML 应用程序引擎
    QQmlApplicationEngine engine;

    // 为各种组件注册单例类型
    qmlRegisterSingletonType<AppCfg>("Qt.Singleton", 1, 0, "Data", appCfg_qobject_singletontype_provider);
    qmlRegisterSingletonType<FileManager>("Qt.Singleton", 1, 0, "FileManager", fileManager_qobject_provider);
    qmlRegisterSingletonType<AppCfg>("Qt.Singleton", 1, 0, "AppCfg", appCfg_qobject_singletontype_provider);
    qmlRegisterSingletonType<FriendModel>("Qt.Singleton", 1, 0, "Model", FriendModel_qobject_provider);

    // 为各种模型和组件注册 QML 类型
    qmlRegisterType<FriendChatModel>("Qt.ChatModel", 1, 0, "FriendChatModel");
    qmlRegisterType<FriendChatData>("Qt.ChatModelData", 1, 0, "FriendChatData");
    qmlRegisterType<GifTextHandler>("Qt.GifText", 1, 0, "GifTextHandler");
    qmlRegisterType<FramelessWindow>("Qt.Window", 1, 0, "Frameless");

    // 为一个 QML 组件注册单例类型
    qmlRegisterSingletonType(QUrl("qrc:/common/SkinModel.qml"), "Qt.Singleton", 1, 0, "SkinSingleton");

    // 设置 QML 引擎的根上下文
    QQmlContext *context = engine.rootContext();
    context->setContextProperty("dataBase", DataBase::instance());

    // 基于平台加载 QML 用户界面
    #if (!defined Q_OS_WINDOWS) || (!defined Q_OS_MAC)
    engine.load(QUrl("qrc:/WindowEntry.qml"));
    #else
    engine.load(QUrl("qrc:/WindowEntryLinux.qml"));
    #endif

    // 检查 QML 引擎是否成功加载了根对象
    if (engine.rootObjects().isEmpty())
        return -1;

    // 启动应用程序事件循环
    return app.exec();
}
