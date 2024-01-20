#ifndef FRAMELESSWINDOW_H
#define FRAMELESSWINDOW_H

#include <QQuickWindow>
#include <QQuickItem>
#include <QSize>
#include <QList>
#include <QMouseEvent>
class FramelessWindow : public QQuickWindow
{
    Q_OBJECT
    Q_PROPERTY(bool resizable MEMBER m_resizable)
    Q_PROPERTY(QList<QRect> moveArea MEMBER m_moveArea)
    Q_PROPERTY(bool maximized READ maximized WRITE setMaximized NOTIFY maximizedChanged)
    // 鼠标处理事件 对应界面:
    //   TopLeft    ---  Top   ---  TopRight
    //   Left       ---  Move  ---  Right
    //   BottomLeft --- Bottom ---  BottomRight
    enum MouseEvent {
        Mouse_None  = 0,
        Mouse_Pressed ,
        Mouse_TopLeft ,
        Mouse_Top,
        Mouse_TopRight,
        Mouse_Left,
        Mouse_Move,
        Mouse_Right,
        Mouse_BottomLeft,
        Mouse_Bottom,
        Mouse_BottomRight,
    };

public:
    explicit FramelessWindow(QWindow *parent = nullptr);

    ~FramelessWindow();
    bool maximized();
    void setMaximized(bool max);


protected:
    bool eventFilter(QObject *obj, QEvent *evt) override;


signals:
    void resizeUpdate(QSize size);
    void maximizedChanged();

private:
    MouseEvent getMousePressEvent(const QPoint &pos);
    MouseEvent getResizeArea(const QPoint &pos);
    void setWindowGeometry(const QPoint &pos);
    void setCursorEvent(MouseEvent event, bool isRefrensh = true);
    void setGeometryCalc(const QSize &size, const QPoint &pos);

    bool m_resizable;
    MouseEvent m_event;
    QPoint m_globalPressPos;
    QPoint m_pressPos;
    bool m_move;
    QSize  m_preSize;
    QList<QRect> m_moveArea;
};



#endif
