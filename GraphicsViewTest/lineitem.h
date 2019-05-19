#ifndef LINEITEM_H
#define LINEITEM_H

#include <QObject>
#include <QGraphicsLineItem>

class QTimer;
class LineItem : public QObject, public QGraphicsLineItem
{
    Q_OBJECT
public:
    LineItem(QGraphicsItem * parent = 0);
    virtual ~LineItem();
protected:
    virtual void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0);
private slots:
    void timeoutSlot();
private:
    QTimer* m_timer;
    QLineF  m_line;
};

#endif // LINEITEM_H
