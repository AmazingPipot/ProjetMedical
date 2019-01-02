#include "mainwindow.h"
#include <QApplication>

#include "parserhdr.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    w.show();

    ParserHDR parser;
    //parser.readHDR("/auto_home/qleroit/IMAGERIE MEDICAL 3D/TP5/brain2.small.hdr");
    parser.readHDR("D:/fds/master2/imagerie_medicale/reconstruction_bin2.hdr");
    parser.readIMG("D:/fds/master2/imagerie_medicale/reconstruction_bin2.img");

    return a.exec();
}
