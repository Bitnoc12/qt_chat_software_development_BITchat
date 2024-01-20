#ifndef FriendMODEL_H
#define FriendMODEL_H

#include <QAbstractListModel>
#include <QTimer>
#include <QDebug>
#include <QGuiApplication>
#include <QClipboard>
#include "udpthread.h"
#include "friendchat_data.h"
#include "friendchat_model.h"
#include "fileManager.h"

class FriendModel : public QAbstractListModel
{
    Q_OBJECT

public:

    FriendModel(QAbstractListModel *parent = nullptr);
    ~FriendModel();
    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QHash<int, QByteArray> roleNames() const;
    Q_INVOKABLE void returnLogin();

    Q_INVOKABLE FriendChatModel * currentChat();
    Q_INVOKABLE void enterChat(QString user);
    Q_INVOKABLE void closeChat(FriendChatModel* model);
    Q_INVOKABLE void sendMsg(QString text);
    Q_INVOKABLE void sendFile(QUrl fileLocal);
    Q_INVOKABLE void cleanChatData();
    Q_INVOKABLE QString login(QString user, QString pwd);

    Q_INVOKABLE void clipboard(QString text);

    Q_INVOKABLE bool friendIsOnline(QString user);

protected:
    void addChatRoom();
    void addFriend(MessageDesc* msg);
    void removeFriend(MessageDesc* msg);
    void msgFromChatRoom(MessageDesc* msg);
    void msgFromFriend(MessageDesc* msg);
    void fileFromChatRoom(MessageDesc* msg);
    void fileFromFriend(MessageDesc* msg);
    void hintInsetChatRoom(const QString &hint);

signals:
    void loginResult(bool result);
    void chatChangeStartAnimation();
    void removeFriendcloseChat();
    void initialize();
    void dataChanged(int rowId);

    void errMsg(QString msg);
    void successMsg(QString msg);

public slots:
    void onMsgUpdate();

private:
    UdpThread m_udp;
    QHash<int, QByteArray> m_role; // 每个m_data[i]中的用户角色
    QList<FriendChatData * > m_data;
    QTimer m_msgTimer;
    FriendChatModel * m_chat;


};

// 定义一个回调类指针,用于接收回调.
static QObject *FriendModel_qobject_provider(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    //Q_UNUSED: 向编译器指示参数未在函数的主体中使用。这可用于抑制编译器警告
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)

    FriendModel *model = new FriendModel();
    return model;
}

#endif // FriendMODEL_H
