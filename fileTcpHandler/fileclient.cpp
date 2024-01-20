#include "fileclient.h"
#include "appcfg.h"
#include <QNetworkProxy>

// 尝试与服务器建立连接的函数
// 如果成功接收到数据，则返回true
bool ClientTask::connectProbe(QTcpSocket *tcpSocket)
{
    m_status = "下载:等待连接";

    // 在TCP套接字处于ConnectedState状态时循环执行
    while (tcpSocket->state() == QTcpSocket::ConnectedState) {
        // 等待1秒以接收传入数据
        if (tcpSocket->waitForReadyRead(1000)) {
            qDebug() << "client connectProbe read:" << tcpSocket->readAll();
            return true; // 连接成功
        }

        // 检查操作是否被取消
        if (m_isCancel) {
            m_status = "下载ERR:取消下载";
            break;
        }

        qDebug() << "client connectProbe:" << tcpSocket->state()
                 << QDateTime::currentDateTime().toString("hh:mm:ss:zzz ");
    }

    return false; // 连接失败
}

// 处理文件下载过程的函数
bool ClientTask::downFile(QTcpSocket *tcpSocket)
{
    // 构造包含用户信息和文件详情的数据字符串
    QString data = QString("%1||%2||%3")
            .arg(AppCfg::getInstance()->read(AppCfg::CurrentUser))
            .arg(m_fileTarget)
            .arg(m_fileSize);

    // 将数据写入套接字，并等待写入操作完成
    tcpSocket->write(data.toUtf8(), data.toUtf8().size());
    tcpSocket->waitForBytesWritten(5000);

    // 尝试打开本地文件以便写入
    QFile downloadFile(m_fileUrl);
    if (!downloadFile.open(QFile::WriteOnly)) {
        qDebug() << "file write error";
        m_status = "下载ERR:文件无权限";
        return false; // 文件写入错误
    }
    m_status = "正在下载";

    qDebug() << "downFile: " << m_fileUrl << m_fileName << downloadFile.size();
    m_currentByte = 0;
    int invalidCnt = 0;
    qint64 preCrtByte = 0;
    qint64 preMSec = QDateTime::currentDateTime().toMSecsSinceEpoch();

    // 在完整文件接收之前持续下载
    while (m_currentByte < m_fileSize) {

        // 等待100毫秒以接收传入数据
        tcpSocket->waitForReadyRead(100);

        // 从套接字中读取传入数据
        QByteArray arr = tcpSocket->read(qMin(m_fileSize - m_currentByte, m_payloadSize));

        if (arr.length() > 0) {
            // 将接收到的数据写入本地文件
            downloadFile.write(arr);
            m_currentByte = downloadFile.size();
            invalidCnt = 0;
        } else if (tcpSocket->state() == QTcpSocket::UnconnectedState && m_currentByte != m_fileSize) {
            m_status = "下载ERR:通信断开";
            break;
        } else {
            invalidCnt++;
        }

        // 检查操作是否被取消
        if (m_isCancel) {
            m_status = "下载ERR:取消下载";
            break;
        }

        // 检查通信超时
        if (invalidCnt++ > 50) {
            m_status = "下载ERR:通信超时";
            break;
        }

        // 更新下载速度信息
        if (updateSpeed(preCrtByte, preMSec)) {
            qDebug() << "client:" << tcpSocket->readBufferSize() << invalidCnt << m_currentByte
                     << QDateTime::currentDateTime().toString("hh:mm:ss:zzz ");
        }
    }

    m_speed = 0;
    downloadFile.flush();
    downloadFile.close();

    // 检查是否成功下载了整个文件
    if (m_currentByte == m_fileSize) {
        m_status = "下载完成";
        return true; // 下载成功
    } else {
        return false; // 下载失败
    }
}

// 线程执行的主要入口点
void ClientTask::run()
{
    QTcpSocket tcpSocket;
    tcpSocket.setReadBufferSize(m_buffSize);
    tcpSocket.connectToHost(m_addr, m_port);

    // 为套接字设置默认网络代理
    tcpSocket.setProxy(QNetworkProxy::DefaultProxy);

    if (tcpSocket.waitForConnected(1000)) {
        if (connectProbe(&tcpSocket)) {
            downFile(&tcpSocket);
        }
        tcpSocket.close();
    } else {
        m_status = "下载ERR:连接超时";
    }
    m_running = false;
}
