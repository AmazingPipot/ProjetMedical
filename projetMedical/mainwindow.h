#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QOpenGLWidget>
#include <QWidget>
#include <QLabel>
#include <QGridLayout>
#include "parserhdr.h"
#include <iostream>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(ParserHDR parse, QWidget *parent = 0);

    QWidget *widget = new QWidget;
    QPixmap *pixmap_img1;
    QPixmap *pixmap_img2;
    QPixmap *pixmap_img3;
    QPixmap *pixmap_imgPrecise;
    ParserHDR Parser;
    int sliceXY = 0;
    int sliceYZ = 0;
    int sliceXZ = 0;
    QLabel  *labelPrecis  = new QLabel;
    QLabel  *label1  = new QLabel;
    QLabel  *label2  = new QLabel;
    QLabel  *label3  = new QLabel;

    QString filename;

    int MouseX = 0;
    int MouseY = 0;
    int MouseX2 = 0;
    int MouseY2 = 0;

    ~MainWindow();

private:
    Ui::MainWindow *ui;
    void wheelEvent(QWheelEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void affichageImages();
    void clickImage();
    void gestionSliceImage(int index);
    void coordonneZoom();
};

#endif // MAINWINDOW_H
