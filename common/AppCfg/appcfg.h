#ifndef APPCFG_H
#define APPCFG_H
#include <QQmlEngine>
#include <QObject>
#include <QMetaEnum>
#include <QString>
#include <QList>
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QCoreApplication>

class AppCfg : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString currentUser MEMBER m_currentUser NOTIFY currentUserChanged)
    Q_PROPERTY(QString fileOpenUrl MEMBER m_fileOpenUrl NOTIFY fileOpenUrlChanged)
    Q_PROPERTY(QString headImage MEMBER m_headImage NOTIFY headImageChanged)
    Q_PROPERTY(QString skinIndex MEMBER m_skinIndex NOTIFY skinIndexChanged)
    Q_PROPERTY(QString bubbleIndex MEMBER m_bubbleIndex NOTIFY bubbleIndexChanged)
    Q_PROPERTY(QString skinOpacity MEMBER m_skinOpacity NOTIFY skinOpacityChanged)
    Q_PROPERTY(QString friendHeadDir MEMBER m_friendHeadDir NOTIFY friendHeadDirChanged)
    Q_PROPERTY(QString headImageSmall MEMBER m_headImageSmall NOTIFY headImageSmallChanged)

private:
    QString m_currentUser;
    QString m_fileOpenUrl;
    QString m_headImage;
    QString m_skinIndex;
    QString m_bubbleIndex;
    QString m_skinOpacity;
    QString m_friendHeadDir;
    QString m_headImageSmall;
signals:
    void currentUserChanged();
    void fileOpenUrlChanged();
    void headImageChanged();
    void skinIndexChanged();
    void bubbleIndexChanged();
    void skinOpacityChanged();
    void friendHeadDirChanged();
    void headImageSmallChanged();

public:
    enum XmlName{
        Start  =0,
        CurrentUser ,       // 用户名
        HeadImage,          // 自己头像文件(qml格式 前面加了"file:///"或者"qrc" )
        FileOpenUrl,        // 文件打开的URL路径
        SkinIndex,
        BubbleIndex,
        SkinOpacity,
        FriendHeadDir,
        HeadImageSmall,
        ScreenCenterX,
        ScreenCenterY,
        End
    };

    Q_ENUM(XmlName)

    explicit AppCfg(QObject *parent = nullptr);
    ~AppCfg();

    Q_INVOKABLE bool writeXmlFile();
    Q_INVOKABLE bool updateHeadImage(QUrl url);

    QString read(XmlName name);
    bool write(XmlName name,QString value);


//fileLog日志
    void fileLogWrite(QString text);
    void fileLogSave();


    static AppCfg* getInstance()
   {
        if(m_instance==NULL)
           m_instance= new AppCfg();
        return m_instance;
   }

private:
    static AppCfg *m_instance;
    class XmlAttribute{
    public:
           XmlName  name;
           QString* property;

           XmlAttribute(XmlName name,  QString* property, QString value)
           {
               this->name = name;
               this->property = property;
               *this->property = value;
           }
    };

    bool readXmlFile();
    void initXmlList();

private:
    static QString m_xmlFileName;
    static QString m_headFileName;
    static QString m_headSmallFileName;
    QString m_skincolor;
    QList<XmlAttribute> attrList;
    QMetaEnum m_metaState;
    QString fileLog;

signals:
    void writeUpdate(XmlName name);

};

// 定义一个回调类指针,用于接收回调.
static QObject *appCfg_qobject_singletontype_provider(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    //Q_UNUSED: 向编译器指示参数未在函数的主体中使用。这可用于抑制编译器警告
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)

    return AppCfg::getInstance();
}


#endif // AppCfg_H
