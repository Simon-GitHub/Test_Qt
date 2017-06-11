#include "Client.h"
#include <QMessageBox>
CClient::CClient(QWidget *parent) :
    QWidget(parent)
{
    bytesReceived = 0;
    fileNameSize = 0;
    m_nClientPort = 5555;

    m_qpClientSocket = new QTcpSocket(this);
    connect(m_qpClientSocket, SIGNAL(readyRead()), this, SLOT(readMsg()));
    connect(m_qpClientSocket, SIGNAL(error(QAbstractSocket::SocketError)), this,SLOT(displayErr(QAbstractSocket::SocketError)));
}
void CClient::setFileName(QString name)
{
    m_qpSaveFile = new QFile(name);
}

void CClient::setHostAddr(QHostAddress addr)
{
    hostAddr = addr;
    newConn();
}

void CClient::newConn()
{

    m_qpClientSocket->abort();
    m_qpClientSocket->connectToHost(hostAddr, m_nClientPort);
    time.start();
}

void CClient::readMsg()
{
    QDataStream in(m_qpClientSocket);
    in.setVersion(QDataStream::Qt_4_7);

    float useTime = time.elapsed();

    if (bytesReceived <= sizeof(qint64)*2) {
        if ((m_qpClientSocket->bytesAvailable() >= sizeof(qint64)*2) && (fileNameSize == 0))
        {
            in>>totalBytes>>fileNameSize;
            bytesReceived += sizeof(qint64)*2;
        }
        if((m_qpClientSocket->bytesAvailable() >= fileNameSize) && (fileNameSize != 0)){
            in>>fileName;
            bytesReceived +=fileNameSize;

            if(!m_qpSaveFile->open(QFile::WriteOnly)){
                QMessageBox::warning(this,tr("应用程序"),tr("无法读取文件 %1:\n%2.").arg(fileName).arg(m_qpSaveFile->errorString()));
                return;
            }
        } else {
            return;
        }
    }
    if (bytesReceived < totalBytes) {
        bytesReceived += m_qpClientSocket->bytesAvailable();
        inBlock = m_qpClientSocket->readAll();
        m_qpSaveFile->write(inBlock);
        inBlock.resize(0);
    }


    double speed = bytesReceived / useTime;
//    ui->cStatusLbl->setText(tr("已接收 %1MB (%2MB/s) \n共%3MB 已用时：%4秒\n估计剩余时间：%5秒")
//                                      .arg(bytesReceived / (1024*1024))
//                                      .arg(speed*1000/(1024*1024),0,'f',2)
//                                      .arg(totalBytes / (1024 * 1024))
//                                      .arg(useTime/1000,0,'f',0)
//                                      .arg(totalBytes/speed/1000 - useTime/1000,0,'f',0));
  QString statusText;
  statusText = QString("已接收 %1MB (%2MB/s) 共%3MB 已用时：%4秒")
                                               .arg(bytesReceived / (1024*1024.0),3,'f',2)
                                               .arg(speed*1000/(1024*1024),0,'f',2)
                                               .arg(totalBytes / (1024 * 1024.0),3,'f',2)
                                               .arg(useTime/1000,0,'f',0);
   emit sigRecvStatusInfo(statusText);
   emit sigRecvProgress(1.0*bytesReceived / totalBytes * 100.0);
    if(bytesReceived == totalBytes)
    {
        m_qpSaveFile->close();
        m_qpClientSocket->close();

    }
}

//void CClient::on_cCancleBtn_clicked()
//{
//    m_qpClientSocket->abort();
//    if (m_qpSaveFile->isOpen())
//        m_qpSaveFile->close();
//}
void CClient::displayErr(QAbstractSocket::SocketError sockErr)
{
    switch(sockErr)
    {
    case QAbstractSocket::RemoteHostClosedError :
        break;
    default :
        qDebug()<< m_qpClientSocket->errorString();
    }
}
