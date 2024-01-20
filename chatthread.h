#ifndef CHATTHREAD_H
#define CHATTHREAD_H

#include <QObject>
#include <QThread>
#include <QUdpSocket>
#include <QMutex>
#include <QHostAddress>
#include <QDebug>

class ChatThread : public QThread
{
    Q_OBJECT
public:
    explicit ChatThread(QThread *parent = nullptr);
signals:

};

#endif // CHATTHREAD_H
