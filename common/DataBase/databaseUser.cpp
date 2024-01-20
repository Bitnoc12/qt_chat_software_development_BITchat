#include "database.h"
#include <QDebug>


bool DataBase::userQuery(QString user, QString pwd)
{
    QMutexLocker lock(&mutex);

    QString cmd = "SELECT * FROM 用户表 WHERE user == '"+user+"' AND pwd == '"+pwd+"';";
    if (query.exec(cmd)) {
        if (query.next()) {

            return true;

        }
    } else {
        qDebug() << "[SQL] QUERY = " << cmd << ", ERROR = " << query.lastError().text();
    }

    return false;
}
