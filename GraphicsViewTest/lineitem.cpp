#include "lineitem.h"
#include <QPainter>
#include <QTimer>

LineItem::LineItem(QGraphicsItem * parent)
    : QGraphicsLineItem(parent)
    , m_timer(new QTimer)
{
    connect(m_timer, SIGNAL(timeout()), this, SLOT(timeoutSlot()));
    m_timer->start(100);
}
LineItem::~LineItem()
{
}
void LineItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
    QGraphicsLineItem::paint(painter, option, widget);
    QPen pen = painter->pen();
    pen.setColor(QColor(0,255,0,255));
    painter->setPen(pen);
    painter->drawLine(m_line);
}
void LineItem::timeoutSlot()
{
    m_line.setP1(line().p1());
    m_line.setP2(m_line.p2()+QPointF(5,5));
    if(m_line.length() >= line().length())
    {
        m_line.setP2(line().p1());
    }
    update();
}
