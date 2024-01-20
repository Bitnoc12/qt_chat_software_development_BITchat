#include "giftexthandler.h"
#include <QDebug>
#include <QTextCursor>
#include <QTextBlock>
#include <QRegularExpression>
#include <QRegularExpressionMatch>

static QString GIF_PREFIXDIR = "qrc:/emoji/res/";

GifTextHandler::GifTextHandler(QObject *parent) : QObject(parent),m_documnt(NULL),m_width(28),m_height(28),
    m_moveTimer(this)
{
    connect(&m_moveTimer, SIGNAL(timeout()), this, SLOT(onMoveUpdate()));
    m_moveTimer.start(50);
}


GifTextHandler::~GifTextHandler()
{
    QList<QString> list = m_movies.keys();
    for(int i=0;i<list.length();i++) {
            delete m_movies[list[i]];
    }
    m_movies.clear();
}

QQuickTextDocument* GifTextHandler::document()
{
    return m_documnt;
}

void GifTextHandler::setDocument(QQuickTextDocument *document)
{
    m_documnt = document;
}

QString GifTextHandler::coding()
{
    QString ret("");
    QTextBlock block = m_documnt->textDocument()->firstBlock();
    QVector<QTextFormat> allFormats = m_documnt->textDocument()->allFormats();

    while(block.isValid()) {
         for(QTextBlock::iterator it = block.begin(); !it.atEnd(); it++) {
            QTextCharFormat charFmt = it.fragment().charFormat();
            if (charFmt.objectType() == QTextFormat::ImageObject) {
                ret.append("["+charFmt.property(QTextFormat::ImageName).toString().remove(GIF_PREFIXDIR)+"]");

            } else {
                ret.append(it.fragment().text());
            }
         }
         if (block.next().isValid())
            ret.append("\r\n");

         block = block.next();
    }
    return ret;
}


void GifTextHandler::encoding(QString text)
{
    if (!m_documnt) {
        qDebug()<<"encoding: m_documnt == nullptr";
        return;
    }
    QTextCursor  cursor = QTextCursor(m_documnt->textDocument());
    cursor.setPosition(0);
    QRegularExpression re("\\[\\d+\\.gif\\]");
    QRegularExpressionMatch match;
    int offset = 0;
    int fileNum;
    QString file;
    QString ret;
    while (offset < text.length()) {
        match = re.match(text,offset);
        if (match.hasMatch()) {
            sscanf(match.captured(0).toLocal8Bit(),"[%d.gif]", &fileNum);
            file = QString(GIF_PREFIXDIR+"%1.gif").arg(fileNum);


            cursor.insertText(text.mid(offset, match.capturedStart(0) - offset));
            ret.append(text.mid(offset, match.capturedStart(0) - offset));
            addMovie(file);
            QTextImageFormat imageFormat;
            imageFormat.setName(file);
            imageFormat.setWidth(m_width);
            imageFormat.setHeight(m_height);
            cursor.insertImage(imageFormat, QTextFrameFormat::InFlow);
            offset = match.capturedEnd(0);
        } else {
            ret.append(text.mid(offset, text.length() - offset));
            cursor.insertText(text.mid(offset, text.length() - offset));
            break;
        }
    }
}


void GifTextHandler::debug()
{

    QTextBlock block = m_documnt->textDocument()->firstBlock();
    QVector<QTextFormat> allFormats = m_documnt->textDocument()->allFormats();
    while(block.isValid()) {
         QTextBlockFormat blockFmt = block.blockFormat();
         qDebug()<<"block:"<<block.blockFormat()<<block.text()<<block.charFormat().property(QTextFormat::ImageName);
         for(QTextBlock::iterator it = block.begin(); !it.atEnd(); it++) {
            QTextCharFormat charFmt = it.fragment().charFormat();
            if (charFmt.objectType() == QTextFormat::ImageObject) {
             qDebug()<<"image:"<<it.fragment().position()<<charFmt.property(QTextFormat::ImageName);
            } else {
                qDebug()<<"text:"<<it.fragment().position()<<it.fragment().text();
            }
         }
         block = block.next();
    }

}

void GifTextHandler::inset(QString fileName)
{
    if (!m_documnt)
        return;

    QTextCursor  cursor = QTextCursor(m_documnt->textDocument());
    cursor.setPosition(m_cursorStart);
    qDebug()<<"inset"<<m_cursorStart<<m_cursorEnd;
    if (m_cursorStart > m_cursorEnd) {
        cursor.setPosition(0);
    } else if (m_cursorStart != m_cursorEnd)
        cursor.setPosition(m_cursorEnd, QTextCursor::KeepAnchor);

    addMovie(fileName);
    QTextImageFormat imageFormat;
    imageFormat.setName(fileName);
    imageFormat.setWidth(m_width);
    imageFormat.setHeight(m_height);
    cursor.insertImage(imageFormat, QTextFrameFormat::InFlow);

}


void GifTextHandler::addMovie(QString fileName)
{
    if (m_movies.contains(fileName)) return;

    QMovie *movie = new QMovie(QString(fileName).remove("qrc"));

    movie->setCacheMode(QMovie::CacheNone);
    connect(movie, &QMovie::finished, movie, &QMovie::start);   //循环播放
    movie->start();
    m_movies.insert(fileName, movie);
}


void GifTextHandler::onMoveUpdate()
{
#define CHECK_GIF_CNT   20
    static int cnt = 0;

    if (cnt > CHECK_GIF_CNT) {
        QList<QString> list = m_movies.keys();
        for(int i=0;i<list.length();i++) {
             m_movies[list[i]]->setProperty("status",false);
        }

        QTextBlock block = m_documnt->textDocument()->firstBlock();
        QVector<QTextFormat> allFormats = m_documnt->textDocument()->allFormats();
        while(block.isValid()) {
             QTextBlockFormat blockFmt = block.blockFormat();
             for(QTextBlock::iterator it = block.begin(); !it.atEnd(); it++) {
                QTextCharFormat charFmt = it.fragment().charFormat();
                if (charFmt.objectType() == QTextFormat::ImageObject) {
                    QString file = charFmt.property(QTextFormat::ImageName).toString();
                    if (!m_movies.contains(file)) {
                        addMovie(file);
                    }
                    m_movies[file]->setProperty("status",true);
                }
             }
             block = block.next();
        }

        for(int i=0;i<list.length();i++) {
            if (m_movies[list[i]]->property("status") == false) {
                    delete m_movies[list[i]];
                    m_movies.remove(list[i]);
                    continue;
            }
        }
        cnt = 0;
    } else {
        cnt += 1;
    }


    int begin = 0;
    QList<QString> list = m_movies.keys();
    for(; begin<list.length(); begin++) {
        m_documnt->textDocument()->addResource(QTextDocument::ImageResource, list[begin],
             m_movies[list[begin]]->currentPixmap());
        m_movies[list[begin]]->setProperty("status",false);
    }

    if (begin)
        emit needUpdate();
}
