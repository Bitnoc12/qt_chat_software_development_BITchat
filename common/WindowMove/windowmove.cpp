#include "windowmove.h"
#include <QDebug>
WindowMove::WindowMove(QQuickItem *parent)
    : QQuickItem (parent),
      m_target(nullptr)
{

    QCoreApplication::instance()->installEventFilter(this); // 给自己加事件过滤器

}



void WindowMove::setTarget(QQuickWindow* target)
{
    qDebug()<<"setTarget"<<this<<target;
    m_target = target;
    this->installEventFilter(target);
}

QQuickWindow* WindowMove::target()
{
    return m_target;
}


bool WindowMove::eventFilter(QObject *obj, QEvent *evt)
{
    QMouseEvent *mouse =  dynamic_cast<QMouseEvent *>(evt);
    qDebug()<<"WindowMove"<<evt;
    return QQuickItem::eventFilter(obj,evt);
}

void WindowMove::mousePressEvent(QMouseEvent *event)
{
    m_globalPressPos = event->globalPos();
    m_pressPos = QPoint((int)position().x(), (int)position().y());
    m_move = true;
    qDebug()<<"WindowMove mousePressEvent"<<m_pressPos<<m_move;
    QQuickItem::mousePressEvent(event);
}

void WindowMove::mouseReleaseEvent(QMouseEvent *event)
{
    qDebug()<<"WindowMove mouseReleaseEvent"<<m_pressPos;
    m_move = false;
    QQuickItem::mouseReleaseEvent(event);
}

void WindowMove::mouseMoveEvent(QMouseEvent *event)
{
    qDebug()<<m_pressPos  + event->globalPos() - m_globalPressPos<<m_target->position();
    if (m_target && event->buttons() & Qt::LeftButton) {    // 按下鼠标移动中
       m_target->setPosition(m_pressPos  + event->globalPos() - m_globalPressPos);
    }
    QQuickItem::mouseMoveEvent(event);
}


