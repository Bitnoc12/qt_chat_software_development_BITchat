#include "friend_model.h"
#include "database.h"

FriendModel::FriendModel(QAbstractListModel *parent)
    : QAbstractListModel(parent),
      m_chat(NULL)
{
    m_role.insert(Qt::UserRole+0, "name");              // 第1个角色名
    m_role.insert(Qt::UserRole+1, "head");              // 第2个角色名
    m_role.insert(Qt::UserRole+2, "recentMsg");         // 第3个角色名
    m_role.insert(Qt::UserRole+3, "recentMsgDate");     // 第4个角色名
    m_role.insert(Qt::UserRole+4, "unreadCnt");         // 第5个角色名



    connect(&m_udp, SIGNAL(loginResult(bool)), this, SIGNAL(loginResult(bool)));

    connect(&m_msgTimer, SIGNAL(timeout()), this, SLOT(onMsgUpdate()));
    m_msgTimer.start(100);
    m_udp.start();
}

FriendModel::~FriendModel()
{
    delete m_chat;
    for (int i=0; i < m_data.count(); i++) {
        delete m_data[i];
    }
    m_data.clear();
}


QString FriendModel::login(QString user, QString pwd)
{
    if(user == "" || pwd == "")
       return "昵称/密码不能为空!";

//    if(!DataBase::instance()->userQuery(user, pwd))
//       return "用户名或密码错误!";

    m_udp.login();
    return "";
}


void FriendModel::returnLogin()
{
    beginResetModel();
    delete m_chat;
    m_chat = NULL;
    for (int i=0; i < m_data.count(); i++) {
        delete m_data[i];
    }
    m_data.clear();
    m_udp.returnLogin();
    emit initialize();
    endResetModel();
}

FriendChatModel * FriendModel::currentChat()
{
    return m_chat;
}

bool FriendModel::friendIsOnline(QString user)
{
    for (int i=0; i < m_data.count(); i++) {
        if (m_data[i]->title() == user) {
            return true;
        }
    }
    return false;

}
void FriendModel::clipboard(QString text)
{
    QClipboard *clip = QGuiApplication::clipboard();
    clip->setText(text);
}
void FriendModel::enterChat(QString user)
{
    for (int i=0; i < m_data.count(); i++) {
        if (m_data[i]->title() == user) {

            if (m_chat != NULL && m_chat->data() == m_data[i])
                return;

            m_data[i]->cleanUnreadCnt();
            m_chat = new FriendChatModel(m_data[i],this);   // 之前的m_chat由ChatListPage.qml来释放
            qDebug()<<"enterChat"<<i<<user<<m_chat;
            emit chatChangeStartAnimation();
            emit dataChanged(i);
            return;
        }
    }
}

void FriendModel::closeChat(FriendChatModel* model)
{
    qDebug()<<"closeChat"<<model;
    delete model;
}

void FriendModel::cleanChatData()
{
    if (!m_chat) return;

    for (int i=0; i < m_data.count(); i++) {
        if (m_chat->data() == m_data[i]) {
            m_chat->cleanMsg();
            emit dataChanged(i);
            return;
        }
    }
}

void FriendModel::sendMsg(QString text)
{
    if (text.length()==0 || !m_chat)
        return;


    for (int i=0; i < m_data.count(); i++) {
        if (m_chat->data() == m_data[i]) {
            m_chat->addMsg(FriendChatData::TypeSend, text);
            m_udp.messageToSendQueue(text, m_data[i]->isChatRoom(), m_data[i]->title());

            // 移动视图(不是聊天室也不是第一个好友)
            if (i != 1 && i!= 0) {
               beginMoveRows(QModelIndex(),i,i,QModelIndex(),1);
               FriendChatData *p=  m_data[i];
               m_data.removeAt(i);
               m_data.insert(1,p);
               endMoveRows();
               i = 1;
            }

            emit dataChanged(i);
            return;
        }
    }
}

void FriendModel::sendFile(QUrl fileLocal)
{
    if (!fileLocal.isValid()  || !m_chat)
        return;
    QString fileStr = fileLocal.toLocalFile();
    QFile file(fileStr);
    if(!file.open(QFile::ReadOnly)) {
        emit errMsg("发送失败,未有读取文件权限!");
        return ;
    }

    QString fileName = QFileInfo(file).fileName();

    if (!FileManager::instance()->serverIsListening()) {
        emit errMsg("发送失败,服务器未启动!");
        return ;
    }

    qDebug()<<file.fileName()<<file.size()<<QFileInfo(file).fileName();


    for (int i=0; i < m_data.count(); i++) {
        if (m_chat->data() == m_data[i]) {
            m_chat->addMsg(FriendChatData::TypeSendFile, fileName, "", file.size(), fileStr, FileManager::instance()->getIp(), FileManager::instance()->getPort());
            m_udp.fileToSendQueue(fileName, file.size(), fileStr , FileManager::instance()->getIp(), FileManager::instance()->getPort(),  m_data[i]->isChatRoom(), m_data[i]->title());

            if (i != 1 && i!= 0) {
               beginMoveRows(QModelIndex(),i,i,QModelIndex(),1);
               FriendChatData *p=  m_data[i];
               m_data.removeAt(i);
               m_data.insert(1,p);
               endMoveRows();
               i = 1;
            }

            emit dataChanged(i);
            return;
        }
    }
}


void FriendModel::onMsgUpdate()
{
    MessageDesc* msg;
    while (m_udp.getRecvMessage(&msg)) {

        switch (msg->type) {
            case UdpThread::PostOnlineUser: addFriend(msg); break;
            case UdpThread::OnlineGetUser : addFriend(msg); break;
            case UdpThread::Offline : removeFriend(msg); break;
            case UdpThread::MsgToAll : msgFromChatRoom(msg); break;
            case UdpThread::MsgToUser : msgFromFriend(msg); break;

            case UdpThread::FileToAll : fileFromChatRoom(msg); break;
            case UdpThread::FileToUser : fileFromFriend(msg); break;

            case UdpThread::AddChatRoom : addChatRoom();   break;
        }
        delete msg;
    }
}
void FriendModel::addChatRoom()
{
    beginInsertRows(QModelIndex(), 0, 0);
    m_data.prepend(new FriendChatData("世界频道", "qrc:/res/chatRoom.png", true));
    endInsertRows();
}
void FriendModel::addFriend(MessageDesc* msg)
{
    for (int i=1; i < m_data.count(); i++) {
        if (m_data[i]->title() == msg->srcUser) {
            return;
        }
    }
    // 将头像 Arr数据转换成jpg文件
    QPixmap pixmap;
#ifdef Q_OS_WINDOWS
    QString str = AppCfg::getInstance()->read(AppCfg::FriendHeadDir).remove("file:///")+msg->srcUser+".jpg";
#else
    QString str = AppCfg::getInstance()->read(AppCfg::FriendHeadDir).remove("file://")+msg->srcUser+".jpg";
#endif
    qDebug()<<msg->headArr.length()<<pixmap.loadFromData((uchar *)msg->headArr.data(), msg->headArr.length());
    bool ret = pixmap.save(str);
    AppCfg::getInstance()->fileLogWrite(QString("addFriend str%1  str%2 ret%3").arg(str).arg(QUrl::fromLocalFile(str).toString()).
                                        arg(ret));

    beginInsertRows(QModelIndex(), 1, 1);   // 由于聊天室始终顶置,所以只能插入到第2行
    m_data.insert(1, new FriendChatData(msg->srcUser, QUrl::fromLocalFile(str).toString()));
    endInsertRows();
    hintInsetChatRoom("\""+msg->srcUser+"\" 于"+QDateTime::currentDateTime().toString(" hh:mm ")+"上线!");
}
void FriendModel::removeFriend(MessageDesc* msg)
{
    for (int i=1; i < m_data.count(); i++) {
        if (m_data[i]->title() == msg->srcUser) {
            beginRemoveRows(QModelIndex(), i, i);

            if (m_chat != NULL && m_chat->data() == m_data[i]) {
                delete m_chat;
                m_chat = NULL;
                emit removeFriendcloseChat();
            }
            delete m_data[i];
            m_data.removeAt(i);
            endRemoveRows();
            hintInsetChatRoom("\""+msg->srcUser+"\" 于"+QDateTime::currentDateTime().toString(" hh:mm ")+"下线!");
            break;
        }
    }
}

void FriendModel::msgFromChatRoom(MessageDesc* msg)
{
    if (m_chat != NULL && m_chat->data() == m_data[0])
       m_chat->addMsg(FriendChatData::TypeRecv, msg->content, msg->srcUser);
    else
        m_data[0]->addRecvMsg(msg->srcUser, msg->content);

    emit dataChanged(0);
}


void FriendModel::msgFromFriend(MessageDesc* msg)
{
    qDebug()<<"msgFromFriend"<<m_data.count()<<msg->srcUser<<msg->content;
    for (int i=1; i < m_data.count(); i++) {
        if (m_data[i]->title() == msg->srcUser) {

             if (m_chat != NULL && m_chat->data() == m_data[i]) {
                m_chat->addMsg(FriendChatData::TypeRecv, msg->content);
             } else {
                m_data[i]->addRecvMsg(msg->srcUser, msg->content);
             }
             if (i != 1) {
                beginMoveRows(QModelIndex(),i,i,QModelIndex(),1);
                FriendChatData *p=  m_data[i];
                m_data.removeAt(i);
                m_data.insert(1,p);
                endMoveRows();
             }

             emit dataChanged(1);

             break;
        }
    }
}

void FriendModel::fileFromChatRoom(MessageDesc* msg)
{
    if (m_chat != NULL && m_chat->data() == m_data[0])
       m_chat->addMsg(FriendChatData::TypeRecvFile, msg->content, msg->srcUser, msg->fileSize, msg->fileLocal, msg->addr, msg->port);
    else
       m_data[0]->addRecvMsg(msg->srcUser, msg->content, msg->fileSize, msg->fileLocal, msg->addr, msg->port);

    emit dataChanged(0);
}
void FriendModel::fileFromFriend(MessageDesc* msg)
{
    qDebug()<<"fileFromFriend"<<m_data.count()<<msg->srcUser<<msg->content<<msg->fileLocal;
    for (int i=1; i < m_data.count(); i++) {
        if (m_data[i]->title() == msg->srcUser) {
            if (m_chat != NULL && m_chat->data() == m_data[i])
               m_chat->addMsg(FriendChatData::TypeRecvFile, msg->content, msg->srcUser,  msg->fileSize, msg->fileLocal, msg->addr, msg->port);
            else
               m_data[i]->addRecvMsg(msg->srcUser, msg->content, msg->fileSize, msg->fileLocal, msg->addr, msg->port);

             if (i != 1) {
                beginMoveRows(QModelIndex(),i,i,QModelIndex(),1);
                FriendChatData *p=  m_data[i];
                m_data.removeAt(i);
                m_data.insert(1,p);
                endMoveRows();
             }

             emit dataChanged(1);

             break;
        }
    }
}

void FriendModel::hintInsetChatRoom(const QString &hint)
{
    if (m_chat != NULL && m_chat->data() == m_data[0]) {
       m_chat->addMsg(FriendChatData::TypeHint, hint);
    } else {
       m_data[0]->addHintMsg(hint);
    }
    emit dataChanged(0);
}


int FriendModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_data.count();
}


QVariant FriendModel::data(const QModelIndex &index, int role) const
{
    switch (role) {
        case Qt::UserRole:  return m_data[index.row()]->title();
        case Qt::UserRole+1:  return m_data[index.row()]->titleUrl();
        case Qt::UserRole+2:  return m_data[index.row()]->recentMsg();
        case Qt::UserRole+3:  return m_data[index.row()]->recentMsgDate();
        case Qt::UserRole+4:  return m_data[index.row()]->unreadCnt();
    }
    return QVariant();
}

QHash<int, QByteArray> FriendModel::roleNames() const
{
    return m_role;
}
