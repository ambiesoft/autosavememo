#include "mainwindow.h"
#include <QApplication>

#include "consts.h"
using namespace Consts;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCoreApplication::setOrganizationName(ORGANIZATION);
    QCoreApplication::setApplicationName(APPNAME);
    QCoreApplication::setApplicationVersion(APPVER);

    MainWindow w;
    w.show();

    return a.exec();
}
