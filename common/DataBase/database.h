#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMutex>
#include <QMutexLocker>
#include <QSqlError>
#include <QVariantList>
#include <QDebug>

class DataBase : public QObject
{
    Q_OBJECT


public:
    explicit DataBase(QObject *parent = nullptr);

    ~DataBase();
    static DataBase* instance()
    {
        if(m_instance==NULL)
           m_instance = new DataBase();
        return m_instance;
    }

    Q_INVOKABLE bool isConnect();


    // user
    Q_INVOKABLE bool userQuery(QString user, QString pwd);


    // log
    Q_INVOKABLE void insertLog(QString log);

    void insetFinishFileLog(QString date,
                              QString type,
                              QString tagetNmae,
                              QString file,
                              QString fileSize,
                              QString local);

    QVariantList finishFileLogQueryAll();
    Q_INVOKABLE void deleteFinishFileLog(int id);


signals:

protected:
    QSqlDatabase db;
    QSqlQuery   query;
    QMutex      mutex;
private:
    static DataBase *m_instance;
};

#endif // DATABASE_H
