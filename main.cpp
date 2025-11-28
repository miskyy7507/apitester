#include <QApplication>

#include "mainwindow.h"
#include "CurlGlobalContext.h"


int main(int argc, char *argv[])
{
    CurlGlobalContext curl;

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    int qt_app_return = a.exec();

    return qt_app_return;
}
