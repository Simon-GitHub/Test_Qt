#include "rectitem.h"
#include <QPainter>
#include <QTimer>

RectItem::RectItem()
    : m_timer(new QTimer)
{
    m_rect.setRect(0,0,0,0);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(timeoutSlot()));
    m_timer->start(100);
}
void RectItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
    QGraphicsRectItem::paint(painter, option, widget);
    QPen pen = painter->pen();
    pen.setColor(QColor(0,255,0));
    painter->setPen(pen);
    painter->setBrush(QBrush(QColor(0,255,0,100)));
    painter->drawRect(m_rect);
}
void RectItem::timeoutSlot()
{
#if 0
    m_rect.setTopLeft(rect().topLeft());
    m_rect.setWidth(rect().width());
    m_rect.setHeight(m_rect.height()+5);
    if(m_rect.height() >  rect().height())
    {
        m_rect.setHeight(0);
    }
#else
    if(m_rect == QRectF(0,0,0,0))
    {
        m_rect.setTopLeft(rect().bottomLeft());
        m_rect.setBottomRight(rect().bottomRight());
    }
    m_rect.setTopLeft(QPointF(rect().left(),m_rect.top()-5));
    m_rect.setBottomRight(rect().bottomRight());
    if(m_rect.height() >  rect().height())
    {
        m_rect.setTopLeft(rect().bottomLeft());
        m_rect.setBottomRight(rect().bottomRight());
    }
#endif
    update();
}

