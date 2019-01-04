#include "mainwindow.h"
#include <QApplication>

#include <iostream>
#include "parserhdr.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //MainWindow w;
    //w.show();

    ParserHDR parser;
    //parser.load("/auto_home/qleroit/IMAGERIE MEDICAL 3D/TP5/brain2.small");
    parser.load("D:/fds/master2/imagerie_medicale/reconstruction_bin2");

    QImage image;
    if(!parser.getImageXY(256, image))
    {
        std::cerr << "Unable to get imageXY 256" << std::endl;
        exit(1);
    }
    image.save("sliceXY.jpg");

    if(!parser.getImageYZ(256, image))
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
    image.save("sliceXZ.jpg");

    return 0;

    return a.exec();
}
