#include <QApplication>
#include <QTextCodec>
#include <QMetaType>
#include "MainWidget.h"
//#include <arpa/inet.h>
int main(int argc,char** argv)
{
    QApplication app(argc,argv);
    //QTextCodec::setCodecForCStrings (QTextCodec::codecForName("UTF-8"));//汉字显示
    //QTextCodec::setCodecForTr (QTextCodec::codecForName("UTF-8"));//汉字显示
  //  in_addr addr1;
//    unsigned long ip =inet_addr("192.168.10.3");
//    memcpy(&addr1,&ip,4);
//    qDebug()<<inet_ntoa(addr1);
    CMainWidget mainWidget;
    return app.exec();
}

