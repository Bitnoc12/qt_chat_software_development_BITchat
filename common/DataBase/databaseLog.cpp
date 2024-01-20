#include "database.h"
#include <QDebug>
#include <QDateTime>

void DataBase::insertLog(QString log)
{
    QMutexLocker lock(&mutex);

    QString current_date =QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm");

    QString cmd = "INSERT INTO 日志表(date, content)"
                   "VALUES ('"+current_date+"', '"+log+"')";

    qDebug()<<__FILE__<<__FUNCTION__<<log<<cmd;
    if (!query.exec(cmd)) {
        qDebug() << "[SQL] QUERY = " << cmd << ", ERROR = " << query.lastError().text();
    }

}

void DataBase::insetFinishFileLog(  QString date,
                                      QString type,
                                      QString tagetNmae,
                                      QString file,
                                      QString fileSize,
                                      QString local)
{
   QMutexLocker lock(&mutex);

   QString cmd = "INSERT INTO 文件传输记录表(date, type, tagetName, file, fileSize, local)"
                  "VALUES ('"+date+"', '"+type+"','"+tagetNmae+"','"+file+"', '"+fileSize+"', '"+local+"')";

   qDebug()<<__FILE__<<__FUNCTION__<<cmd;
   if (!query.exec(cmd)) {
       qDebug() << "[SQL] QUERY = " << cmd << ", ERROR = " << query.lastError().text();
   }
}

void DataBase::deleteFinishFileLog(int id)
{

    QMutexLocker lock(&mutex);
    QString cmd = "DELETE FROM 文件传输记录表 WHERE  id = "+QString("%1").arg(id)+" ";

    qDebug()<<__FILE__<<__FUNCTION__<<cmd;
    if (!query.exec(cmd)) {
        qDebug() << "[SQL] QUERY = " << cmd << ", ERROR = " << query.lastError().text();
    }
}

QVariantList DataBase::finishFileLogQueryAll()
{
    QVariantList result;
    QMutexLocker lock(&mutex);
    QString cmd = "SELECT * from 文件传输记录表 ORDER BY date DESC;";
    if (query.exec(cmd)) {
        while(query.next()) {
            QVariantMap record;
            record["id"] = query.value("id").toInt();
            record["date"] = query.value("date").toString();
            record["type"] = query.value("type").toString();
            record["tagetName"] = query.value("tagetName").toString();
            record["file"] = query.value("file").toString();
            record["fileSize"] = query.value("fileSize").toString();
            record["local"] = query.value("local").toString();
            result.append(record);
        }
    } else {
        qDebug() << "[SQL] QUERY = " << cmd << ", ERROR = " << query.lastError().text();
    }
    return result;
}
