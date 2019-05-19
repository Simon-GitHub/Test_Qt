#pragma once

#include <QWidget>

namespace Ui
{
	class DataSend2Widget;
}
class QTimer;
class DataSend2Widget: public QWidget
{
	Q_OBJECT
public:
	DataSend2Widget(QWidget* parent = 0, Qt::WindowFlags f = 0);
	virtual ~DataSend2Widget();
private slots:
	void dirToolButtonClicked(bool);
	void beginPushButtonClicked(bool);
	void pausePushButtonClicked(bool);
	void stopPushButtonClicked(bool);
	void timerTimeout();
private:
	Ui::DataSend2Widget* m_ui;
	QTimer* m_timer;
	QByteArray m_allByteArray;
	unsigned int m_sendNum;
};