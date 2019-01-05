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

    ParserHDR parser;

    //if(!parser.load("D:/fds/master2/imagerie_medicale/reconstruction_bin2"))
    //if(!parser.load("D:/Google Drive/fds/M2_IMAGINA/HMIN318M_Imagerie_medicale_et_3D/TP/tp1/MR_head.Coronal"))
    if(!parser.load("/auto_home/qleroit/IMAGERIE MEDICAL 3D/PROJET/DATA"))
    {
        std::cerr << "Unable to load analyze7.5 file" << std::endl;
        exit(1);
    }

    QImage image;
    if(!parser.getImageXY(parser.depth / 2, image))
    {
        std::cerr << "Unable to get imageXY " << parser.depth / 2 << std::endl;
        exit(1);
    }
    image.save("sliceXY.jpg");

    /*
    /////// tests ///////
    if(!parser.getImageXY(parser.depth / 2, QRect(0, 0, parser.width, parser.height), image))
    {
        std::cerr << "Unable to get imageXY " << parser.depth / 2 << std::endl;
        exit(1);
    }
    image.save("sliceXY.jpg");

    if(!parser.getImageYZ(parser.width / 2, image))
    {
        std::cerr << "Unable to get imageYZ " << parser.width / 2 << std::endl;
        exit(1);
    }
    image.save("sliceYZ.jpg");

    if(!parser.getImageXZ(parser.height / 2, image))
    {
        std::cerr << "Unable to get imageXZ " << parser.width - 1 << std::endl;
        exit(1);
    }
    image.save("sliceXZ.jpg");
    
    if(!parser.getImageXY(parser.depth / 2, QRect(parser.width / 4, parser.height / 4, parser.width / 2, parser.height / 2), image))
    {
        std::cerr << "Unable to get rect imageXY " << parser.depth / 2 << std::endl;
        exit(1);
    }
    image.save("sliceXYrect.jpg");

    if(!parser.getImageYZ(parser.width / 2, QRect(parser.depth / 4, parser.height / 4, parser.depth / 2, parser.height / 2), image))
    {
        std::cerr << "Unable to get rect imageYZ " << parser.width / 2 << std::endl;
        exit(1);
    }
    image.save("sliceYZrect.jpg");


    if(!parser.getImageXZ(parser.height / 2, QRect(parser.width / 4, parser.depth / 4, parser.width / 2, parser.depth / 2), image))
    {
        std::cerr << "Unable to get rect imageXZ " << parser.width / 2 << std::endl;
        exit(1);
    }
    image.save("sliceXZrect.jpg");

    if(!parser.getImageXY(parser.depth / 2, QRect(0, 0, parser.width / 4, parser.height / 4), image))
    {
        std::cerr << "Unable to get rect imageXY " << parser.depth / 2 << std::endl;
        exit(1);
    }
    image.save("sliceXYrect2.jpg");

    if(!parser.getImageYZ(parser.width / 2, QRect(0, 0, parser.depth / 4, parser.height / 4), image))
    {
        std::cerr << "Unable to get rect imageYZ " << parser.width / 2 << std::endl;
        exit(1);
    }
    image.save("sliceYZrect2.jpg");


    if(!parser.getImageXZ(parser.height / 2, QRect(0, 0, parser.width / 4, parser.depth / 4), image))
    {
        std::cerr << "Unable to get rect imageXZ " << parser.width / 2 << std::endl;
        exit(1);
    }
    image.save("sliceXZrect2.jpg");

    return 0;
    
    */

    MainWindow w(parser);
    w.setWindowTitle("Imagerie Medicale");
    w.setCentralWidget(w.widget);
    //w.setMinimumSize(1024,768);
    //w.setMaximumSize(1024,768);
    //w.update();
    w.show();

    return a.exec();
}
