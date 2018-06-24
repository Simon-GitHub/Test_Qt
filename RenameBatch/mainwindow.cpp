#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTimer>
#include <QDateTime>
#include <QFileDialog>
#include <QUdpSocket>
#include <fstream>
#include <iostream>
#include <set>
#include <QSet>

const unsigned char MainWindow::s_typeReceive[4]={0xAB, 0x00, 0x00, 0x01};
const unsigned char MainWindow::s_typeSend[4]={0xAB, 0x00, 0x00, 0x02};

MainWindow::MainWindow(QWidget *parent) :
QMainWindow(parent),
	m_ui(new Ui::MainWindow)
	, m_timer(new QTimer)
	, m_sendNum(0)
	, m_udpSocket(new QUdpSocket(this))
	, m_speedFactor(1.0)
	, m_multiple(0.1)
	, m_workState(UnknownState)
	, m_workModel(UnknownModel)
{
#if 1
	QPolygonF polygon;
	polygon.push_back(QPointF(1,0));
	polygon.push_back(QPointF(0,1));
	polygon.push_back(QPointF(-1,0));
	polygon.push_back(QPointF(0,-1));

	bool b = polygon.containsPoint(QPointF(0,0), Qt::OddEvenFill);
#endif
	m_ui->setupUi(this);
	connect(m_ui->dirToolButton, SIGNAL(clicked(bool)), SLOT(dirToolButtonClicked(bool)));
	connect(m_ui->addHeaderPushButton, SIGNAL(clicked(bool)), SLOT(addHeaderPushButtonClicked(bool)));
	connect(m_ui->subHeaderPushButton, SIGNAL(clicked(bool)), SLOT(subHeaderPushButtonClicked(bool)));
	connect(m_ui->beginPushButton, SIGNAL(clicked(bool)), SLOT(beginPushButtonClicked(bool)));
	connect(m_ui->normalPushButton, SIGNAL(clicked(bool)), SLOT(normalPushButtonClicked(bool)));
	connect(m_ui->acceleratePushButton, SIGNAL(clicked(bool)), SLOT(acceleratePushButtonClicked(bool)));
	connect(m_ui->deceleratePushButton, SIGNAL(clicked(bool)), SLOT(deceleratePushButtonClicked(bool)));
	connect(m_ui->pausePushButton, SIGNAL(clicked(bool)), SLOT(pausePushButtonClicked(bool)));
	connect(m_ui->stopPushButton, SIGNAL(clicked(bool)), SLOT(stopPushButtonClicked(bool)));
	connect(m_timer, SIGNAL(timeout()), SLOT(timerTimeout()));

	initTargetVisible(); 
}

MainWindow::~MainWindow()
{
	delete m_ui;
}

void MainWindow::initTargetVisible()
{
	m_targetVisibleUdpSocket = new QUdpSocket(this);
	m_targetVisibleUdpSocket->bind(QHostAddress::AnyIPv4, m_ui->receivePortLineEdit->text().toInt(), QUdpSocket::ReuseAddressHint);
	QHostAddress mcast_addr(m_ui->receiveIpLineEdit->text());  
	m_targetVisibleUdpSocket->joinMulticastGroup(mcast_addr);
	connect(m_targetVisibleUdpSocket, SIGNAL(readyRead()),
		this, SLOT(processPendingDatagrams()));
}

QDateTime MainWindow::dateTime(QString str)
{
	QStringList strList = str.split("_");
	if(strList.size() != 6) return QDateTime();
	QDate date(strList[0].toInt(), strList[1].toInt(), strList[2].toInt());
	int seconds = strList[5].toDouble();
	double allSeconds = strList[5].toDouble()+0.0005;
	int mSeconds = (allSeconds-seconds)*1000;
	QTime time(strList[3].toInt(), strList[4].toInt(), seconds, mSeconds);
	return QDateTime(date, time);
}

void MainWindow::dirToolButtonClicked(bool)
{
	QString dirStr = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
		"/home",
		QFileDialog::ShowDirsOnly
		| QFileDialog::DontResolveSymlinks);
	if(dirStr.isEmpty()) return;
	m_ui->dirLineEdit->setText(dirStr);
}

void MainWindow::addHeaderPushButtonClicked(bool)
{
	QString dirStr = m_ui->dirLineEdit->text();
	if(dirStr.isEmpty()) return;
	QDir dir(dirStr);
	QFileInfoList fileInfoList = dir.entryInfoList(QDir::Files, QDir::Time);
	for (int i = 0; i < fileInfoList.size(); ++i)
	{
		QString baseName = fileInfoList[i].completeBaseName();
		QStringList strList = baseName.split("_");
		if(strList.size() != 9) continue;
		QString newBaseName;
		for(int j = 3; j < strList.size(); ++j)
		{
			newBaseName += strList[j];
			if (j != strList.size() - 1)
			{
				newBaseName += "_";
			}
		}
		QString absoluteFilePath = fileInfoList[i].absoluteFilePath();
		QString suffix = fileInfoList[i].suffix();
		QString absolutePath = fileInfoList[i].absolutePath();
		QFile file(absoluteFilePath);
		bool renameFlag = file.rename(absolutePath + QDir::separator() + newBaseName + "." + suffix);
		while (!renameFlag)
		{
			double num = newBaseName.right(6).toDouble();
			num = num + 0.001;
			newBaseName = newBaseName.left(newBaseName.lastIndexOf("_")+1) + QString("%1").arg(num,5,'f',3);
			renameFlag = file.rename(absolutePath + QDir::separator() + newBaseName + "." + suffix);
		}
		QString pathName = absolutePath + QDir::separator() + newBaseName + "." + suffix;
		char rawData[65535] = {0};
		short length = 0;
		{
			std::ifstream ifs;
			ifs.open(pathName.toLocal8Bit().data(), std::ios::binary);
			ifs.read(rawData,65535);
			length = ifs.gcount();
			ifs.close();
		}

		const char type1[4]={0xEB, 0x90, 0x21, 0x00};
		const char type2[4]={0xEB, 0x90, 0x22, 0x00};
		const char type3[4]={0xEB, 0x90, 0x23, 0x00};
		const char type4[4]={0xEB, 0x90, 0x24, 0x00};
		const char type5[4]={0xEB, 0x90, 0x25, 0x00};
		std::ofstream ofs;
		ofs.open( pathName.toLocal8Bit().data(), std::ios::binary|std::ios::out );
		ofs.seekp( 0, std::ios::beg );

		// 添加数据包头字节
		if (strList[1] == "DynData")
		{
			ofs.write( type1, 4 );
		}
		else if (strList[1] == "FacData")
		{
			ofs.write( type2, 4 );
		}
		else if (strList[1] == "AreaPointData")
		{
			ofs.write( type3, 4 );
		}
		else if (strList[1] == "ActionSeqData")
		{
			ofs.write( type4, 4 );
		}
		else if (strList[1] == "OrbitArcData")
		{
			ofs.write( type5, 4 );
		}
		else
		{
		}
		// 添加标志
		char flag[1] = { 0x00 };
		ofs.write(flag,1);
		// 添加包序号
		short index = 0;
		ofs.write((char*)&index,2);
		// 添加有效数据长度
		ofs.write((char*)&length,2);
		// 添加有效数据
		ofs.write(rawData,length);
		// 添加校验和
		short allLength = 9+length;
		ofs.write((char*)(&allLength),1);
		ofs.close();
	}
}

void MainWindow::subHeaderPushButtonClicked(bool)
{
	QString dirStr = m_ui->dirLineEdit->text();
	if(dirStr.isEmpty()) return;
	QDir dir(dirStr);
	QFileInfoList fileInfoList = dir.entryInfoList(QDir::Files, QDir::Time);
	for (int i = 0; i < fileInfoList.size(); ++i)
	{
		QString absoluteFilePath = fileInfoList[i].absoluteFilePath();
		char rawData[65535] = {0};
		memset(rawData, 0, 65535);
		short length = 0;
		{
			std::ifstream ifs;
			ifs.open(absoluteFilePath.toLocal8Bit().data(), std::ios::binary);
			ifs.read(rawData,65535);
			length = ifs.gcount();
			ifs.close();
		}

		std::ofstream ofs;
		ofs.open( absoluteFilePath.toLocal8Bit().data(), std::ios::binary|std::ios::out );
		ofs.seekp( 0, std::ios::beg );

		ofs.write(rawData+9,length-10);
		ofs.close();
	}
}

void MainWindow::beginPushButtonClicked(bool)
{
	if(m_workState != Pause)
	{
		QString dirStr = m_ui->dirLineEdit->text();
		if(dirStr.isEmpty()) return;
		QDir dir(dirStr);
		m_fileInfoList = dir.entryInfoList(QDir::Files, QDir::Name);
	}
	m_workState = Begin;
	if (m_ui->realTimeRadioButton->isChecked())
	{
		m_workModel = RealTime;
	}
	else if (m_ui->simulationRadioButton->isChecked())
	{
		m_workModel = Simulation;
	}
	else
	{
	}
	timerTimeout();
}

void MainWindow::normalPushButtonClicked(bool)
{
	if (m_workModel == RealTime)
	{
		m_speedFactor = 1.0;
		m_ui->speedFactorLineEdit->setText(QString("%1").arg(m_speedFactor));
	}
	else if (m_workModel == Simulation)
	{
		m_ui->intervalSpinBox->setValue(100);
	}
	else 
	{
	}
}

void MainWindow::acceleratePushButtonClicked(bool)
{
	if (m_workModel == RealTime)
	{
		double temp = m_speedFactor - m_multiple;
		while (temp <= m_multiple) 
		{
			m_multiple = m_multiple / 10;
			temp = m_speedFactor - m_multiple;
		}
		m_speedFactor = temp;
		m_ui->speedFactorLineEdit->setText(QString::number(m_speedFactor, 'f', 10));
	}
	else 
	{
	}
}

void MainWindow::deceleratePushButtonClicked(bool)
{
	if (m_workModel == RealTime)
	{
		m_multiple = 0.1;
		m_speedFactor = m_speedFactor + m_multiple;
		m_ui->speedFactorLineEdit->setText(QString::number(m_speedFactor));
	}
	else 
	{
	}
}

void MainWindow::pausePushButtonClicked(bool)
{
	m_workState = Pause;
	m_timer->stop();
}

void MainWindow::stopPushButtonClicked(bool)
{
	m_workState = Stop;
	m_timer->stop();
	m_sendNum = 0;
	m_ui->frameSpinBox->setValue(m_sendNum);
}

void MainWindow::timerTimeout()
{
	m_sendNum = m_ui->frameSpinBox->value();
	QFileInfo fileInfo = m_fileInfoList[m_sendNum];

	QString absoluteFilePath = fileInfo.absoluteFilePath();
	QFile file(absoluteFilePath);
	file.open(QIODevice::ReadOnly);
	QByteArray byteArray = file.readAll();
	file.close();
	{
#if 0
		char buf[3] = {0};
		unsigned char result[100] = {0};
		std::ifstream ifs;
		ifs.open(absoluteFilePath.toLocal8Bit().data(), std::ios::binary);
		char temp[65535] = {0};
		ifs.read(temp, 65535);
		int count = ifs.gcount();
		ifs.close();
#endif
	}
	int size = byteArray.size();
	QString ip = m_ui->ipLineEdit->text();
	m_udpSocket->writeDatagram(byteArray.data(), byteArray.size(),
		QHostAddress(m_ui->ipLineEdit->text()), m_ui->portSpinBox->value());

	if ((m_sendNum+1) < m_fileInfoList.size())
	{
		if (m_workModel == RealTime)
		{
			QDateTime beginDateTime = dateTime(m_fileInfoList[m_sendNum].completeBaseName());
			QDateTime endDateTime = dateTime(m_fileInfoList[m_sendNum+1].completeBaseName());
			int ms = beginDateTime.msecsTo(endDateTime);
			m_timer->start(ms*m_speedFactor);
		}
		else if (m_workModel == Simulation)
		{
			m_timer->start(m_ui->intervalSpinBox->value());
		}
		else
		{
		}
		++m_sendNum;
	}
	else
	{
		m_sendNum = 0;
		m_timer->stop();
	}
	m_ui->frameSpinBox->setValue(m_sendNum);
}

void MainWindow::processPendingDatagrams()
{
	while (m_targetVisibleUdpSocket->hasPendingDatagrams()) 
	{
		QHostAddress senderIp;
		quint16 senderPort;
		QByteArray datagram;
		datagram.resize(m_targetVisibleUdpSocket->pendingDatagramSize());
		m_targetVisibleUdpSocket->readDatagram(datagram.data(), datagram.size(), &senderIp, &senderPort);
		QString ip = senderIp.toString();

		int resive = datagram.size();
		const char* recvAddr = datagram.constData();

		char headerByte[4];
		memset(headerByte,0,4);
		memcpy(headerByte,recvAddr,4);
		if (memcmp(headerByte,s_typeReceive,4) != 0) continue;

		short validDataLength = 0;
		memcpy(&validDataLength, recvAddr+4, sizeof(short));
		short targetCount = 0;
		memcpy(&targetCount, recvAddr+4+sizeof(short), sizeof(short));

		int sendDataLength = 4+sizeof(short)*2+(36+sizeof(short))*targetCount;
		char* sendDataChar = new char[sendDataLength];
		memset(sendDataChar,0,sendDataLength);
		memcpy(sendDataChar,s_typeSend,4);
		short parseLen = 4+sizeof(short)*2;
		for (short i = 0; i < targetCount; ++i)
		{
			char uuid[36];
			memset(uuid, 0, 36);
			memcpy(uuid, recvAddr+parseLen, 36);
			memcpy(sendDataChar+4+sizeof(short)*2+i*(36+sizeof(short)),recvAddr+parseLen,36);
			parseLen += 36;
			short visible = i%2;
			memcpy(sendDataChar+4+sizeof(short)*2+i*(36+sizeof(short))+36,&visible,sizeof(short));

			parseLen += 22;
			short pointCount = 0;
			memcpy(&pointCount, recvAddr+parseLen, sizeof(short));
			parseLen += (2 + pointCount* 8);
		}

		short sendValidDataLength = validDataLength-8;
		memcpy(sendDataChar+4,&sendValidDataLength,2);
		memcpy(sendDataChar+6,&targetCount,2);

		QHostAddress mcast_addr(m_ui->sendIpLineEdit->text());  
		QUdpSocket udpSocket;
		udpSocket.writeDatagram(QByteArray(sendDataChar, sendDataLength), mcast_addr, m_ui->sendPortLineEdit->text().toInt());
	}
}
