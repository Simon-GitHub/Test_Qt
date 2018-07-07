#include <QApplication>
#include "TestDialog.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    TestDialog w;
    w.show();

    return a.exec();
}