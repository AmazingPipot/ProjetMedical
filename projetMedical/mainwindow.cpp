#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "parserhdr.h"
#include <QMouseEvent>
#include <QGraphicsView>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    //gestionEcran();
    QPixmap pix(":/logoum.png");

    //QWidget *widget = new QWidget;

    pixmap_img1 = new QPixmap(":/logoum.png");
    pixmap_img2 = new QPixmap(":/logoum.png");
    pixmap_img3 = new QPixmap(":/logoum.png");
    //QPixmap *pixmap_img = new QPixmap("slice.jpg");

    //label->setPixmap(*pixmap_img);
    label1->setPixmap(*pixmap_img1);
    label2->setPixmap(*pixmap_img2);
    label3->setPixmap(*pixmap_img3);


    QGridLayout *gridLayout = new QGridLayout(widget);
    gridLayout->addWidget(label1, 0, 0);
    gridLayout->addWidget(label2, 0, 1);
    gridLayout->addWidget(label3, 1, 0);

    //gridLayout->addWidget(label, 1, 1);


    this->setMouseTracking(false);

    update();
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::wheelEvent(QWheelEvent *e){
    if (e->orientation()==Qt::Vertical)
        {
            MouseX = cursor().pos().x();
            MouseY = cursor().pos().y();
            if (e->delta()>0){
               gestionEcran();
              //printf("Dans un sens");
            }
            else{
               //printf("Dans autre sens");
            }
        }
    //fprintf(stderr,"Position souris %d   %d\n",cursor().pos().x(),cursor().pos().y());
    e->accept();
}
void MainWindow::gestionEcran()
{
    int positionMouseX = MouseX-this->pos().x();
    int positionMouseY = MouseY-this->pos().y();


    if ((positionMouseX > label1->pos().x() && positionMouseX < label1->pos().x()+label1->width())
    && (positionMouseY > label1->pos().y() && positionMouseY < label1->pos().y()+label1->height()))
    {
         fprintf(stderr,"JE SUIS DANS LE LABEL 1 ");
    }
    else if ((positionMouseX > label2->pos().x() && positionMouseX < label2->pos().x()+label2->width())
    && (positionMouseY > label2->pos().y() && positionMouseY < label2->pos().y()+label2->height()))
     {
          fprintf(stderr,"JE SUIS DANS LE LABEL 2 ");
     }
    else if ((positionMouseX > label3->pos().x() && positionMouseX < label3->pos().x()+label3->width())
    && (positionMouseY > label3->pos().y() && positionMouseY < label3->pos().y()+label3->height()))
     {
          fprintf(stderr,"JE SUIS DANS LE LABEL 3 ");
     }


}
