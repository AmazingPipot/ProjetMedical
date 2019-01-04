#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMouseEvent>
#include <QGraphicsView>
#include <QTimer>

MainWindow::MainWindow(ParserHDR parse, QWidget *parent) :
    QMainWindow(parent),
    Parser(parse),
    filename(":/logoum.png"),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    affichageImages();
    //gestionEcran();
    //QPixmap pix(":/logoum.png");

    //QWidget *widget = new QWidget;

    /*pixmap_img1 = new QPixmap(":/logoum.png");
    pixmap_img2 = new QPixmap(":/logoum.png");
    pixmap_img3 = new QPixmap(":/logoum.png");
    pixmap_imgPrecise = new QPixmap(filename);

    label1->setPixmap(*pixmap_img1);
    label2->setPixmap(*pixmap_img2);
    label3->setPixmap(*pixmap_img3);
    labelPrecis->setPixmap(*pixmap_imgPrecise);


    QGridLayout *gridLayout = new QGridLayout(widget);

    gridLayout->addWidget(label1, 0, 0);
    gridLayout->addWidget(label2, 0, 1);
    gridLayout->addWidget(label3, 0, 2);
    gridLayout->addWidget(labelPrecis, 1, 1);*/
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::mousePressEvent(QMouseEvent *e)
{
    if (e->button()==Qt::LeftButton)
    {
        MouseX2 = (int)e->localPos().x();
        MouseY2 = (int)e->localPos().y();

        fprintf(stderr,"position clic %d %d \n",MouseX2,MouseY2);
    }
}
void MainWindow::mouseReleaseEvent(QMouseEvent *e)
{
    if (e->button()==Qt::LeftButton)
    {
        MouseX = (int)e->localPos().x();
        MouseY = (int)e->localPos().y();
        fprintf(stderr,"position relachement %d %d \n",MouseX, MouseY);
        clickImage();
        coordonneZoom();
    }
}
void MainWindow::wheelEvent(QWheelEvent *e){
    if (e->orientation()==Qt::Vertical)
        {
            MouseX = cursor().pos().x();
            MouseY = cursor().pos().y();
            if (e->delta()>0){
               gestionSliceImage(1);
            }
            else{
                gestionSliceImage(-1);
            }
        }
    e->accept();
}

void MainWindow::affichageImages()
{
    pixmap_img1 = new QPixmap(":/logoum.png");
    pixmap_img2 = new QPixmap(":/logoum.png");
    pixmap_img3 = new QPixmap(":/logoum.png");
    pixmap_imgPrecise = new QPixmap(filename);

    label1->setPixmap(*pixmap_img1);
    label2->setPixmap(*pixmap_img2);
    label3->setPixmap(*pixmap_img3);
    labelPrecis->setPixmap(*pixmap_imgPrecise);


    QGridLayout *gridLayout = new QGridLayout(widget);

    gridLayout->addWidget(label1, 0, 0);
    gridLayout->addWidget(label2, 0, 1);
    gridLayout->addWidget(label3, 0, 2);
    gridLayout->addWidget(labelPrecis, 1, 1);


}

void MainWindow::gestionSliceImage(int index)
{
    int positionMouseX = MouseX-this->pos().x();
    int positionMouseY = MouseY-this->pos().y();

    int slice = 0;
    QImage image;
    if ((positionMouseX > label1->pos().x() && positionMouseX < label1->pos().x()+label1->width())
    && (positionMouseY > label1->pos().y() && positionMouseY < label1->pos().y()+label1->height()))
    {
         fprintf(stderr,"JE SUIS DANS LE LABEL 1 ");
         sliceXY+= index;
         if(!Parser.getImageXY(sliceXY, image))
         {
             std::cerr << "Unable to get imageXY 255" << std::endl;
             exit(1);
         }
         image.save("sliceXY.jpg");

    }
    else if ((positionMouseX > label2->pos().x() && positionMouseX < label2->pos().x()+label2->width())
    && (positionMouseY > label2->pos().y() && positionMouseY < label2->pos().y()+label2->height()))
     {
          fprintf(stderr,"JE SUIS DANS LE LABEL 2 ");
          sliceYZ+= index;
          if(!Parser.getImageYZ(sliceYZ, image))
          {
              std::cerr << "Unable to get imageXY 255" << std::endl;
              exit(1);
          }
          image.save("sliceYZ.jpg");
     }
    else if ((positionMouseX > label3->pos().x() && positionMouseX < label3->pos().x()+label3->width())
    && (positionMouseY > label3->pos().y() && positionMouseY < label3->pos().y()+label3->height()))
     {
          fprintf(stderr,"JE SUIS DANS LE LABEL 3 ");
          sliceXZ+= index;
          if(!Parser.getImageXZ(sliceXZ, image))
          {
              std::cerr << "Unable to get imageXY 255" << std::endl;
              exit(1);
          }
          image.save("sliceXZ.jpg");
     }
    affichageImages();
}

void MainWindow::clickImage()
{
    QImage image;
    if ((MouseX > label1->pos().x() && MouseX < label1->pos().x()+label1->width())
    && (MouseY > label1->pos().y() && MouseY < label1->pos().y()+label1->height()))
    {
         fprintf(stderr,"JE SUIS DANS LE LABEL 1 ");
         if(!Parser.getImageXY(sliceXY, image))
         {
             std::cerr << "Unable to get imageXY 255" << std::endl;
             exit(1);
         }
         printf("BIG IMAGE 1");
         image.save("sliceXY.jpg");
         filename = "sliceXY.jpg";

    }
    else if ((MouseX > label2->pos().x() && MouseX < label2->pos().x()+label2->width())
    && (MouseY > label2->pos().y() && MouseY < label2->pos().y()+label2->height()))
     {
          fprintf(stderr,"JE SUIS DANS LE LABEL 2 ");
          if(!Parser.getImageYZ(sliceYZ, image))
          {
              std::cerr << "Unable to get imageXY 255" << std::endl;
              exit(1);
          }
          printf("BIG IMAGE 2");
          image.save("sliceYZ.jpg");
          filename = "sliceYZ.jpg";
     }
    else if ((MouseX > label3->pos().x() && MouseX < label3->pos().x()+label3->width())
    && (MouseY > label3->pos().y() && MouseY < label3->pos().y()+label3->height()))
     {
          fprintf(stderr,"JE SUIS DANS LE LABEL 3 ");
          if(!Parser.getImageXZ(sliceXZ, image))
          {
              std::cerr << "Unable to get imageXY 255" << std::endl;
              exit(1);
          }
          printf("BIG IMAGE 3");
          image.save("sliceXZ.jpg");
          filename = "sliceXZ.jpg";
     }
    affichageImages();
}

void MainWindow::coordonneZoom()
{
    QImage image;
    if ((MouseX > labelPrecis->pos().x() && MouseX < labelPrecis->pos().x()+labelPrecis->width())
    && (MouseY > labelPrecis->pos().y() && MouseY < labelPrecis->pos().y()+labelPrecis->height()))
    {
         fprintf(stderr,"JE SUIS DANS LE LABEL PRECIS ");
         int X1, X2;
         int Y1, Y2;
         int W;
         int H;

         if (abs(MouseX-MouseX2) > 10 && abs(MouseY-MouseY2) > 10)
         {
             if (MouseX < MouseX2)
             {
                 int v = MouseX;
                 MouseX = MouseX2;
                 MouseX2 = v;
             }
             if (MouseY < MouseY2)
             {
                 int v = MouseY;
                 MouseY = MouseY2;
                 MouseY2 = v;
             }
             if (MouseX2 < labelPrecis->pos().x())
             {
                 MouseX2 = labelPrecis->pos().x();
             }
             if (MouseY2 < labelPrecis->pos().y())
             {
                 MouseY2 = labelPrecis->pos().y();
             }
             /*if (MouseX > labelPrecis->pos().x() + pixmap_imgPrecise->width())
             {
                 MouseX = labelPrecis->pos().x() + pixmap_imgPrecise->width();
             }
             if (MouseY > labelPrecis->pos().y() + pixmap_imgPrecise->height())
             {
                 MouseY = labelPrecis->pos().y() + pixmap_imgPrecise->height();
             }*/

             X1 = MouseX2 - labelPrecis->pos().x();
             Y1 = MouseY2 - labelPrecis->pos().y();
             X2 = MouseX;
             Y2 = MouseY;
             W = MouseX - MouseX2;
             H = MouseY - MouseY2;
             fprintf(stderr,"position rectangle zoom \n X %d Y %d X2 %d Y2 %d \n W %d H %d \n ",X1,Y1,X2,Y2,W,H);
         }
         /*if(!Parser.getImageXY(sliceXY, image))
         {
             std::cerr << "Unable to get imageXY 255" << std::endl;
             exit(1);
         }*/
         //image.save("sliceXY.jpg");

    }
}
