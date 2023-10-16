
#include "mainwindow.h"

#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("Image parser");
    w.setMinimumSize(1024, 768);
    w.show();

    return a.exec();
}
