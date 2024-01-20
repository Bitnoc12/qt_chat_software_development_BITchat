#ifndef FRIENDCHATDATA_H
#define FRIENDCHATDATA_H

#include <QObject>
#include <QDateTime>



struct ChatPrivate
{
public:
    QString srcUser;    // 发送者名称
    QString date;       // 日期
    int msgType;        // 消息类型
    QString msg;        // 消息内容

    // 下面是文件标志
    qint64 fileSize;
    QString fileLocal;
    QString addr;
    int port;
};

class FriendChatData : public QObject
{
    Q_OBJECT
public:
    enum MsgType {
        TypeHint  =0,
        TypeDateHint ,
        TypeRecv,
        TypeSend,
        TypeRecvFile,
        TypeSendFile,
    };
    Q_ENUM(MsgType)
    explicit FriendChatData(QObject *parent = nullptr);
    explicit FriendChatData(QString name, QString titleUrl, bool isChatRoom = false, QObject *parent = nullptr);
    ~FriendChatData();
    int count();
    QString title();
    QString titleUrl();
    bool  isChatRoom();

    void addHintMsg(QString msg);
    void addHintTimer();

    void addRecvMsg(QString srcUser, QString msg, qint64 fileSize = 0, QString fileLocal = "", QString addr = "", int port = 0, bool unreadFlag = true);

    void addSendMsg(QString msg, qint64 fileSize = 0, QString fileLocal = "", QString addr = "", int port = 0);

    QString recentMsg();        // 最近消息
    QString recentMsgDate();    // 最新消息时间
    int unreadCnt();            // 未读条数
    void cleanUnreadCnt();      // 清除未读条数
    void cleanData();
    ChatPrivate* at(int i);

    int msgCntFromMsgType(const MsgType &type);

protected:
    void checkPreMsgAddTimeHint();

signals:


private:
    QList<ChatPrivate * > m_data;
    QString m_title;
    QString m_titleUrl;
    bool m_isChatRoom;
    qint64 m_preMsgSec;
    int m_unreadCnt;
};

#endif // FRIENDCHATDATA_H
