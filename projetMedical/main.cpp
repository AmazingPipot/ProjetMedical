#include "mainwindow.h"
#include "parserhdr.h"
#include <QApplication>
#include <QtGui>
#include <QLabel>
#include <QGridLayout>
#include <QMouseEvent>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    ParserHDR parser;

    parser.readHDR("/auto_home/qleroit/IMAGERIE MEDICAL 3D/PROJET/DATA.hdr");
    parser.readIMG("/auto_home/qleroit/IMAGERIE MEDICAL 3D/PROJET/DATA.img");
    //parser.readHDR("D:/fds/master2/imagerie_medicale/reconstruction_bin2.hdr");
    //parser.readIMG("D:/fds/master2/imagerie_medicale/reconstruction_bin2.img");

    MainWindow w(parser);
    w.setWindowTitle("Imagerie Medicale");
    w.setCentralWidget(w.widget);
    w.setMinimumSize(1024,768);
    w.setMaximumSize(1024,768);
    w.update();
    w.show();

    return a.exec();
}
