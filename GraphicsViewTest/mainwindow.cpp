#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "lineitem.h"
#include "rectitem.h"
#include "widgetitem.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QGraphicsScene* scene = new QGraphicsScene(ui->graphicsView);
    ui->graphicsView->setScene(scene);
    LineItem* lineItem = new LineItem;
    lineItem->setLine(QLine(0,0,200,200));
    QPen pen;
    pen.setColor(QColor(255,0,0));
    pen.setWidth(2);
    lineItem->setPen(pen);
    lineItem->setPos(250,100);
    scene->addItem(lineItem);

    RectItem* rectItem = new RectItem;
    rectItem->setRect(QRect(0,0,200,200));
    rectItem->setPen(pen);
    rectItem->setPos(0,0);
    scene->addItem(rectItem);

    WidgetItem* widgetItem = new WidgetItem;
    widgetItem->setPos(0,300);
    scene->addItem(widgetItem);
}

MainWindow::~MainWindow()
{
    delete ui;
}
