#ifndef MYITEM_H
#define MYITEM_H

#include <QGraphicsItem>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QPixmap>
class MyItem : public QGraphicsItem
{
public:
    MyItem(qreal wid,qreal hgt);
    QRectF boundingRect()const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QPainterPath shape()const;
    void Rotate();
    QString Cardinate();
private:
    void paintPic();
private:
    qreal m_width;
    qreal m_height;
    qreal m_Rotation;
    QPixmap* m_pm;
};

#endif // MYITEM_H
