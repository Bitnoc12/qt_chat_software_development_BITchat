#ifndef UDPTHREAD_H
#define UDPTHREAD_H

#include <QObject>
#include <QThread>
#include <QUdpSocket>
#include <QSemaphore>
#include <QHostAddress>
#include <QDebug>
#include <QNetworkDatagram>
#include "appcfg.h"
#include <QPixmap>
#include <QBuffer>
#include <QQueue>

struct MessageDesc {
    int type;           // type为SendMessageType
    QString srcUser;
    QByteArray headArr;
    QString destUser;
    QString content;

    // 下面是文件传输标志
    qint64 fileSize;       // kb单位
    QString fileLocal;
    QString addr;
    int port;

};

class UdpThread : public QThread
{
    Q_OBJECT
    void run() override;

    enum State{
        WaitLogin,
        Login,
        LoginOK,
        EXIT
    };

public:
    enum SendMessageType {
        CheckUserExist = 0,
        OnlineGetUser,
        Offline,
        MsgToAll,
        MsgToUser,
        FileToAll,
        FileToUser,
        AddChatRoom,
        PostUserIsExist = 101,
        PostOnlineUser,
    };
    explicit UdpThread(QThread *parent = nullptr);
    ~UdpThread();
    bool getRecvMessage(MessageDesc **msg);
    void messageToSendQueue(const QString& msg, bool isChatRoom, const QString& destUser);

    void fileToSendQueue(const QString& fileName, const qint64& fileSize, const QString& fileLocal,
                         const QString& addr, const int& port, bool isChatRoom, const QString& destUser);

private:
    void sendMessage(SendMessageType type, QString msg = "", QString destUser = "", qint64 fileSize = 0,  QString fileLocal="", QString addr="",  int port = -1);


    bool recvMessage(MessageDesc& msg);

    bool checkUserExist();
    void processMessge();
    bool senderIsMyself(MessageDesc& msg);
    void handlerProcessMessge(MessageDesc& msg);

    void addChatRoom();
    void enRecvqueue(MessageDesc& msg);
    void cleanRecvQueue();

    void deSendQueue();

    QQueue<MessageDesc*> m_recvQueue;
    QSemaphore m_recvQueueSem;
    QQueue<MessageDesc*> m_sendQueue;
    QSemaphore m_sendQueueSem;
    State m_state;
    QUdpSocket *m_udp;
    int m_port;
    QByteArray head_pic;

signals:
    void loginResult(bool result);

public slots:
    void login();
    void returnLogin();
};


#endif // UDPTHREAD_H
