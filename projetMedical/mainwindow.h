#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QOpenGLWidget>
#include <QWidget>
#include <QLabel>
#include <QGridLayout>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void gestionEcran();
    QWidget *widget = new QWidget;
    QPixmap *pixmap_img1;
    QPixmap *pixmap_img2;
    QPixmap *pixmap_img3;

    QLabel  *label  = new QLabel;
    QLabel  *label1  = new QLabel;
    QLabel  *label2  = new QLabel;
    QLabel  *label3  = new QLabel;

    int MouseX = 0;
    int MouseY = 0;

    ~MainWindow();

private:
    Ui::MainWindow *ui;
    void wheelEvent(QWheelEvent *e);
};

#endif // MAINWINDOW_H
