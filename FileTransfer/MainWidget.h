#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QGroupBox>
#include <QUdpSocket>
#include "Server.h"
#include "Client.h"
#include "ProgressBar.h"

enum MsgType{Msg, UsrEnter, UsrLeft, FileName,Accept, Refuse};
class CMainWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CMainWidget(QWidget *parent = 0);
    void SndMsg(MsgType type, QString srvaddr="");
    QString GetIP();
    void InitSetSocket();
    void ReInitSocket();
signals:
    void sigSendFile(QString addr,unsigned short port,QString filename);
    void sigRecvDataSignal(QByteArray);
public slots:
    void SltSendStatusInfo(QString statusText);
    void SltSendProgress(double value);
    void SltRecvStatusInfo(QString statusText);
    void SltRecvProgress(double value);
    void DecodePacketData(QByteArray byte_array);
private:
    /*************************************
    * 函数名:createComponent
    * 函数功能：创建控件
    * 输入参数：无
    * 输出参数：无
    * 返回值：无
    **************************************/
    void createComponent();

    /*************************************
    * 函数名:createLayout
    * 函数功能：为控件添加布局
    * 输入参数：无
    * 输出参数：无
    * 返回值：无
    **************************************/
    void createLayout();

    void hasPendingFile(QString srcaddr,QString recvaddr,QString filename);
    bool isExistTheAddr(QString recvaddr);
private slots:
    void sltOpenFile();
    void sltSendFile();
    void sltSetFileName(QString addr,QString filename);
    void sltGetReceiveData();
private:
    QLabel      *m_qpServerIPLbl;
    QLineEdit   *m_qpServerIPEdit;

    QLabel      *m_qpServerPortLbl;
    QLineEdit   *m_qpServerPortEdit;
    QPushButton *m_qpSendFileBtn;
    QLabel      *m_qpSendFileLbl;
    QLineEdit   *m_qpSendFileEdit;
    QPushButton *m_qpSelFileBtn;
    QLabel      *m_qpSendProgressLbl;

    QLabel      *m_qpSendStateLbl;
    QLineEdit   *m_qpSendStateEdit;


    QLabel      *m_qpListenPortLbl;
    QLineEdit   *m_qpListenPortEdit;
    QPushButton *m_qpStartListenBtn;
    QLabel      *m_qpReceiveFileLbl;
    QLineEdit   *m_qpReceiveFileEdit;
    QLabel      *m_qpReceiveProgressLbl;

    QLabel      *m_qpReceiveStateLbl;
    QLineEdit   *m_qpReceiveStateEdit;

    QGroupBox   *m_qpSendFileBox;
    QGroupBox   *m_qpReceiveFileBox;

    CProgressBar     *m_pSendProgress;
    CProgressBar     *m_pRecvProgress;
private:
    QString     m_qSendFileName;
    QString     m_qSendAddr;
    QString     m_qLocalAddr;

    CServer     *m_pServer;
    CClient     *m_pClient;
    QUdpSocket  *m_qpSendUdpSocket;

    unsigned short  m_nPort;
};

#endif // MAINWIDGET_H
