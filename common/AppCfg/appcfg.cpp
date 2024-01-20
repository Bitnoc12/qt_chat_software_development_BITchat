#include "appcfg.h"
#include <QXmlStreamWriter>
#include <QImage>
#include <QPixmap>
#include <QApplication>
#include <QDesktopWidget>

QString AppCfg::m_xmlFileName = "cfg.xml";
QString AppCfg::m_headFileName = "myhead.jpg";
QString AppCfg::m_headSmallFileName = "myhead_small.jpg";
AppCfg* AppCfg::m_instance = NULL;
AppCfg::AppCfg(QObject *parent) : QObject(parent),
    m_metaState(QMetaEnum::fromType<AppCfg::XmlName>())
{

    initXmlList();
    readXmlFile();

}

AppCfg::~AppCfg()
{
    qDebug()<<"~AppCfg : writeXmlFile";
    writeXmlFile();
    fileLogSave();
}

void AppCfg::initXmlList()
{
    attrList.clear();
    attrList<<XmlAttribute(XmlName::CurrentUser, &m_currentUser, "");
    attrList<<XmlAttribute(XmlName::HeadImage,&m_headImage, "qrc:/head/head.jpg");
    attrList<<XmlAttribute(XmlName::HeadImageSmall,&m_headImageSmall,  "qrc:/head/head_small.jpg");
    attrList<<XmlAttribute(XmlName::FileOpenUrl,&m_fileOpenUrl, "");
    attrList<<XmlAttribute(XmlName::SkinIndex, &m_skinIndex, "3");
    attrList<<XmlAttribute(XmlName::BubbleIndex,&m_bubbleIndex,  "0");
    attrList<<XmlAttribute(XmlName::SkinOpacity,&m_skinOpacity,  "1.0");



    QDir  dir(QCoreApplication::applicationDirPath() +"/friend//");
    if (!dir.exists()) {
        qDebug()<<"MKDIR: "<<dir.path()<<dir.mkdir(dir.path());
    }
    attrList<<XmlAttribute(XmlName::FriendHeadDir,&m_friendHeadDir,
        QUrl::fromLocalFile(QCoreApplication::applicationDirPath() +"/friend//").toString());

}

QString AppCfg::read(XmlName name)
{
    QString ret("");

    foreach (XmlAttribute attr, attrList)
    if(attr.name == name) {
        ret = *attr.property;
    }
    return ret;
}

bool AppCfg::write(XmlName name,QString value)
{
    bool ret(false);

    for(int i =0; i<attrList.count(); i++)
    if(attrList[i].name == name)
    {
        qDebug()<<"write:"<<m_metaState.valueToKey(name)<<value;
        if (*attrList[i].property != value) {
            *attrList[i].property = value;
            ret =true;
            *attrList[i].property = value;
            emit writeUpdate(name);
        }
        break;
    }

    return ret;
}

bool AppCfg::readXmlFile()
{
    QFile  file(QCoreApplication::applicationDirPath() +"//"+ m_xmlFileName);
    qDebug()<<"readXmlFile"<<file.size();
    QString defualtFriendHeadDir = read(XmlName::FriendHeadDir);
    // 打开失败 那就是都是默认值
    if(!file.open(QFile::ReadOnly | QFile::Text)) {
        file.close();
        writeXmlFile();
        return false;
    }

    QXmlStreamReader* reader=new QXmlStreamReader(&file);

     while(!reader->atEnd()) {

        QString name = reader->name().toString();

        if(reader->isStartElement()) {
            if(name=="index") {
                reader->readNext();
            } else {
                bool isOk = false;
                XmlName xmlName = (XmlName)m_metaState.keyToValue(name.toLocal8Bit(), &isOk); // 字符串转枚举
                if (!isOk) {
                    qDebug()<<"readXmlFile !isOk "<<name;
                }
                write(xmlName, reader->readElementText());
            }
        }

        if(reader->isEndElement()) {

        }
        reader->readNext();
    }

    file.close();
    if(reader->hasError() || defualtFriendHeadDir != read(XmlName::FriendHeadDir))
    {
       qDebug()<<"readXml ERR:"<<reader->errorString();
       initXmlList();
       writeXmlFile();
       return false;
    }

    return true;
}


bool AppCfg::updateHeadImage(QUrl url)     //  更新头像文件
{
    QImage image(url.toLocalFile());
    if (image.isNull()) {
        qDebug()<<"updateHeadImage err";
        return false;
    }
    // 大图
    image = image.scaled(QSize(100,100), Qt::KeepAspectRatio,Qt::SmoothTransformation);
    url = QUrl::fromLocalFile(QCoreApplication::applicationDirPath()+"//"+ m_headFileName);
    qDebug()<<"save:"<<image.save(url.toLocalFile());
    m_headImage = "";
    emit headImageChanged();
    m_headImage = url.toString();
    emit headImageChanged();

    // 小图
    image = image.scaled(QSize(40,40), Qt::KeepAspectRatio,Qt::SmoothTransformation);
    url = QUrl::fromLocalFile(QCoreApplication::applicationDirPath()+"//"+ m_headSmallFileName);
    qDebug()<<"save:"<<image.save(url.toLocalFile());
    m_headImageSmall = "";
    emit headImageSmallChanged();
    m_headImageSmall = url.toString();
    emit headImageSmallChanged();



    return true;
}



bool AppCfg::writeXmlFile()
{
    QFile  file(QCoreApplication::applicationDirPath() +"//"+ m_xmlFileName);
    QFileInfo info(file);
    qDebug()<<"AppCfg : writeXmlFile";
    if(!file.open(QFile::WriteOnly | QFile::Text))
    {
        qDebug()<<"file write error";
        return false;
    }

    QXmlStreamWriter* writer=new QXmlStreamWriter(&file);
    QXmlStreamAttributes attributes;

    writer->setCodec("utf-8");
    writer->setAutoFormatting(true);
    writer->writeStartDocument();
    writer->writeStartElement("index");

    writer->writeComment("配置");

    for(int i = (int)XmlName::Start + 1; i<(int)XmlName::End; i++)
    {
         QString name = m_metaState.valueToKey(i);   // 枚举转字符串
         QString value = read((XmlName)i);
         writer->writeTextElement(name,value);
    }


    writer->writeComment("配置完成");
    writer->writeEndElement();
    writer->writeEndDocument();

    file.close();
    return true;
}
