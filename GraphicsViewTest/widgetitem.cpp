#include "widgetitem.h"
#include <QPainter>
#include <QTimer>
#include <QTableWidget>

WidgetItem::WidgetItem()
    : m_timer(new QTimer)
{
    QTableWidget* tableWidget = new QTableWidget;
    tableWidget->setRowCount(1);
    tableWidget->setColumnCount(2);
    setWidget(tableWidget);
    m_rect.setRect(0,0,0,0);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(timeoutSlot()));
    m_timer->start(100);
}

void WidgetItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
    QGraphicsProxyWidget::paint(painter, option, widget);
    QPen pen = painter->pen();
    pen.setColor(QColor(0,255,0));
    painter->setPen(pen);
    painter->setBrush(QBrush(QColor(0,255,0,100)));
    painter->drawRect(m_rect);
}
void WidgetItem::timeoutSlot()
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
