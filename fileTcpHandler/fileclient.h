#ifndef FILECLIENT_H
#define FILECLIENT_H

#include <QObject>
#include <QTcpSocket>

#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include "fileTask.h"

class ClientTask : public FileTask
{
    void run() override;


    bool connectProbe(QTcpSocket *tcpSocket);
    bool downFile(QTcpSocket *tcpSocket);


public:
    ClientTask(QString fileLocal, QString fileTarget,
               qint64 fileSize,  QString targetName, QString addr, int port) {

        m_fileName = QFileInfo(fileLocal).fileName();
        m_fileTarget = fileTarget;
        m_fileUrl = fileLocal;
        m_fileSize = fileSize;
        m_addr = addr;
        m_port = port;
        m_tagetNmae = targetName;
    }

private:
    QString m_fileTarget;
    QString m_addr;
    int m_port;
};




#endif // FILECLIENT_H
