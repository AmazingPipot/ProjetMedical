#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QOpenGLWidget>
#include <QWidget>
#include <QLabel>
#include <QGridLayout>
#include "parserhdr.h"
#include <iostream>
#include <QPainter>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    explicit MainWindow(ParserHDR parse, QString fil, QString nam1, QString nam2, int axe, QWidget *parent = 0);


    MainWindow *Window = this;
    QWidget *widget = new QWidget;
    QPixmap *pixmap_img1;
   /* QPixmap *pixmap_img2;
    QPixmap *pixmap_img3;
    QPixmap *pixmap_imgPrecise;*/
    ParserHDR Parser;
    /*int sliceXY = 0;
    int sliceYZ = 0;
    int sliceXZ = 0;*/
    //QLabel  *labelPrecis  = new QLabel;
    QLabel  *label1  = new QLabel;
    /*QLabel  *label2  = new QLabel;
    QLabel  *label3  = new QLabel;*/

    QString filename;


    int MouseX = 0;
    int MouseY = 0;
    int MouseX2 = 0;
    int MouseY2 = 0;

    QString name = "Detaillée";
    QString name2 = "";

    int Zoom = 2;
    int ZoomX1, ZoomX2, ZoomY1, ZoomY2, HB, GD;
    int slice;
    int Axe;
    QPainter painter;

    ~MainWindow();
    Ui::MainWindow *ui;

private:

    void wheelEvent(QWheelEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void keyReleaseEvent(QKeyEvent *e);
    void paintEvent(QPaintEvent *);

    void affichageImages();
    void clickImage();
    void gestionSliceImage(int index);
    void coordonneZoom();
    void rechargeInformatin(QString fil, QString nam1, int axe, int sli);
};

#endif // MAINWINDOW_H
