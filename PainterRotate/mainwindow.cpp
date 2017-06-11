#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    scene=new QGraphicsScene;
    ui->graphicsView->setScene(scene);
    //把“画布” “挂”到展示的窗体QGraphicsView当中
    scene->setSceneRect(-500,-500,1000,1000);
    //设置“画布”矩形区域，坐标（-500，-500）为左上角，这就意味着scene的中心位置为(0,0).这个坐标系就是scene的坐标系
    ui->graphicsView->centerOn(0,0);//把画布scene的中心位置放到QGraphicsView正中央
    item=new MyItem(260,240);
    scene->addItem(item);//往“画布”上绘制MyItem
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
