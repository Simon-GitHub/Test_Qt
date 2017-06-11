#include "ProgressBar.h"

CProgressBar::CProgressBar(QWidget *parent) :
        QWidget(parent)
{
    m_nMax =100;
    m_nMin =0;
    this->initVariables();
    this->setMaximumHeight(35);

}


void CProgressBar::initVariables()
{
    m_nValue = 0;
}

void CProgressBar::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing);
    drawBg(&painter);
    draw(&painter);
}


void CProgressBar::drawBg(QPainter *painter)
{
    painter->save();
    painter->setPen(Qt::NoPen);
    painter->setBrush(PYV_BG_COLOR);
    painter->drawRect(rect());
    painter->restore();
}

void CProgressBar::draw(QPainter *painter)
{
    painter->save();

    qreal dX = (qreal)(width() - (PYV_BAR_COUNT + 1 ) * PYV_TOP_SPACE)/PYV_BAR_COUNT;

    qreal initX = PYV_TOP_SPACE;
    for(int BarIndex = 0;BarIndex < PYV_BAR_COUNT;BarIndex++)
    {
       QPointF BarTopLeft(initX,PYV_TOP_SPACE);
       //qDebug()<<"BarTopLeft"<<BarTopLeft;
       QPointF BarBottomRight(BarTopLeft.x()+dX ,height()-PYV_TOP_SPACE);
      // qDebug()<<"BarBottomRight"<<BarBottomRight;
       QRectF BarRect(BarTopLeft,BarBottomRight);

       QLinearGradient BarGradient(BarRect.topLeft(),BarRect.bottomLeft());
       BarGradient.setColorAt(0.0,PYV_BAR_DEFAULT_END_COLOR);
       BarGradient.setColorAt(0.5,PYV_BAR_DEFAULT_START_COLOR);
       BarGradient.setColorAt(1.0,PYV_BAR_DEFAULT_END_COLOR);

       painter->setPen(Qt::NoPen);
       painter->setOpacity(PYV_OPACITY);
       painter->setBrush(BarGradient);

       painter->drawRoundedRect(BarRect,PYV_RECT_RADIUS,PYV_RECT_RADIUS);

       /// increment initY
       initX += dX + PYV_TOP_SPACE;
    }

    // draw value bar
    painter->save();
    painter->setPen(Qt::NoPen);
    painter->setOpacity(1.0);

    QLinearGradient ValueBarGradient(QPointF(PYV_TOP_SPACE,PYV_TOP_SPACE),QPointF(width()-PYV_TOP_SPACE,height()-PYV_TOP_SPACE));
    ValueBarGradient.setColorAt(0.0,PYV_BAR_START_COLOR);
    ValueBarGradient.setColorAt(0.7,PYV_BAR_MIDDLE_COLOR);
    ValueBarGradient.setColorAt(1.0,PYV_BAR_END_COLOR);
    painter->setBrush(ValueBarGradient);

//    qreal rInitY =  PYV_TOP_SPACE*((PYV_BAR_COUNT - m_nValue) + 1)+(PYV_BAR_COUNT - m_nValue )* dY ;
//    for(int ValueIndex = 0;ValueIndex < m_nValue; ValueIndex++)
//    {
//        QPointF BarTopLeft(PYV_LEFT_SPACE,rInitY);
//        QPointF BarBottomRight(width() - PYV_LEFT_SPACE,rInitY + dY);
//        QRectF BarRect(BarTopLeft,BarBottomRight);
//        painter->drawRoundedRect(BarRect,PYV_RECT_RADIUS,PYV_RECT_RADIUS);

//        /// decrement initY
//        rInitY += dY + PYV_TOP_SPACE;
//    }
    int roundrectcount = m_nValue / 10;

    initX = PYV_TOP_SPACE;
    for(int ValueIndex = 0;ValueIndex < roundrectcount; ValueIndex++)
    {
        QPointF BarTopLeft(initX,PYV_TOP_SPACE);
       // qDebug()<<"BarTopLeft"<<BarTopLeft;
        QPointF BarBottomRight(BarTopLeft.x()+dX ,height()-PYV_TOP_SPACE);
       // qDebug()<<"BarBottomRight"<<BarBottomRight;
        QRectF BarRect(BarTopLeft,BarBottomRight);
        painter->drawRoundedRect(BarRect,PYV_RECT_RADIUS,PYV_RECT_RADIUS);
        initX += dX + PYV_TOP_SPACE;
    }
    if((m_nValue - roundrectcount*10) > 0)
    {
        qreal final = (m_nValue - roundrectcount * 10)/10 * dX;
        QPointF BarTopLeft(initX,PYV_TOP_SPACE);
        QPointF BarBottomRight(BarTopLeft.x()+final ,height()-PYV_TOP_SPACE);
        QRectF BarRect(BarTopLeft,BarBottomRight);
        painter->drawRoundedRect(BarRect,PYV_RECT_RADIUS,PYV_RECT_RADIUS);
    }

    painter->setPen(PYP_TEXT_COLOR);

    QFont TextFont("Times New Roman",18);
    TextFont.setBold(true);
    painter->setFont(TextFont);

    QString strText;

    strText = tr("%1%").arg(m_nValue,2,'f',2);
    //qDebug()<<"strText = "<<strText;
    QPointF TextPos(width()/2-30,height()/2+PYV_LEFT_SPACE);
    painter->drawText(TextPos,strText);
 //   painter->drawText(TextRect,strText,Qt::AlignVCenter|Qt::AlignHCenter);
  //  TextRect.setTopLeft(QPointF(TopLeft.x()/2,TopLeft.y()));
   // TextRect.setBottomRight(QPointF(BottomRight.x()/2,BottomRight.y()/2));

  //  QRectF textRect(QPointF(PYV_TOP_SPACE,width()/2),QPointF(width()/2+50,height()-PYV_TOP_SPACE));

//    qDebug()<<"textRect.topleft"<<textRect.topLeft()<<"textRect.bottomright"<<textRect.bottomRight();
//    painter->drawText(textRect,strText);
    painter->restore();
    painter->restore();
}



void CProgressBar::setValue(double value)
{
    if(value > m_nMax)
    {
        m_nValue = m_nMax;
    }
    else if(value < m_nMin)
    {
        m_nValue = m_nMin;
    }
    else
    {
        m_nValue = value;
    }
    qDebug()<<"m_nValue"<<m_nValue;
    update();
}

double CProgressBar::value() const
{
    return m_nValue;
}

