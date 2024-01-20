#include "fileserver.h"
#include <QFileInfo>
#include <QNetworkProxy>

// 尝试与客户端建立连接的函数
bool ServerTask::connectProbe(QTcpSocket *tcpSocket)
{
    m_status = "上传:建立连接";
    tcpSocket->write("connecting"); // 向客户端发送连接确认
    while (tcpSocket->state() == QTcpSocket::ConnectedState) {
        if (tcpSocket->waitForBytesWritten(1000)) {
            return true; // 连接成功
        }
        if (m_isCancel) {
           m_status = "下载ERR:取消下载";
           break;
        }

        qDebug() << "client connectProbe:" << tcpSocket->state()
                 << QDateTime::currentDateTime().toString("hh:mm:ss:zzz ");
    }
    return false; // 连接失败
}

// 处理文件上传过程的函数
bool ServerTask::uploadFile(QTcpSocket *tcpSocket)
{
    QFile file(m_fileUrl);
    if (!file.open(QFile::ReadOnly)) {
        m_status = "上传ERR:文件无权限";
        qDebug() << "ServerTask::uploadFile failed";
        return false; // 文件无权限
    }
    m_fileName = QFileInfo(m_fileUrl).fileName();
    if (file.size() != m_fileSize) {
        qDebug() << "ServerTask::uploadFile check size err!";
        m_status = "上传ERR:文件参数不匹配";
        return false; // 文件大小不匹配
    }
    m_status = "正在上传";

    int invalidCnt = 0;
    m_currentByte = 0;
    qint64 preCrtByte = 0;
    qint64 preMSec = QDateTime::currentDateTime().toMSecsSinceEpoch();
    QByteArray arr = file.read(qMin(m_fileSize - m_currentByte, m_payloadSize));

    // 在文件未完全上传之前持续传输
    while (m_currentByte < m_fileSize) {
        tcpSocket->waitForBytesWritten(100);
        qint64 byte = tcpSocket->write(arr);

        if (byte > 0) {
             m_currentByte += byte;
             arr = file.read(qMin(m_fileSize - m_currentByte, m_payloadSize));
             invalidCnt = 0;
        } else if (tcpSocket->state() == QTcpSocket::UnconnectedState && m_currentByte != m_fileSize) {
            m_status = "上传ERR:通信断开";
            break;
        } else {
            invalidCnt ++;
        }

        if (m_isCancel) {
           m_status = "上传ERR:取消上传";
           break;
        }

        if (invalidCnt++ > 50) {
           m_status = "上传ERR:通信超时";
           break;
        }

        if (updateSpeed(preCrtByte, preMSec)) {
            qDebug() << "server:" << tcpSocket->readBufferSize() << invalidCnt << m_currentByte
                     << QDateTime::currentDateTime().toString("hh:mm:ss:zzz ")
                     << m_speed << byte << arr.length();
        }
    }

    m_speed = 0;
    file.close();
    if (m_currentByte == m_fileSize) {
        m_status = "上传完成";
        return true; // 上传成功
    } else {
        return false; // 上传失败
    }
}

// 线程执行的主要入口点
void ServerTask::run()
{
    QTcpSocket tcpSocket;
    tcpSocket.setReadBufferSize(m_buffSize);

    // 使用给定的套接字描述符设置套接字
    if (!tcpSocket.setSocketDescriptor(m_socketDescriptor)) {
        m_status = "上传ERR:Socket异常";
        qDebug() << "tcpSocket.setSocketDescriptor(m_socketDescriptor) failed";
        return;
    }

    if (connectProbe(&tcpSocket)) {
        if (tcpSocket.waitForReadyRead(3000)) {
            QString str = QString::fromUtf8(tcpSocket.readAll());
            QStringList list = str.split("||");

            // 用户名 文件位置  文件大小
            if (list.length() == 3) {
                m_tagetNmae = list[0];
                m_fileUrl = list[1];
                m_fileSize = list[2].toLongLong();
                uploadFile(&tcpSocket);
            } else {
                m_status = "上传ERR:参数错误";
            }
        } else {
            m_status = "上传ERR:无响应";
            qDebug() << "获取要下载的文件 err";
        }
    }

    tcpSocket.disconnectFromHost();
    tcpSocket.waitForDisconnected();
    m_running = false;
}

// 文件服务器类的构造函数
FileServer::FileServer(QObject *parent) : QTcpServer(parent)
{
    m_port = 8001; // 默认端口号

    setProxy(QNetworkProxy::DefaultProxy); // 设置默认网络代理

    // 尝试监听指定端口，如果端口已被占用，则递增端口号
    while (!listen(QHostAddress::AnyIPv4, m_port)) {
        m_port += 1;

        if (m_port > 40000)
            return; // 避免无法找到可用端口
    }
}

// 新客户端连接时的回调函数
void FileServer::incomingConnection(qintptr socketDescriptor)
{
    emit newClientConnected(socketDescriptor); // 发送新客户端连接信号
}
