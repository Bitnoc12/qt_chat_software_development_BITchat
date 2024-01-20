#include "friendchat_data.h"
#include <QDebug>

FriendChatData::FriendChatData(QObject *parent)
    : QObject(parent)
{
    // 构造函数
}

FriendChatData::FriendChatData(QString name, QString titleUrl, bool isChatRoom, QObject *parent)
    : QObject(parent),
      m_title(name),
      m_titleUrl(titleUrl),
      m_isChatRoom(isChatRoom),
      m_preMsgSec(0),
      m_unreadCnt(0)
{
    // 带参数的构造函数
    if (!m_isChatRoom) {
        addHintMsg("你好友上线了，可以开始聊天了。");
    } else {
        addHintMsg("你已进入世界频道。");
    }
}

FriendChatData::~FriendChatData()
{
    // 析构函数，释放资源
    cleanData();
}

ChatPrivate* FriendChatData::at(int i)
{
    if (i >= m_data.count())
        return NULL;

    return m_data[i];
}

int FriendChatData::count()
{
    return m_data.count();
}
QString FriendChatData::title()
{
    return m_title;
}

QString FriendChatData::titleUrl()
{
    return m_titleUrl;
}

bool FriendChatData::isChatRoom()
{
    return m_isChatRoom;
}

QString FriendChatData::recentMsg()
{
   if (m_data.count() == 0)
       return "";

   return m_data[m_data.count()-1]->msg;
}
QString FriendChatData::recentMsgDate()
{
    if (m_data.count() == 0)
        return "";

    return m_data[m_data.count()-1]->date;
}

int FriendChatData::unreadCnt()
{
    return m_unreadCnt;
}

void FriendChatData::cleanUnreadCnt()
{
    m_unreadCnt = 0;
}

void FriendChatData::cleanData()
{
    // 清空数据，释放内存
    for (int i = 0; i < m_data.count(); i++) {
        delete m_data[i];
    }
    m_data.clear();
    m_preMsgSec = 0;
}

void FriendChatData::addHintMsg(QString msg)
{
    // 添加提示消息
    ChatPrivate* p = new ChatPrivate;
    p->msg = msg;
    p->msgType = TypeHint;
    p->date = QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm");
    m_data.append(p);
}
void FriendChatData::addHintTimer()
{
    // 添加时间提示
    ChatPrivate* p = new ChatPrivate;
    p->msgType = TypeDateHint;
    p->date = QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm");
    m_data.append(p);
}

void FriendChatData::addRecvMsg(QString srcUser, QString msg, qint64 fileSize, QString fileLocal, QString addr, int port, bool unreadFlag)
{
    // 添加接收的消息
    checkPreMsgAddTimeHint();
    ChatPrivate* p = new ChatPrivate;
    p->msg = msg;

    p->msgType =  fileLocal == "" ? TypeRecv : TypeRecvFile;
    p->srcUser = srcUser == "" ?  m_title : srcUser;
    p->fileLocal = fileLocal;
    p->fileSize = fileSize;
    p->addr = addr;
    p->port = port;
    p->date = QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm");
    m_data.append(p);
    if (unreadFlag) {
        m_unreadCnt += 1;
    } else {
        m_unreadCnt = 0;
    }
}

void FriendChatData::addSendMsg(QString msg, qint64 fileSize, QString fileLocal, QString addr, int port)
{
    // 添加发送的消息
    checkPreMsgAddTimeHint();
    ChatPrivate* p = new ChatPrivate;
    p->msg = msg;
    p->msgType = fileLocal == "" ? TypeSend : TypeSendFile;
    p->fileLocal = fileLocal;
    p->fileSize = fileSize;
    p->addr = addr;
    p->port = port;
    p->date = QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm");
    m_data.append(p);
}

int FriendChatData::msgCntFromMsgType(const MsgType &type)
{
    // 根据消息类型获取消息计数
    switch (type) {
        case FriendChatData::TypeHint:
        case FriendChatData::TypeDateHint: return 1;
        case FriendChatData::TypeRecv:
        case FriendChatData::TypeSend:
        case FriendChatData::TypeRecvFile:
        case FriendChatData::TypeSendFile:
            if (m_preMsgSec == 0 ||
                    QDateTime::currentDateTime().toSecsSinceEpoch() - m_preMsgSec > 60) {
                return 2;
            }
    }
    return 1;
}

void FriendChatData::checkPreMsgAddTimeHint()
{
    // 检查是否需要添加时间提示
    if (m_preMsgSec == 0 ||
            QDateTime::currentDateTime().toSecsSinceEpoch() - m_preMsgSec > 60) {
        addHintTimer();
    }
    m_preMsgSec = QDateTime::currentDateTime().toSecsSinceEpoch();
}
