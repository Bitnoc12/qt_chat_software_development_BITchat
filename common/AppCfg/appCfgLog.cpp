#include "appcfg.h"
#include <QDateTime>

void AppCfg::fileLogWrite(QString text)
{
    text = QDateTime::currentDateTime().toString("[yyyy/MM/dd hh:mm:ss dddd] ")+text+"\r\n";
    fileLog.append(text);

    if(fileLog.length()>=600)
    {
        fileLogSave();
    }


}
void AppCfg::fileLogSave()
{

    if(fileLog.length()==0)    return;

    QDir dir(QCoreApplication::applicationDirPath()+"/日志记录");
    if(!dir.exists()){
         dir.mkdir(QCoreApplication::applicationDirPath()+"/日志记录");

    }

    QFile file(QCoreApplication::applicationDirPath()+"/日志记录/"+QDateTime::currentDateTime().toString("yyyy-MM-dd")+".txt");

    if(file.open(QFile::Append | QFile::Text))
    {

        file.write(fileLog.toLocal8Bit());
        fileLog.clear();
        file.close();
    }

}
