#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<cstdlib>
#include<ctime>
#define inf 0x3f3f3f3f
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    image = QImage(600,300,QImage::Format_RGB32);  //�����ĳ�ʼ����С��Ϊ600*500��ʹ��32λ��ɫ
    QColor backColor = qRgb(255,255,255);    //������ʼ������ɫʹ�ð�ɫ
    image.fill(backColor);//�Ի����������

    Paint();
}

void MainWindow::Paint()
{
    QPainter painter(&image);
    painter.setRenderHint(QPainter::Antialiasing, true);//���÷����ģʽ���ÿ�һ��


    int pointx=35,pointy=280;//ȷ��������������꣬���ﶨ��(35,280)
    int width=580-pointx,height=260;//ȷ���������ȸ��߶� ���Ķ��廭��Ϊ600X300��������˶�����

    //���������� ������ԭ��(35��280)
    painter.drawRect(5,5,600-10,300-10);//��Χ�ľ��Σ���(5,5)�𣬵�(590,290)��������Χ����5�ļ�϶��

    painter.drawLine(pointx,pointy,width+pointx,pointy);//������x���Ϊwidth
    painter.drawLine(pointx,pointy-height,pointx,pointy);//������y�߶�Ϊheight


    srand(time(NULL));

    //������������ֵ����Сֵ��ƽ����
    const int n=30;//nΪ���ݸ���
    double sum=0;
    double ave=0;
    int _ma=0;//����������ֵ
    int _mi=inf;

    int a[n];//���ݴ���������a�У���СΪn

    for(int i=0;i<n;i++)
        a[i]=rand()%40+20;
    int maxpos=0,minpos=0;
    for(int i=0;i<n;i++)
    {
        sum+=a[i];
        if(a[i]>_ma){
            _ma=a[i];
            maxpos=i;
        }
        if(a[i]<_mi){
            _mi=a[i];
            minpos=i;
        }
    }
    ave=sum/n;//ƽ����

    double kx=(double)width/(n-1); //x���ϵ��
    double ky=(double)height/_ma;//y����ı���ϵ��
    QPen pen,penPoint;
    pen.setColor(Qt::black);
    pen.setWidth(2);

    penPoint.setColor(Qt::blue);
    penPoint.setWidth(5);
    for(int i=0;i<n-1;i++)
    {
        //����y���ǵ��ŵģ�����y������Ҫpointy-a[i]*ky ����kyΪ����ϵ��
        painter.setPen(pen);//��ɫ����������
        painter.drawLine(pointx+kx*i,pointy-a[i]*ky,pointx+kx*(i+1),pointy-a[i+1]*ky);
        painter.setPen(penPoint);//��ɫ�ıʣ����ڱ�Ǹ�����
        painter.drawPoint(pointx+kx*i,pointy-a[i]*ky);
    }
    painter.drawPoint(pointx+kx*(n-1),pointy-a[n-1]*ky);//�������һ����

    //����ƽ����
    QPen penAve;
    penAve.setColor(Qt::red);//ѡ���ɫ
    penAve.setWidth(2);
    penAve.setStyle(Qt::DotLine);//��������Ϊ����
    painter.setPen(penAve);
    painter.drawLine(pointx,pointy-ave*ky,pointx+width,pointy-ave*ky);

    //�������ֵ����Сֵ
    QPen penMaxMin;
    penMaxMin.setColor(Qt::darkGreen);//����ɫ
    painter.setPen(penMaxMin);
    painter.drawText(pointx+kx*maxpos-kx,pointy-a[maxpos]*ky-5,QString::fromLocal8Bit("���ֵ")+QString::number(_ma));
    painter.drawText(pointx+kx*minpos-kx,pointy-a[minpos]*ky+15,QString::fromLocal8Bit("��Сֵ")+QString::number(_mi));

    penMaxMin.setColor(Qt::red);
    penMaxMin.setWidth(7);
    painter.setPen(penMaxMin);
    painter.drawPoint(pointx+kx*maxpos,pointy-a[maxpos]*ky);//������ֵ��
    painter.drawPoint(pointx+kx*minpos,pointy-a[minpos]*ky);//�����Сֵ��


    //���ƿ̶���
    QPen penDegree;
    penDegree.setColor(Qt::black);
    penDegree.setWidth(2);
    painter.setPen(penDegree);
    //����x��̶���
    for(int i=0;i<10;i++)//�ֳ�10��
    {
        //ѡȡ���ʵ����꣬����һ�γ���Ϊ4��ֱ�ߣ����ڱ�ʾ�̶�
        painter.drawLine(pointx+(i+1)*width/10,pointy,pointx+(i+1)*width/10,pointy+4);
        painter.drawText(pointx+(i+0.65)*width/10,
                         pointy+10,QString::number((int)((i+1)*((double)n/10))));
    }
    //y��̶���
    double _maStep=(double)_ma/10;//y��̶ȼ����������ֵ����ʾ
    for(int i=0;i<10;i++)
    {
        //����ϳ����������ջ���ԭ���ɡ�
        //��Ҫ����ȷ��һ��λ�ã�Ȼ��һ���̶̵�ֱ�߱�ʾ�̶ȡ�
        painter.drawLine(pointx,pointy-(i+1)*height/10,
                         pointx-4,pointy-(i+1)*height/10);
        painter.drawText(pointx-20,pointy-(i+0.85)*height/10,
                         QString::number((int)(_maStep*(i+1))));
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
