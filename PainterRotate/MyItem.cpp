#include "MyItem.h"
#include <QStyle>

MyItem::MyItem(qreal wid,qreal hgt)  {
    m_width=wid;m_height=hgt;
    m_Rotation=0;
    setZValue(0);
    setAcceptDrops(true);
    setFlags(ItemIsSelectable | ItemIsMovable);
    m_pm=NULL;
    paintPic();
}

QString MyItem::Cardinate(){
    QPointF lt=this->mapToScene(-m_width/2,-m_height/2);
    QPointF lb=this->mapToScene(-m_width/2,m_height/2);
    QPointF rt=this->mapToScene(m_width/2,-m_height/2);
    QPointF rb=this->mapToScene(m_width/2,m_height/2);

    QString _lt=QString("Left-Top:")+"("+QString::number(lt.x())+","+QString::number(lt.y())+");\n";
    QString _lb=QString("Left-Bottom:")+"("+QString::number(lb.x())+","+QString::number(lb.y())+");\n";
    QString _rt=QString("Right-Top:")+"("+QString::number(rt.x())+","+QString::number(rt.y())+");\n";
    QString _rb=QString("Right-Bottom:")+"("+QString::number(rb.x())+","+QString::number(rb.y())+");\n";

    return QString(_lt+_lb+_rt+_rb);
}

void MyItem::paintPic(){
    m_pm=new QPixmap(m_width/2,m_height/2);
    QPainter painter(m_pm);
    painter.setPen(Qt::gray);
    m_pm->fill();
    painter.drawText(10,10,"TEST_TOO");
    painter.drawRect(1,1,m_width/2-2,m_height/2-2);
}
QRectF MyItem::boundingRect()const{
    qreal max=(m_width>m_height)?m_width:m_height;
    return QRectF(-(max+1)/2,-(max+1)/2,max,max);//每个item都有自己的一个坐标系，
//这个设置相当于把item相对于自身的坐标系的原点（0,0）放到自己的正中央。
}
QPainterPath MyItem::shape()const{
    QPainterPath path;
    path.addRect(QRectF(-m_width/2,-m_height/2,m_width,m_height));
    return path;
}
void MyItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    //Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->rotate(m_Rotation);
    painter->drawText(0,0,QString("This is a test!"));
    //painter->rotate(-m_Rotation);
    painter->drawRect(QRectF(-m_width/2+2,-m_height/2+2,m_width-4,m_height-4));
    painter->drawPixmap(-m_width/4,-m_height/4,m_pm->width(),m_pm->height(),*m_pm);
    if(option->state & QStyle::State_Selected){
            painter->setPen(QPen(Qt::red,2,Qt::DotLine));
            painter->drawRect(QRectF(-m_width/2,-m_height/2,m_width,m_height));
    }
    update();
}

void MyItem::Rotate(){
    m_Rotation+=90;
    if(m_Rotation==360)m_Rotation=0;
}
