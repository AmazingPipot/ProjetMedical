#include "mainwindow.h"
#include <QApplication>
#include <iostream>
#include "parserhdr.h"
#include <QApplication>
#include <QtGui>
#include <QLabel>
#include <QGridLayout>
#include <QMouseEvent>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //ParserHDR parser;

    //parser.readHDR("/auto_home/qleroit/IMAGERIE MEDICAL 3D/PROJET/DATA.hdr");
    //parser.readIMG("/auto_home/qleroit/IMAGERIE MEDICAL 3D/PROJET/DATA.img");
    //parser.readHDR("D:/fds/master2/imagerie_medicale/reconstruction_bin2.hdr");
    //parser.readIMG("D:/fds/master2/imagerie_medicale/reconstruction_bin2.img");

    ParserHDR parser;
    //parser.load("/auto_home/qleroit/IMAGERIE MEDICAL 3D/TP5/brain2.small");

    //if(!parser.load("D:/fds/master2/imagerie_medicale/reconstruction_bin"))
    //if(!parser.load("D:/Google Drive/fds/M2_IMAGINA/HMIN318M_Imagerie_medicale_et_3D/TP/tp1/MR_head.Coronal"))
    if(!parser.load("/auto_home/qleroit/IMAGERIE MEDICAL 3D/PROJET/DATA"))
    {
        std::cerr << "Unable to load analyze7.5 file" << std::endl;
        exit(1);
    }

    QImage image;
    if(!parser.getImageXY(0, image))
    {
        std::cerr << "Unable to get imageXY 255" << std::endl;
        exit(1);
    }
    image.save("sliceXY.jpg");

    /*if(!parser.getImageYZ(256, image))
    {
        std::cerr << "Unable to get imageYZ 256" << std::endl;
        exit(1);
    }
    image.save("sliceYZ.jpg");

    if(!parser.getImageXZ(256, image))
    {
        std::cerr << "Unable to get imageXZ 256" << std::endl;
        exit(1);
    }
    image.save("sliceXZ.jpg");*/

    MainWindow w(parser);
    w.setWindowTitle("Imagerie Medicale");
    w.setCentralWidget(w.widget);
    //w.setMinimumSize(1024,768);
    //w.setMaximumSize(1024,768);
    //w.update();
    w.show();


    return a.exec();
}
