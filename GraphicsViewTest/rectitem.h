#ifndef RECTITEM_H
#define RECTITEM_H

#include <QObject>
#include <QGraphicsRectItem>

class QTimer;
class RectItem : public QObject, public QGraphicsRectItem
{
    Q_OBJECT
public:
    RectItem();
protected:
    virtual void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0);
private slots:
    void timeoutSlot();
private:
    QTimer* m_timer;
    QRectF  m_rect;
};

#endif // RECTITEM_H
