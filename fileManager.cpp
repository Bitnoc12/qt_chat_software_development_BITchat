#include "fileManager.h"
#include "database.h"
#include <QUrl>
#include <QDesktopServices>
#include <QDir>

// FileManager 类的构造函数
FileManager* FileManager::m_instance = NULL;
FileManager::FileManager(QObject *parent) : QObject(parent)
{
    QThreadPool::globalInstance()->setExpiryTimeout(-1);    // 禁止队列过期
    QThreadPool::globalInstance()->setMaxThreadCount(3);    // 线程池3个
    connect(&server, SIGNAL(newClientConnected(qintptr)), this, SLOT(onUploadFileTask(qintptr)));
}

// FileManager 类的析构函数
FileManager::~FileManager()
{
    QMutableListIterator<FileTask*> it(underwayList);
    while (it.hasNext()) {
        FileTask* task = it.next();
        task->cancelTask();
        if (QThreadPool::globalInstance()->tryTake(task)) {
            delete task;
            it.remove();
        }
    }
}

// 创建新的下载任务
void FileManager::newDownFileTask(QUrl fileLocal, QString fileTarget,
                                 qint64 fileSize,  QString targetName, QString addr, int port)
{
    FileTask* task = new ClientTask(fileLocal.toLocalFile(), fileTarget, fileSize, targetName, addr, port);
    underwayList.append(task);

    QThreadPool::globalInstance()->start(task);

    emit underwayCntChanged();
}

// 处理上传文件任务
void FileManager::onUploadFileTask(qintptr socketDescriptor)
{
    FileTask* task = new ServerTask(socketDescriptor, this);
    underwayList.append(task);
    QThreadPool::globalInstance()->start(task);
    emit underwayCntChanged();
}

// 获取本机 IP 地址
QString FileManager::getIp()
{
    QList<QHostAddress> list = QNetworkInterface::allAddresses();
    foreach (QHostAddress address, list) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol)
            return address.toString();
    }
    return 0;
}

// 获取文件服务器监听的端口号
int FileManager::getPort()
{
    return server.m_port;
}

// 判断文件服务器是否正在监听
bool FileManager::serverIsListening()
{
    return server.isListening();
}

// 移除正在进行的任务
void FileManager::removeUnderwayTask(QString id, QString fileUrl)
{
    for (int i = 0; i < underwayList.length(); i++) {
        if (underwayList[i]->getId() == id && underwayList[i]->getFileUrl() == fileUrl) {
           underwayList[i]->cancelTask();

           if (QThreadPool::globalInstance()->tryTake(underwayList[i])) {
               delete underwayList[i];
               underwayList.removeAt(i);
               emit underwayCntChanged();
           }
        }
    }
    emit underwayCntChanged();
}

// 获取正在进行的任务数据列表
QVariantList FileManager::getUnderwayData()
{
   QVariantList list;

   QMutableListIterator<FileTask*> it(underwayList);

   bool finishRefrensh = false;
   while (it.hasNext()) {
       FileTask* task = it.next();

       if (!task->getRunniing() && task->isCancel()) {
           delete task;
           it.remove();
           finishRefrensh = true;
       } else if (!task->getRunniing() && task->getStatus().indexOf("完成") >= 0) {
           DataBase::instance()->insetFinishFileLog(
                         task->getId(),
                         task->getStatus(),
                         task->getTagetNmae(),
                         task->getFileName(),
                         QString("%1").arg(task->getFileSize()),
                         task->getFileUrl()
                       );
           delete task;
           it.remove();
           finishRefrensh = true;
       } else {
           QVariantMap map;
           map["id"] = task->getId();
           map["status"] = task->getStatus();
           map["fileName"] = task->getFileName();
           map["fileUrl"] = task->getFileUrl();
           map["fileSize"] = task->getFileSize();
           map["tagetName"] = task->getTagetNmae();
           map["currentByte"] = task->getCurrentByte();
           map["speed"] = task->getSpeedByte();
           list.append(map);
       }
   }

   if (finishRefrensh) {
       requesetFinishDataRerensh();
       emit underwayCntChanged();
   }

   return list;
}

// 请求刷新已完成任务数据
void FileManager::requesetFinishDataRerensh()
{
    emit finishDataRefrensh(DataBase::instance()->finishFileLogQueryAll());
}

// 打开文件资源管理器
bool FileManager::openExplorer(QString fileLocal)
{
    if (!QFile::exists(fileLocal)) {
        return false;
    }

    if (!QFileInfo(fileLocal).isDir()) {
        fileLocal = QFileInfo(fileLocal).absoluteDir().absolutePath();
    }

    QDesktopServices::openUrl("file:///" + fileLocal);
    return true;
}

// 打开文件
bool FileManager::openFile(QString fileLocal)
{
    if (!QFile::exists(fileLocal)) {
        return false;
    }

    QDesktopServices::openUrl("file:///" + fileLocal);
    return true;
}
