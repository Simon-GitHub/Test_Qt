#include "MainWidget.h"
#include <QGridLayout>
#include <QDebug>
//#include <QWindowsStyle>
#include <QFileDialog>
#include <QMessageBox>
#include <QNetworkInterface>
CMainWidget::CMainWidget(QWidget *parent) :
    QWidget(parent)
{
    m_nPort =23232;
    createComponent();
    createLayout();
    m_qLocalAddr = GetIP();

    m_qpSendUdpSocket = new QUdpSocket(this);

    this->resize(550,400);
    InitSetSocket();
    //m_pRecvProgressBar->setValue(80);
    connect(m_qpSelFileBtn,SIGNAL(clicked()),this,SLOT(sltOpenFile()));
    connect(m_qpSendFileBtn,SIGNAL(clicked()),this,SLOT(sltSendFile()));
    connect(this,SIGNAL(sigSendFile(QString,unsigned short,QString)),m_pServer,SLOT(sltSendFileContent(QString,unsigned short,QString)));
    connect(m_pServer,SIGNAL(sndFileName(QString,QString)),this,SLOT(sltSetFileName(QString,QString)));
    connect(this,SIGNAL(sigRecvDataSignal(QByteArray)),this, SLOT(DecodePacketData(QByteArray)));
    connect(m_pServer,SIGNAL(sigSendStatusInfo(QString)),this,SLOT(SltSendStatusInfo(QString)));
    connect(m_pServer,SIGNAL(sigSendProgress(double)),this,SLOT(SltSendProgress(double)));



}
void CMainWidget::createComponent()
{

    m_pSendProgress = new CProgressBar(this);
    m_pRecvProgress = new CProgressBar(this);
    m_qpServerIPLbl = new QLabel(QString::fromLocal8Bit("接收方IP:"),this);
    m_qpServerIPEdit = new QLineEdit(this);
    m_qpServerPortLbl = new QLabel(QString::fromLocal8Bit("接收方端口:"),this);
    m_qpServerPortEdit = new QLineEdit(this);
    m_qpServerPortEdit->setText(QString::fromLocal8Bit("5555"));

    m_qpSendFileBtn = new QPushButton(QString::fromLocal8Bit("发送文件"),this);
    m_qpSendFileLbl = new QLabel(QString::fromLocal8Bit("发送文件:"),this);
    m_qpSendFileEdit = new QLineEdit(this);
    m_qpSelFileBtn = new QPushButton(QString::fromLocal8Bit("选择文件"),this) ;
    m_qpSendProgressLbl = new QLabel(QString::fromLocal8Bit("发送进度:"),this);

    m_qpSendStateLbl = new QLabel(QString::fromLocal8Bit("发送状态:"),this);
    m_qpSendStateEdit = new QLineEdit(this);


    m_qpListenPortLbl = new QLabel(QString::fromLocal8Bit("监听端口:"),this);
    m_qpListenPortEdit = new QLineEdit(this);
    m_qpListenPortEdit->setText(QString::fromLocal8Bit("5555"));
    m_qpStartListenBtn = new QPushButton(QString::fromLocal8Bit("开始监听"),this);
    m_qpReceiveFileLbl = new QLabel(QString::fromLocal8Bit("接收文件:"),this);
    m_qpReceiveFileEdit = new QLineEdit(this);
    m_qpReceiveProgressLbl = new QLabel(QString::fromLocal8Bit("接收进度:"),this);

    m_qpReceiveStateLbl = new QLabel(QString::fromLocal8Bit("接收状态:"),this);
    m_qpReceiveStateEdit = new QLineEdit(this);


    m_qpSendFileBox = new QGroupBox(QString::fromLocal8Bit("发送文件"),this);
    //m_qpSendFileBox->setStyle(new QWindowsStyle);
    m_qpReceiveFileBox = new QGroupBox(QString::fromLocal8Bit("接收文件"),this);
    //m_qpReceiveFileBox->setStyle(new QWindowsStyle);

    m_pServer = new CServer(this);


}
void CMainWidget::createLayout()
{

    QGridLayout *sendLayout = new QGridLayout();
    sendLayout->addWidget(m_qpServerIPLbl,0,0,1,1);
    sendLayout->addWidget(m_qpServerIPEdit,0,1,1,2);
    sendLayout->addWidget(m_qpServerPortLbl,0,3,1,1);
    sendLayout->addWidget(m_qpServerPortEdit,0,4,1,1);
    sendLayout->addWidget(m_qpSendFileBtn,0,5,1,1);

    sendLayout->addWidget(m_qpSendFileLbl,1,0,1,1);
    sendLayout->addWidget(m_qpSendFileEdit,1,1,1,4);
    sendLayout->addWidget(m_qpSelFileBtn,1,5,1,1);

    sendLayout->addWidget(m_qpSendStateLbl,2,0,1,1);
    sendLayout->addWidget(m_qpSendStateEdit,2,1,1,5);

    sendLayout->addWidget(m_qpSendProgressLbl,3,0,1,1);
    sendLayout->addWidget(m_pSendProgress,3,1,1,5);

    m_qpSendFileBox->setLayout(sendLayout);

    QGridLayout *receiveLayout = new QGridLayout();
    receiveLayout->addWidget(m_qpListenPortLbl,0,0,1,1);
    receiveLayout->addWidget(m_qpListenPortEdit,0,1,1,5);
    receiveLayout->addWidget(m_qpStartListenBtn,0,6,1,1);

    receiveLayout->addWidget(m_qpReceiveFileLbl,1,0,1,1);
    receiveLayout->addWidget(m_qpReceiveFileEdit,1,1,1,6);

    receiveLayout->addWidget(m_qpReceiveStateLbl,2,0,1,1);
    receiveLayout->addWidget(m_qpReceiveStateEdit,2,1,1,6);

    receiveLayout->addWidget(m_qpReceiveProgressLbl,3,0,1,1);
    receiveLayout->addWidget(m_pRecvProgress,3,1,1,6);



    m_qpReceiveFileBox->setLayout(receiveLayout);

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addWidget(m_qpSendFileBox);
    mainLayout->addWidget(m_qpReceiveFileBox);
    this->setLayout(mainLayout);
    this->show();

}

void CMainWidget::sltOpenFile()
{
    m_qSendFileName = QFileDialog::getOpenFileName(this);
    if(!m_qSendFileName.isEmpty())
    {
        //m_qSendFileName = m_qSendFileName.right(m_qSendFileName.size() - m_qSendFileName.lastIndexOf('/')-1);
        m_qpSendFileEdit->setText(m_qSendFileName);
    }

}

void CMainWidget::sltSendFile()
{
    QString recv_addr = m_qpServerIPEdit->text().trimmed();
    unsigned short port = m_qpServerPortEdit->text().toUShort();
    m_nPort =23232;
    m_qLocalAddr = GetIP();
//    if((GetIP() != m_qLocalAddr) || (port != m_nPort))
//    {
//        m_qLocalAddr = GetIP();

//        ReInitSocket();
//    }
    //判断ip和端口以及发送文件是否为空
   // m_qLocalAddr="192.168.40.229";

   // recv_addr ="192.168.40.209";
    m_qSendAddr =recv_addr;

    emit(sigSendFile(recv_addr,port,m_qSendFileName));
}

void CMainWidget::sltSetFileName(QString addr,QString filename)
{
    m_qSendFileName = filename;
    m_qSendAddr = addr;
    SndMsg(FileName);
}

void CMainWidget::SndMsg(MsgType type, QString srvaddr)
{
    QByteArray data;
    QDataStream out(&data, QIODevice::WriteOnly);
    QString address = GetIP();
    QString filename = m_qSendFileName.right(m_qSendFileName.size() - m_qSendFileName.lastIndexOf('/')-1);
    out << type ;

    switch(type)
    {

      case FileName :
      {

        out <<m_qLocalAddr<<m_qSendAddr<<filename;
        break;
      }
      case Accept:
      {
        out<<m_qLocalAddr<<srvaddr;
        break;
      }
      case Refuse :
      {
        out <<m_qLocalAddr<<srvaddr;
        break;
      }
      default:
        break;

    }
    int len = m_qpSendUdpSocket->writeDatagram(data,data.length(),QHostAddress::Broadcast, m_nPort);
    qDebug()<<"len"<<len;

}


QString CMainWidget::GetIP()
{
    QList<QHostAddress> list = QNetworkInterface::allAddresses();
    foreach (QHostAddress addr, list) {
        if((addr.protocol() == QAbstractSocket::IPv4Protocol) && (addr.toString() != "127.0.0.1"))
        {
            return addr.toString();
        }
    }
    return "0";
}

void CMainWidget::ReInitSocket()
{
    m_qpSendUdpSocket->close();
    qDebug()<<"m_qLocalAddr"<<m_qLocalAddr;
    int suss =  m_qpSendUdpSocket->bind(m_nPort, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
    if(!suss)
    {
         qDebug()<<"bind port fail";
    }
}



void CMainWidget::sltGetReceiveData()
{

    if(m_qpSendUdpSocket->hasPendingDatagrams())
    {
        qDebug()<<"data size = "<<(m_qpSendUdpSocket->pendingDatagramSize());
        QByteArray bytearray;
        bytearray.resize(m_qpSendUdpSocket->pendingDatagramSize());

        m_qpSendUdpSocket->readDatagram( bytearray.data(), bytearray.size());
        emit(sigRecvDataSignal(bytearray));
    }
}


void CMainWidget::DecodePacketData(QByteArray byte_array)
{
    QDataStream in(&byte_array, QIODevice::ReadOnly);
    int msgType;
    QString srcaddr,recvaddr;
    in >> msgType;

    switch(msgType)
    {

      case FileName:
      {
         QString  fileName;
         in >>srcaddr>>recvaddr >> fileName;

         hasPendingFile(srcaddr,recvaddr, fileName);
         break;
      }
      case Accept:
      {
        in>>srcaddr>>recvaddr;
        break;
      }
      case Refuse:
      {
        in>>srcaddr>>recvaddr;
        QMessageBox::information(this,tr("提示"),tr("对方拒绝接收？"),QMessageBox::Ok);

        break;
      }
      default:
        break;

    }

}

void CMainWidget::hasPendingFile(QString srcaddr,QString recvaddr,QString filename)
{
    qDebug()<<"recvaddr"<<recvaddr<<"filename"<<filename;
    if(isExistTheAddr(recvaddr))
    {
        int btn = QMessageBox::information(this,tr("接受文件"),tr("来自%1的%2文件,是否接收？").arg(srcaddr).arg(filename),QMessageBox::Yes,QMessageBox::No);
        if (btn == QMessageBox::Yes)
        {
            QString name = QFileDialog::getSaveFileName(0,tr("保存文件"),filename);
            if(!name.isEmpty())
            {

                m_pClient = new CClient(this);
                m_pClient->setFileName(name);
                m_pClient->setHostAddr(QHostAddress(srcaddr));
               // clnt->show();
                 SndMsg(Accept,srcaddr);
                 m_qpReceiveFileEdit->setText(name);
                 connect(m_pClient,SIGNAL(sigRecvStatusInfo(QString)),this,SLOT(SltRecvStatusInfo(QString)));
                 connect(m_pClient,SIGNAL(sigRecvProgress(double)),this,SLOT(SltRecvProgress(double)));

            }
        }
        else
        {
            SndMsg(Refuse,srcaddr);
        }
    }

}
bool CMainWidget::isExistTheAddr(QString recvaddr)
{
   QList<QHostAddress> list = QNetworkInterface::allAddresses();
   foreach (QHostAddress addr, list)
   {
     if((addr.protocol() == QAbstractSocket::IPv4Protocol) && (QHostAddress(recvaddr) == addr))
     {
        return true;
     }
   }
   return false;
}

void CMainWidget::InitSetSocket()
{

     int suss =  m_qpSendUdpSocket->bind(m_nPort, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
     if(!suss)
     {
         qDebug()<<"bind port fail";
     }
     connect(m_qpSendUdpSocket, SIGNAL(readyRead()),this, SLOT(sltGetReceiveData()), Qt::DirectConnection);//连接接收信号槽
}

void CMainWidget::SltSendStatusInfo(QString statusText)
{
    if(!statusText.isEmpty())
    {
        m_qpSendStateEdit->setText(statusText);
    }
}

void CMainWidget::SltSendProgress(double value)
{
    m_pSendProgress->setValue(value);
}

void CMainWidget::SltRecvStatusInfo(QString statusText)
{
    if(!statusText.isEmpty())
    {
        m_qpReceiveStateEdit->setText(statusText);
    }
}

void CMainWidget::SltRecvProgress(double value)
{
    m_pRecvProgress->setValue(value);
}
