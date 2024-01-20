#include "udpthread.h"
#include <QDateTime>
#include <QNetworkProxy>

UdpThread::UdpThread(QThread *parent) : QThread(parent),
    m_recvQueueSem(0),
    m_sendQueueSem(0),
    m_state(WaitLogin),
    m_port(44544)
{
}

// 登录状态
void UdpThread::login()
{
     m_state = Login;

}

UdpThread::~UdpThread()
{
   qDebug()<<"~UdpThread";
   m_state = EXIT;
   while (this->isRunning());
}

// 返回登录等待状态
void UdpThread::returnLogin()
{
     m_state = WaitLogin;
}

// 发送消息的方法
void UdpThread::sendMessage(SendMessageType type, QString msg, QString destUser, qint64 fileSize, QString fileLocal, QString addr,  int port)
{
    QByteArray data;
    QDataStream out(&data,QIODevice::WriteOnly);
    out << type << AppCfg::getInstance()->read(AppCfg::CurrentUser);

    QBuffer bf(&head_pic);
    QPixmap head;
    switch (type) {
        case OnlineGetUser:
           head_pic.resize(0);
#ifdef Q_OS_WINDOWS
            head.load(AppCfg::getInstance()->read(AppCfg::HeadImageSmall).remove("file:///").remove("qrc"));
#else
            head.load(AppCfg::getInstance()->read(AppCfg::HeadImageSmall).remove("file://").remove("qrc"));
#endif
           head.save(&bf, "jpg");
           out << head_pic;
           break;

        case PostOnlineUser:
           out << head_pic;
           break;

        case MsgToAll:
           out << msg;
           break;

        case MsgToUser:
           out << destUser << msg;
           break;
        case FileToAll:
           out << msg << fileSize << fileLocal << addr << port;
           break;

        case FileToUser:
           out << destUser << msg << fileSize << fileLocal << addr << port;
           break;

        default:
           break;
    }
    m_udp->writeDatagram(data,data.length(),QHostAddress::Broadcast, m_port);
}

// 检查用户是否存在
bool UdpThread::checkUserExist()
{
    bool ret = false;

    // 清除之前的资源
    cleanRecvQueue();
    if (m_udp->hasPendingDatagrams() ) {
        m_udp->receiveDatagram();
    }

    sendMessage(CheckUserExist);
    qint64 mSec = QDateTime::currentDateTime().toMSecsSinceEpoch();
    MessageDesc msg;
    while(QDateTime::currentDateTime().toMSecsSinceEpoch() - mSec <= 1400) {
        if (recvMessage(msg)) {
            if (msg.type == PostUserIsExist &&
                AppCfg::getInstance()->read(AppCfg::CurrentUser) == msg.srcUser) {
                ret = true;
                break;
            }
        }
    }
    emit loginResult(ret);
    return ret;
}

// 接收消息
bool UdpThread::recvMessage(MessageDesc& msg)
{
    QByteArray datagram;
    QDataStream in(&datagram,QIODevice::ReadOnly);
    if (m_udp->hasPendingDatagrams()) {
        datagram.resize(m_udp->pendingDatagramSize());   //调整为接收到数据的大小
        m_udp->readDatagram(datagram.data(),datagram.size());
        QString srcUser;
        in >> msg.type;
        in >> msg.srcUser;
        AppCfg::getInstance()->fileLogWrite(QString("recvMessage type%1 srcUser%2").arg(msg.type).arg(msg.srcUser));

        if (msg.type == OnlineGetUser) {
            in >> msg.headArr;
        } else if (msg.type == PostOnlineUser) {
            in >> msg.headArr;
        } else if (msg.type == MsgToAll) {
            in >> msg.content;
        } else if (msg.type == MsgToUser) {
            in >> msg.destUser;
            in >> msg.content;
        } else if (msg.type == FileToAll) {
            in >> msg.content;
            in >> msg.fileSize;
            in >> msg.fileLocal;
            in >> msg.addr;
            in >> msg.port;
        } else if (msg.type == FileToUser) {
            in >> msg.destUser;
            in >> msg.content;
            in >> msg.fileSize;
            in >> msg.fileLocal;
            in >> msg.addr;
            in >> msg.port;
        }

        return true;

    }
    return false;
}

// 处理消息
void UdpThread::processMessge()
{
    MessageDesc msg;
    while (m_state == LoginOK) {
        if (recvMessage(msg)) {
            handlerProcessMessge(msg);
        }
        deSendQueue();
        msleep(10);
    }

    sendMessage(Offline);

    if (m_state == EXIT) {
        m_udp->disconnectFromHost();
    }
}

// 检查消息发送者是否为自己
bool UdpThread::senderIsMyself(MessageDesc& msg)
{
    if (msg.srcUser == AppCfg::getInstance()->read(AppCfg::CurrentUser)) {
        if (msg.type == CheckUserExist) {
            sendMessage(PostUserIsExist);
        }
        return true;

    }
    return false;
}

// 处理消息的方法
void UdpThread::handlerProcessMessge(MessageDesc& msg)
{
    if (senderIsMyself(msg)) {
        return;
    }

    switch (msg.type) {
        case OnlineGetUser:
            enRecvqueue(msg);
            sendMessage(PostOnlineUser);
            break;
        case MsgToUser:
            if (msg.destUser == AppCfg::getInstance()->read(AppCfg::CurrentUser))
                enRecvqueue(msg);
            break;

        case FileToUser:
            if (msg.destUser == AppCfg::getInstance()->read(AppCfg::CurrentUser))
                enRecvqueue(msg);
            break;

        default:
            enRecvqueue(msg);
            break;
    }
}

// 将消息添加到发送队列
void UdpThread::messageToSendQueue(const QString& msg, bool isChatRoom, const QString& destUser)
{
    MessageDesc *pMsg = new MessageDesc;
    pMsg->type = isChatRoom ? MsgToAll : MsgToUser;
    pMsg->srcUser = AppCfg::getInstance()->read(AppCfg::CurrentUser);
    pMsg->destUser = destUser;
    pMsg->content = msg;
    m_sendQueue.enqueue(pMsg);
    m_sendQueueSem.release();
}

// 将文件消息添加到发送队列
void UdpThread::fileToSendQueue(const QString& fileName, const qint64 &fileSize,
                                const QString& fileLocal, const QString& addr, const int& port, bool isChatRoom, const QString& destUser)
{
    MessageDesc *pMsg = new MessageDesc;
    pMsg->type = isChatRoom ? FileToAll : FileToUser;
    pMsg->srcUser = AppCfg::getInstance()->read(AppCfg::CurrentUser);
    pMsg->destUser = destUser;
    pMsg->fileLocal = fileLocal;
    pMsg->addr = addr;
    pMsg->port = port;
    pMsg->content = fileName;
    pMsg->fileSize = fileSize;
    m_sendQueue.enqueue(pMsg);
    m_sendQueueSem.release();
}

// 获取接收消息
bool UdpThread::getRecvMessage(MessageDesc** msg)
{
    if (m_state == LoginOK && m_recvQueueSem.tryAcquire()) {
        *msg = m_recvQueue.dequeue();
        return true;
    }
    return false;
}

// 添加聊天室
void UdpThread::addChatRoom()
{
    MessageDesc msg;
    msg.type = AddChatRoom;
    enRecvqueue(msg);
}

// 将消息添加到接收队列
void UdpThread::enRecvqueue(MessageDesc& msg)
{
   MessageDesc *pMsg = new MessageDesc;
   pMsg->type = msg.type;
   pMsg->srcUser = msg.srcUser;
   pMsg->headArr = msg.headArr;
   pMsg->destUser = msg.destUser;
   pMsg->content = msg.content;

   pMsg->fileSize = msg.fileSize;
   pMsg->fileLocal = msg.fileLocal;
   pMsg->addr = msg.addr;
   pMsg->port = msg.port;

   m_recvQueue.enqueue(pMsg);
   m_recvQueueSem.release();
}

// 清空接收队列
void UdpThread::cleanRecvQueue()
{
    while (m_recvQueue.count()) {
        if (m_recvQueueSem.tryAcquire()) {
            MessageDesc* msg = m_recvQueue.dequeue();
            delete msg;
        }
    }
}

// 处理发送队列
void UdpThread::deSendQueue()
{
    while(m_sendQueueSem.tryAcquire()) {
        MessageDesc* msg = m_sendQueue.dequeue();
        sendMessage((SendMessageType)msg->type, msg->content, msg->destUser, msg->fileSize, msg->fileLocal, msg->addr, msg->port);
        delete msg;
    }
}

// 线程运行函数
void UdpThread::run()
{
    m_udp = new QUdpSocket();

    m_udp->setProxy(QNetworkProxy::DefaultProxy);

    m_udp->bind(QHostAddress::Any, m_port, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
    while(1) {
        switch (m_state) {
            case WaitLogin:
                msleep(10);
                break;
            case Login:
                if (checkUserExist()) {
                   m_state =  WaitLogin;
                } else {
                   addChatRoom();
                   sendMessage(OnlineGetUser);
                   m_state =  LoginOK;
                }
                break;

            case LoginOK:
                processMessge();
                break;
            case EXIT:
                delete m_udp;
                return;
        }
    }
}
