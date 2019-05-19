#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileInfoList>

class QTimer;
class QUdpSocket;
namespace Ui 
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
	enum WorkState
	{
		UnknownState = 0
		, Begin
		, Pause
		, Stop
	};
	enum WorkModel
	{
		UnknownModel = 0
		, RealTime
		, Simulation
	};
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
	void initTargetVisible();
	QDateTime dateTime(QString str);
private slots:
	void dirToolButtonClicked(bool);
	void addHeaderPushButtonClicked(bool);
	void subHeaderPushButtonClicked(bool);
	void modifyNamePushButtonClicked(bool);
	void beginPushButtonClicked(bool);
	void pausePushButtonClicked(bool);
	void stopPushButtonClicked(bool);
	void normalPushButtonClicked(bool);
	void acceleratePushButtonClicked(bool);
	void deceleratePushButtonClicked(bool);
	void timerTimeout();
	// 
	void processPendingDatagrams();

private:
    Ui::MainWindow *m_ui;
	WorkState		m_workState;
	WorkModel		m_workModel;
	QTimer*			m_timer;
	QFileInfoList	m_fileInfoList;
	int				m_sendNum;
	QUdpSocket*		m_udpSocket;
	double			m_speedFactor;
	double			m_multiple;
	// target visible
	QUdpSocket*		m_targetVisibleUdpSocket;
	static const unsigned char			s_typeReceive[4];
	static const unsigned char			s_typeSend[4];
};

#endif // MAINWINDOW_H
