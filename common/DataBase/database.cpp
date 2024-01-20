#include "database.h"
#include <QCoreApplication>
#include "appcfg.h"
DataBase* DataBase::m_instance = NULL;

DataBase::DataBase(QObject *parent) : QObject(parent)
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(QCoreApplication::applicationDirPath()+"/data.db3");    //如果本目录下没有该文件,则会在本目录下生成,否则连接该文件
    if (!db.open()) {
        AppCfg::getInstance()->fileLogWrite("db.open() ERR:" + QCoreApplication::applicationDirPath()+"/data.db3");
        return;
    }
    else
      query = QSqlQuery(db);

     AppCfg::getInstance()->fileLogWrite("db.open() ok:" + QCoreApplication::applicationDirPath()+"/data.db3");

    // 判断表是否有 用户表
    QString strSql = " SELECT * FROM sqlite_master WHERE type='table' AND name = '用户表';";
    QSqlQuery query;
    query.exec(strSql);
    if (!query.next())
    {
        qDebug()<<"DataBase ERR";
        qDebug() << "[SQL] QUERY = " << strSql << ", ERROR = " << query.lastError().text();

        query.exec("CREATE TABLE 用户表 ("
                           "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                           "user VARCHAR(40) NOT NULL, "
                           "pwd VARCHAR(40) NOT NULL)");

        query.exec("CREATE TABLE 日志表 ("
                           "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                           "date VARCHAR(40) NOT NULL, "
                           "content VARCHAR(200) NOT NULL)");

        query.exec("CREATE TABLE 文件传输记录表 ("
                           "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                           "date VARCHAR(40) NOT NULL, "
                           "type VARCHAR(40) NOT NULL, "
                           "tagetName VARCHAR(200) NOT NULL, "
                           "file VARCHAR(200) NOT NULL,"
                           "fileSize VARCHAR(40) NOT NULL,"
                           "local VARCHAR(200) NOT NULL)");
    }


}


DataBase::~DataBase()
{
    if(db.isOpen())
    {
        db.close();
    }
}

bool DataBase::isConnect()
{
    if(db.isOpen())
    {
        return true;
    }
    else if(db.open())
    {
        return true;
    }

    return false;
}

