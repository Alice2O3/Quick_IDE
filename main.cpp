#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    View_Model::getInstance();
    MainWindow w;
    w.show();

    return a.exec();
}
