#ifndef FriendCHATMODEL_H
#define FriendCHATMODEL_H

#include <QAbstractListModel>
#include "appcfg.h"
#include "friendchat_data.h"


class FriendChatModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit FriendChatModel(FriendChatData *data = nullptr, QAbstractListModel *parent = nullptr);
    ~FriendChatModel();
    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QHash<int, QByteArray> roleNames() const;
    FriendChatData * data();
    Q_INVOKABLE QString title();
    Q_INVOKABLE bool isChatRoom();
    void addMsg(FriendChatData::MsgType type,const QString &msg, QString srcUser = "", qint64 fileSize = 0,
                QString fileLocal = "", QString addr = "", int port = 0);
    void cleanMsg();
signals:


private:
    FriendChatData* m_data;
    QHash<int, QByteArray> m_role; // 每个m_data[i]中的用户角色
};

#endif // FriendCHATMODEL_H
