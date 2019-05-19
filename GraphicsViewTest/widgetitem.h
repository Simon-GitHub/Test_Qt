#ifndef WIDGETITEM_H
#define WIDGETITEM_H

#include <QObject>
#include <QGraphicsProxyWidget>

class QTimer;
class WidgetItem: public QGraphicsProxyWidget
{
    Q_OBJECT
public:
    WidgetItem();
protected:
    virtual void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0);
private slots:
    void timeoutSlot();
private:
    QTimer* m_timer;
    QRectF  m_rect;
};

#endif // WIDGETITEM_H
