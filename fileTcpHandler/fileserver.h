#ifndef FILESERVER_H
#define FILESERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QRunnable>
#include <QThread>
#include <QThreadPool>
#include <QDebug>
#include <QFile>
#include <QNetworkInterface>
#include "fileTask.h"


class ServerTask : public FileTask
{
    void run() override;

    bool connectProbe(QTcpSocket *tcpSocket);
    bool uploadFile(QTcpSocket *tcpSocket);
public:
    ServerTask(qintptr socketDescriptor, QObject *obj) {
      m_socketDescriptor = socketDescriptor;
    }

};




class FileServer : public QTcpServer
{
    Q_OBJECT

    void incomingConnection(qintptr socketDescriptor);

public:
    explicit FileServer(QObject *parent = nullptr);
    int m_port;

signals:
    void newClientConnected(qintptr socketDescriptor);

};

#endif // FILESERVER_H
