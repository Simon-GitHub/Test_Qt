#include "DataSend2Widget.h"
#include "ui_DataSend2Widget.h"
#include <QTimer>
#include <QFileDialog>
#include <QUdpSocket>

DataSend2Widget::DataSend2Widget(QWidget* parent, Qt::WindowFlags f)
	: QWidget(parent, f)
	, m_ui(new Ui::DataSend2Widget)
	, m_timer(new QTimer)
	, m_sendNum(0)
{
	m_ui->setupUi(this);
	connect(m_ui->dirToolButton, SIGNAL(clicked(bool)), SLOT(dirToolButtonClicked(bool)));
	connect(m_ui->beginPushButton, SIGNAL(clicked(bool)), SLOT(beginPushButtonClicked(bool)));
	connect(m_ui->pausePushButton, SIGNAL(clicked(bool)), SLOT(pausePushButtonClicked(bool)));
	connect(m_ui->stopPushButton, SIGNAL(clicked(bool)), SLOT(stopPushButtonClicked(bool)));
	connect(m_timer, SIGNAL(timeout()), SLOT(timerTimeout()));
}

DataSend2Widget::~DataSend2Widget()
{

}

void DataSend2Widget::dirToolButtonClicked(bool)
{
	QString dirStr = QFileDialog::getOpenFileName(this, tr("Open Directory"),
		"./");
	if(dirStr.isEmpty()) return;
	m_ui->dirLineEdit->setText(dirStr);

	QFile file(m_ui->dirLineEdit->text());
	if(!file.open(QIODevice::ReadOnly)) return;
	QByteArray byteArray = file.readAll();
	file.close();
	m_allByteArray = QByteArray::fromHex(byteArray);
}

void DataSend2Widget::beginPushButtonClicked(bool)
{
	m_timer->start(m_ui->intervalSpinBox->value());
}

void DataSend2Widget::pausePushButtonClicked(bool)
{
	m_timer->stop();
}

void DataSend2Widget::stopPushButtonClicked(bool)
{
	m_timer->stop();
	m_sendNum = 0;
	m_ui->frameSpinBox->setValue(m_sendNum);
}

void DataSend2Widget::timerTimeout()
{
	int byteCount = m_ui->byteCountSpinBox->value();
	if((m_sendNum+1)*byteCount > m_allByteArray.size())
	{
		stopPushButtonClicked(true);
		return;
	}
	QUdpSocket udpSocket;
	udpSocket.writeDatagram(
		m_allByteArray.data() + m_sendNum * byteCount
		, byteCount
		, QHostAddress(m_ui->ipLineEdit->text())
		, m_ui->portSpinBox->value());
	m_ui->frameSpinBox->setValue(m_sendNum);
	++m_sendNum;
}