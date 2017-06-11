#ifndef CLIENT_H
#define CLIENT_H

#include <QWidget>
#include <QFile>
#include <QHostAddress>
#include <QTcpSocket>
#include <QTime>
class CClient : public QWidget
{
    Q_OBJECT
public:
    explicit CClient(QWidget *parent = 0);
    void setHostAddr(QHostAddress addr);
    void setFileName(QString name);
signals:
    void sigRecvStatusInfo(QString statusText);
    void sigRecvProgress(double value);
public slots:
     void readMsg();
     void displayErr(QAbstractSocket::SocketError);
private:
     void newConn();
private:
     QFile *m_qpSaveFile;
     QTcpSocket *m_qpClientSocket;
     QHostAddress hostAddr;
     QTime time;
private:
     unsigned short m_nClientPort;
     qint64 bytesReceived;
     qint64 fileNameSize;
     qint64 totalBytes;
     QString fileName;
     QByteArray inBlock;
};

#endif // CLIENT_H
