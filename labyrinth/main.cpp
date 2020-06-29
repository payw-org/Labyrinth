#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowFlags(Qt::FramelessWindowHint);
//    w.setWindowFlags(Qt::Tool |
//                     Qt::FramelessWindowHint |
//                     Qt::WindowSystemMenuHint |
//                     Qt::WindowStaysOnTopHint);
//    QPainterPath path;
//    path.addRoundedRect(w.rect(), 3, 3);
//    w.setMask(path.toFillPolygon().toPolygon());
    w.show();

    return a.exec();
}
