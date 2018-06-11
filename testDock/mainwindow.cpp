#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "myDockTitleBar.h"
 
#include <QDockWidget>
#include <QToolButton>
#include <QPushButton>
#include <QVBoxLayout>
#include <QMdiSubWindow>
#include <QFrame>
#include <QDebug>
 
#include <QPainter>
#include <QMouseEvent>
#include <QBitmap>
 
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

	setDockNestingEnabled(false);
	//É¾³ýÖÐÑë´°Ìå
	QWidget* p = takeCentralWidget();
	if(p)
		delete p;

	testDock = new QDockWidget(tr("Test Dock"));
    testDock->setFeatures(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetFloatable
                          | QDockWidget::DockWidgetMovable);
    testDock->setMinimumWidth(200);
    testDock->setStyleSheet(tr("background-color:green;"));
#if 1
	MyDockTitleBar *titlebar1 = new MyDockTitleBar(testDock);
	testDock->setTitleBarWidget(titlebar1);
#else
	testDock->setTitleBarWidget(new QWidget);
#endif
    QWidget *testWidget = new QWidget;
    QVBoxLayout *box = new QVBoxLayout;
 
    QPushButton *okButton = new QPushButton(tr("Ok1"));
    okButton->setStyleSheet(tr("background-color:white;"));
	connect(okButton, SIGNAL(clicked(bool)), this, SLOT(pushButtonClicked(bool)));
    box->addWidget(okButton);
    testWidget->setLayout(box);
 
    testDock->setWidget(testWidget);
 
    //////////////////////////////////////////////////////////
 
    QDockWidget *testDock2 = new QDockWidget(tr("Test Dock2"));
    testDock2->setFeatures(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetFloatable
                           | QDockWidget::DockWidgetMovable);
    testDock2->setMinimumWidth(200);
    testDock2->setStyleSheet(tr("background-color:orange;"));
#if 1
	MyDockTitleBar *titlebar2 = new MyDockTitleBar(testDock2);
	testDock2->setTitleBarWidget(titlebar2);
#else
	testDock2->setTitleBarWidget(new QWidget);
#endif
    QWidget *testWidget2 = new QWidget;
    QVBoxLayout *box2 = new QVBoxLayout;
 
    QPushButton *okButton2 = new QPushButton(tr("Ok2"));
    okButton2->setStyleSheet(tr("background-color:white;"));
    box2->addWidget(okButton2);
    testWidget2->setLayout(box2);
 
    testDock2->setWidget(testWidget2);
 
    //////////////////////////////////////////////////////////
	QDockWidget *testDock3 = new QDockWidget(tr("Test Dock2"));
	{
		testDock3->setFeatures(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetFloatable
			| QDockWidget::DockWidgetMovable);
		testDock3->setMinimumWidth(200);
		testDock3->setStyleSheet(tr("background-color:orange;"));
#if 1
		MyDockTitleBar *titlebar2 = new MyDockTitleBar(testDock3);
		testDock3->setTitleBarWidget(titlebar2);
#else
		testDock3->setTitleBarWidget(new QWidget);
#endif
		QWidget *testWidget2 = new QWidget;
		QVBoxLayout *box2 = new QVBoxLayout;

		QPushButton *okButton2 = new QPushButton(tr("Ok3"));
		okButton2->setStyleSheet(tr("background-color:white;"));
		box2->addWidget(okButton2);
		testWidget2->setLayout(box2);

		testDock3->setWidget(testWidget2);
	}

	//////////////////////////////////////////////////////////
	QDockWidget *testDock4 = new QDockWidget(tr("Test Dock2"));
	{
		testDock4->setFeatures(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetFloatable
			| QDockWidget::DockWidgetMovable);
		testDock4->setMinimumWidth(200);
		testDock4->setStyleSheet(tr("background-color:orange;"));
#if 1
		MyDockTitleBar *titlebar2 = new MyDockTitleBar(testDock4);
		testDock4->setTitleBarWidget(titlebar2);
#else
		testDock4->setTitleBarWidget(new QWidget);
#endif
		QWidget *testWidget2 = new QWidget;
		QVBoxLayout *box2 = new QVBoxLayout;

		QPushButton *okButton2 = new QPushButton(tr("Ok4"));
		okButton2->setStyleSheet(tr("background-color:white;"));
		box2->addWidget(okButton2);
		testWidget2->setLayout(box2);

		testDock4->setWidget(testWidget2);
	}

	//////////////////////////////////////////////////////////
#if 1
#if 0
    this->addDockWidget(Qt::LeftDockWidgetArea, testDock);
 
    this->setTabPosition(Qt::LeftDockWidgetArea, QTabWidget::West);
    this->tabifyDockWidget(testDock, testDock2);
#else
	this->addDockWidget(Qt::LeftDockWidgetArea, testDock);
	this->addDockWidget(Qt::RightDockWidgetArea, testDock2);
	this->addDockWidget(Qt::LeftDockWidgetArea, testDock3);
	this->addDockWidget(Qt::RightDockWidgetArea, testDock4);
#endif
#else
	QGridLayout* lay = dynamic_cast<QGridLayout*>(ui->centralwidget->layout());
	if (!lay)
	{
		lay = new QGridLayout(ui->centralwidget);
		lay->setSpacing(0);
		lay->setContentsMargins(0, 0, 0, 0);
	}
	lay->addWidget(testDock, 0, 0);
	lay->addWidget(testDock2, 0, 1);
#endif
}
 
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::pushButtonClicked(bool isClicked)
{
	testDock->setFloating(!testDock->isFloating());
}