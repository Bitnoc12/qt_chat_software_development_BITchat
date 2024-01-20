#ifndef FILETASK_H
#define FILETASK_H

#include <QRunnable>
#include <QThread>
#include <QThreadPool>
#include <QDateTime>
#include <QDebug>

class FileTask : public QRunnable
{

public:
    FileTask() {
        m_id = QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss");
        setAutoDelete(false);
    }
    virtual ~FileTask() {

    }

    virtual void cancelTask() {
        m_isCancel = true;
    }

    virtual bool isCancel() {
        return m_isCancel;
    }


    virtual QString getId() {
        return m_id;
    }

    virtual QString getStatus() {
        return m_status;
    }
    virtual QString getFileName() {
        return m_fileName;
    }
    virtual QString getFileUrl() {
        return m_fileUrl;
    }

    virtual qint64 getFileSize() {
        return m_fileSize;
    }

    virtual QString getTagetNmae() {
        return m_tagetNmae;
    }
    virtual qint64 getCurrentByte() {
        return m_currentByte;
    }

    virtual qint64 getSpeedByte() {
        return m_speed;
    }
    virtual bool getRunniing() {
        return m_running;
    }

protected:
    QString m_id = "";
    bool m_isCancel = false;
    QString m_status = "等待传输";
    QString m_fileName = "未知";
    QString m_fileUrl = "未知";
    qint64  m_fileSize = -1;
    qintptr m_socketDescriptor;
    QString m_tagetNmae = "未知";
    qint64 m_currentByte = -1;
    qint64 m_speed = 0;
    qint64 m_payloadSize = 32*1024;
    qint64 m_buffSize = 1024*1024;
    bool m_running = true;

    bool updateSpeed(qint64& preCrtByte, qint64& preMSec) {

        qint64 currentMSec = QDateTime::currentDateTime().toMSecsSinceEpoch();
        qint64 intevalMSec =  currentMSec - preMSec;

        if(intevalMSec < 1000) {
            return false;
        }

        qint64 intevalByte = m_currentByte  - preCrtByte;
        m_speed = intevalByte * 1000.0 / intevalMSec;

        preMSec = currentMSec;
        preCrtByte = m_currentByte;
        return true;
    }
};



#endif // FILETASK_H
