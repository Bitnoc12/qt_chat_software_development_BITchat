#include "friendchat_model.h"
#include <QDebug>
#include <QDateTime>

FriendChatModel::FriendChatModel(FriendChatData *data, QAbstractListModel *parent)
    : QAbstractListModel(parent),
      m_data(data)
{
    // 构造函数，初始化数据和角色映射
    m_role.insert(Qt::UserRole+0, "srcUser");
    m_role.insert(Qt::UserRole+1, "date");
    m_role.insert(Qt::UserRole+2, "msgType");
    m_role.insert(Qt::UserRole+3, "msg");
    m_role.insert(Qt::UserRole+4, "fileSize");
    m_role.insert(Qt::UserRole+5, "fileLocal");
    m_role.insert(Qt::UserRole+6, "addr");
    m_role.insert(Qt::UserRole+7, "port");
}

FriendChatModel::~FriendChatModel()
{
    qDebug() << "~FriendChatModel";
}

FriendChatData* FriendChatModel::data()
{
    return m_data;
}

QString FriendChatModel::title()
{
    if (m_data == NULL)
        return "";

    return m_data->title();
}

bool FriendChatModel::isChatRoom()
{
    return m_data->isChatRoom();
}

int FriendChatModel::rowCount(const QModelIndex &parent) const
{
    if (m_data == NULL)
        return 0;

    return m_data->count();
}

void FriendChatModel::addMsg(FriendChatData::MsgType type, const QString &msg, QString srcUser, qint64 fileSize, QString fileLocal, QString addr, int port)
{
    if (m_data == NULL)
        return;
    const QModelIndex &index = QModelIndex();
    int currentCnt = m_data->count();
    beginInsertRows(index, currentCnt, currentCnt+m_data->msgCntFromMsgType(type)-1);

    switch (type) {
        case FriendChatData::TypeHint: m_data->addHintMsg(msg);  break;
        case FriendChatData::TypeDateHint: m_data->addHintTimer();  break;
        case FriendChatData::TypeRecv: m_data->addRecvMsg(srcUser, msg, 0, "", "", 0, false); break;
        case FriendChatData::TypeSend: m_data->addSendMsg(msg, 0, "", "", 0);  break;
        case FriendChatData::TypeRecvFile: m_data->addRecvMsg(srcUser, msg, fileSize, fileLocal, addr, port, false); break;
        case FriendChatData::TypeSendFile: m_data->addSendMsg(msg, fileSize, fileLocal, addr, port);  break;
    }
    endInsertRows();
}

void FriendChatModel::cleanMsg()
{
    if (m_data == NULL)
        return;

    beginResetModel();
    m_data->cleanData();
    endResetModel();
}

QVariant FriendChatModel::data(const QModelIndex &index, int role) const
{
    if (m_data != NULL) {
        switch (role) {
            case Qt::UserRole:  return  m_data->at(index.row())->srcUser;
            case Qt::UserRole+1:  return m_data->at(index.row())->date;
            case Qt::UserRole+2:  return m_data->at(index.row())->msgType;
            case Qt::UserRole+3:  return m_data->at(index.row())->msg;
            case Qt::UserRole+4:  return m_data->at(index.row())->fileSize;
            case Qt::UserRole+5:  return m_data->at(index.row())->fileLocal;
            case Qt::UserRole+6:  return m_data->at(index.row())->addr;
            case Qt::UserRole+7:  return m_data->at(index.row())->port;
        }
    }
    return QVariant();
}

QHash<int, QByteArray> FriendChatModel::roleNames() const
{
    return m_role;
}
