#ifndef FILEHANDLERMODEL_H
#define FILEHANDLERMODEL_H
#include <QQmlEngine>
#include <QObject>
#include "fileTcpHandler/fileclient.h"
#include "fileTcpHandler/fileserver.h"
#include "fileTcpHandler/fileTask.h"


class FileManager : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int underwayCnt READ underwayCnt NOTIFY underwayCntChanged)

signals:
    void underwayCntChanged();

public:
    explicit FileManager(QObject *parent = nullptr);
    ~FileManager();
    int underwayCnt() { return underwayList.length(); }

    static FileManager* instance()
    {
        if(m_instance==NULL)
           m_instance = new FileManager();
        return m_instance;
    }

    QString getIp();
    int getPort();
    bool serverIsListening();

    Q_INVOKABLE void newDownFileTask(QUrl fileLocal, QString fileTarget,
                                     qint64 fileSize,  QString targetName, QString addr, int port);

    Q_INVOKABLE QVariantList getUnderwayData();

    Q_INVOKABLE void removeUnderwayTask(QString id, QString fileUrl);
    Q_INVOKABLE void requesetFinishDataRerensh();
    Q_INVOKABLE bool openExplorer(QString fileLocal);
    Q_INVOKABLE bool openFile(QString fileLocal);


signals:
    void finishDataRefrensh(QVariantList list); // 刷新完成列表(不需要实时获取)

private slots:
    void onUploadFileTask(qintptr socketDescriptor);

private:
    static FileManager *m_instance;

    QList<FileTask*> underwayList;

    FileServer server;
};


// 定义一个回调类指针,用于接收回调.
static QObject *fileManager_qobject_provider(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    //Q_UNUSED: 向编译器指示参数未在函数的主体中使用。这可用于抑制编译器警告
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)

    return FileManager::instance();
}


#endif // FILEHANDLERMODEL_H
