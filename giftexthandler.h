#ifndef GIFTEXTHANDLER_H
#define GIFTEXTHANDLER_H

#include <QObject>
#include <QQuickTextDocument>
#include <QTextDocument>
#include <QHash>
#include <QMovie>
#include <QTimer>

class GifTextHandler : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QQuickTextDocument *document READ document  WRITE setDocument NOTIFY documentChanged)
    Q_PROPERTY(int cursorStart MEMBER m_cursorStart )
    Q_PROPERTY(int cursorEnd MEMBER m_cursorEnd )
    Q_PROPERTY(int width MEMBER m_width )
    Q_PROPERTY(int height MEMBER m_height )

public:
    explicit GifTextHandler(QObject *parent = nullptr);
    ~GifTextHandler();
    Q_INVOKABLE void inset(QString fileName);
    Q_INVOKABLE void debug();
    Q_INVOKABLE QString coding();
    Q_INVOKABLE void encoding(QString text);

    QQuickTextDocument *document();
    void setDocument(QQuickTextDocument *document);

public slots:
    void onMoveUpdate();

signals:
    void documentChanged();
    void needUpdate();

private:
    void addMovie(QString fileName);

    QQuickTextDocument *m_documnt;
    int m_cursorStart;
    int m_cursorEnd;
    int m_width;
    int m_height;
    QHash<QString, QMovie*> m_movies;
    QTimer m_moveTimer;
};

#endif // GIFTEXTHANDLER_H
