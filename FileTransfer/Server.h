#ifndef SERVER_H
#define SERVER_H

#include <QWidget>
#include <QFile>
#include <QTime>
class QTcpSocket;
class QTcpServer;
class CServer : public QWidget
{
    Q_OBJECT
public:
    explicit CServer(QWidget *parent = 0);
    
signals:
    void sndFileName(QString addr,QString filename);
    void sigSendStatusInfo(QString statusText);
    void sigSendProgress(double value);
public slots:
    void sltSendFileContent(QString addr,unsigned short port,QString fileofname);
    void sndMsg();
private:
    void initSrv();
private slots:

    void updClntProgress(qint64 numBytes);
private:
    QString m_qReceiveAddr;
    qint16 m_nRecvPort;
    QTcpServer *m_qpTcpSrv;
    QString fileName;
    QString theFileName;
    QFile *m_qpLocalFile;

    qint64 totalBytes;
    qint64 bytesWritten;
    qint64 bytesTobeWrite;
    qint64 payloadSize;
    QByteArray outBlock;

    QTcpSocket *clntConn;

    QTime time;
};

#endif // SERVER_H
