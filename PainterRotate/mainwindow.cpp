#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    scene=new QGraphicsScene;
    ui->graphicsView->setScene(scene);
    //�ѡ������� ���ҡ���չʾ�Ĵ���QGraphicsView����
    scene->setSceneRect(-500,-500,1000,1000);
    //���á������������������꣨-500��-500��Ϊ���Ͻǣ������ζ��scene������λ��Ϊ(0,0).�������ϵ����scene������ϵ
    ui->graphicsView->centerOn(0,0);//�ѻ���scene������λ�÷ŵ�QGraphicsView������
    item=new MyItem(260,240);
    scene->addItem(item);//�����������ϻ���MyItem
    item->setPos(0,0);
    ui->textBrowser->clear();
    ui->textBrowser->append(item->Cardinate());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_bttn_Rotate_clicked()
{
    item->Rotate();
    ui->textBrowser->clear();
    ui->textBrowser->append(item->Cardinate());
}
