#ifndef WindowMove_H
#define WindowMove_H

#include <QQuickItem>
#include <QQuickWindow>
#include <QSize>
class WindowMove : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(QQuickWindow* target WRITE setTarget)
public:
    explicit WindowMove(QQuickItem *parent = nullptr);
    void setTarget(QQuickWindow* target);
    QQuickWindow* target();
protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    bool eventFilter(QObject *obj, QEvent *evt) override;


signals:

private:
    QQuickWindow* m_target;
    QPoint m_globalPressPos;    // 鼠标按下的全局坐标
    QPoint m_pressPos;          // 鼠标按下的应用程序内的坐标
    bool m_move;
};

#endif
