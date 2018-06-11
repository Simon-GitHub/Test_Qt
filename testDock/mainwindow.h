#ifndef MAINWINDOW_H
#define MAINWINDOW_H
 
#include <QMainWindow>
#include <QWidget>
 
namespace Ui {
class MainWindow;
}
 
class MainWindow : public QMainWindow
{
    Q_OBJECT
     
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private slots:
	void pushButtonClicked(bool isClicked);

private:
    Ui::MainWindow *ui;
	QDockWidget *testDock;
};
 
#endif // MAINWINDOW_H