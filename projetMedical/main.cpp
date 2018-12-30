#include "mainwindow.h"
#include <QApplication>

#include "parserhdr.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    ParserHDR *parse = new ParserHDR();
    parse->lectureHDR("/auto_home/qleroit/IMAGERIE MEDICAL 3D/TP5/brain2.small.hdr");
    return a.exec();
}
