#include "Server.h"
#include <QTcpServer>
#include <QTcpSocket>
#include <QMessageBox>
#include <QApplication>
CServer::CServer(QWidget *parent) :
    QWidget(parent)
{

    m_qpTcpSrv = new QTcpServer(this);
    m_nRecvPort = 5555;
    payloadSize = 64*1024;
    connect(m_qpTcpSrv, SIGNAL(newConnection()), this, SLOT(sndMsg()));
}

void CServer::sltSendFileContent(QString addr,unsigned short port,QString fileofname)
{
  //  m_nRecvPort = port;
    fileName = fileofname;
    if(!m_qpTcpSrv->isListening())
    {
        if(!m_qpTcpSrv->listen(QHostAddress::Any,m_nRecvPort))//开始监听
        {
            qDebug() << m_qpTcpSrv->errorString();
            close();
            return;
        }
    }
    emit sndFileName(addr,fileofname);
}
void CServer::initSrv()
{
    payloadSize = 64*1024;
    totalBytes = 0;
    bytesWritten = 0;
    bytesTobeWrite = 0;

}
void CServer::sndMsg()
{
    initSrv();
    clntConn = m_qpTcpSrv->nextPendingConnection();
    connect(clntConn,SIGNAL(bytesWritten(qint64)),this,SLOT(updClntProgress(qint64)));


    m_qpLocalFile = new QFile(fileName);
    if(!m_qpLocalFile->open((QFile::ReadOnly))){
        QMessageBox::warning(this, tr("应用程序"), tr("无法读取文件 %1:\n%2").arg(fileName).arg(m_qpLocalFile->errorString()));
        return;
    }
    totalBytes = m_qpLocalFile->size();
    qDebug()<<"totalBytes "<<totalBytes;
    QDataStream sendOut(&outBlock, QIODevice::WriteOnly);
    sendOut.setVersion(QDataStream::Qt_4_7);
    time.start();  // 开始计时
    QString curFile = fileName.right(fileName.size() - fileName.lastIndexOf('/')-1);
    sendOut << qint64(0) << qint64(0)<< curFile;
    totalBytes += outBlock.size();
    sendOut.device()->seek(0);
    sendOut << totalBytes << qint64((outBlock.size() - sizeof(qint64)*2));
    bytesTobeWrite = totalBytes - clntConn->write(outBlock);
    outBlock.resize(0);

}

void CServer::updClntProgress(qint64 numBytes)
{
   // qDebug()<<"numBytes"<<numBytes;
    qApp->processEvents();
    bytesWritten += (int)numBytes;
    if (bytesTobeWrite > 0) {
        outBlock = m_qpLocalFile->read(qMin(bytesTobeWrite, payloadSize));
        bytesTobeWrite -= (int)clntConn->write(outBlock);
        outBlock.resize(0);
    } else {
        m_qpLocalFile->close();
    }
//    ui->progressBar->setMaximum(totalBytes);
//    ui->progressBar->setValue(bytesWritten);

    float useTime = time.elapsed();
    double speed = bytesWritten / useTime;
//    ui->sStatusLbl->setText(tr("已发送 %1MB (%2MB/s) \n共%3MB 已用时:%4秒\n估计剩余时间：%5秒")
//                   .arg(bytesWritten / (1024*1024))
//                   .arg(speed*1000 / (1024*1024), 0, 'f', 2)
//                   .arg(totalBytes / (1024 * 1024))
//                   .arg(useTime/1000, 0, 'f', 0)
//                   .arg(totalBytes/speed/1000 - useTime/1000, 0, 'f', 0));
    QString statusText;
    statusText=QString("已发送 %1MB (%2MB/s) 共%3MB 已用时:%4秒")
                       .arg(bytesWritten / (1024*1024.0),3,'f',2)
                       .arg(speed*1000 / (1024*1024), 3, 'f', 2)
                       .arg(totalBytes / (1024 * 1024.0), 3, 'f', 2)
                       .arg(useTime/1000, 0, 'f', 0);
  //  qDebug()<<"statusText"<<statusText;
    emit sigSendStatusInfo(statusText);
    emit sigSendProgress(1.0*bytesWritten / totalBytes * 100.0);
    qDebug()<<"bytesWritten ="<<bytesWritten<<"totalBytes"<<totalBytes;
    qDebug()<<"11Progress ="<<(1.0*bytesWritten / totalBytes * 100.0);
    if(bytesWritten == totalBytes) {
        m_qpLocalFile->close();
        m_qpTcpSrv->close();

    }
}
