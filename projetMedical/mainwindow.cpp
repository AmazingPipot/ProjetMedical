#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMouseEvent>
#include <QGraphicsView>
#include <QTimer>


MainWindow::MainWindow(ParserHDR parse, QString fil, QString nam1, QString nam2, int axe, QWidget *parent) :
    QMainWindow(parent),
    Parser(parse),
    Axe(axe),
    filename(fil),
    name(nam1),
    name2(nam2),
    ui(new Ui::MainWindow)
{
    slice = Axe/2;
    ui->setupUi(this);
    affichageImages();
    //this->setWindowTitle(name+name2+" : "+QString::number(slice)+" / "+QString::number(Axe));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::mousePressEvent(QMouseEvent *e)
{
    if (name2 == "")
    {
        if (e->button()==Qt::LeftButton)
        {
            MouseX2 = (int)e->localPos().x();
            MouseY2 = (int)e->localPos().y();

            fprintf(stderr,"position clic %d %d \n",MouseX2,MouseY2);
        }
    }
}
void MainWindow::mouseReleaseEvent(QMouseEvent *e)
{

    if (e->button()==Qt::LeftButton)
    {
        MouseX = (int)e->localPos().x();
        MouseY = (int)e->localPos().y();
        fprintf(stderr,"position relachement %d %d \n",MouseX, MouseY);

        if (name2 != "")
        {
            coordonneZoom();
        }
        else{
            clickImage();
        }
    }
}
void MainWindow::wheelEvent(QWheelEvent *e){
    if (name2 == "")
    {
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
}

void MainWindow::affichageImages()
{
    pixmap_img1 = new QPixmap(filename);

    if (name2 == "")
    {
        label1->setPixmap(pixmap_img1->scaled(pixmap_img1->width()/5,pixmap_img1->height()/5,Qt::KeepAspectRatio));
        QGridLayout *gridLayout = new QGridLayout(widget);

        gridLayout->addWidget(label1, 0, 0);
        this->setWindowTitle(name+name2+" : "+QString::number(slice)+" / "+QString::number(Axe));

        this->resize(pixmap_img1->width()/5,pixmap_img1->height()/5);
    }
    else
    {
        label1->setPixmap(pixmap_img1->scaled(pixmap_img1->width(),pixmap_img1->height(),Qt::KeepAspectRatio));
        QGridLayout *gridLayout = new QGridLayout(widget);

        gridLayout->addWidget(label1, 0, 0);
        this->setWindowTitle(name+name2+" : "+QString::number(slice)+" / "+QString::number(Axe));

        this->resize(pixmap_img1->width(),pixmap_img1->height());
    }
}

void MainWindow::gestionSliceImage(int index)
{
    int positionMouseX = MouseX-this->pos().x();
    int positionMouseY = MouseY-this->pos().y();

    QImage image;
    if ((positionMouseX > label1->pos().x() && positionMouseX < label1->pos().x()+label1->width())
    && (positionMouseY > label1->pos().y() && positionMouseY < label1->pos().y()+label1->height()))
    {
         fprintf(stderr,"JE SUIS DANS LE LABEL 1 ");
         slice+= index;
         if (filename == "sliceXY")
         {
             if(!Parser.getImageXY(slice, image))
             {
                 std::cerr << "Unable to get imageXY " <<slice << std::endl;
                 exit(1);
             }
         }
         else if (filename == "sliceYZ")
         {
             if(!Parser.getImageYZ(slice, image))
             {
                 std::cerr << "Unable to get imageYZ " <<slice << std::endl;
                 exit(1);
             }
         }
         else if (filename == "sliceXZ")
         {
             if(!Parser.getImageXZ(slice, image))
             {
                 std::cerr << "Unable to get imageXZ " <<slice<< std::endl;
                 exit(1);
             }
         }
         image.save(filename);
         affichageImages();
         //this->setWindowTitle(name+" : "+QString::number(slice)+" / "+QString::number(Axe));
    }

}

void MainWindow::clickImage()
{
    QImage image;
    if (name2 == "")
    {
        if ((MouseX > label1->pos().x() && MouseX < label1->pos().x()+label1->width())
        && (MouseY > label1->pos().y() && MouseY < label1->pos().y()+label1->height()))
        {
            Window->rechargeInformatin(filename,name, Axe, slice);
        }
    }
}

void MainWindow::rechargeInformatin(QString fil, QString nam1, int axe, int sli)
{
    slice = sli;
    Axe = axe;
    filename = fil;
    name = nam1;
    affichageImages();

}
void MainWindow::coordonneZoom()
{
    QImage image;
    if ((MouseX > label1->pos().x() && MouseX < label1->pos().x()+label1->width())
    && (MouseY > label1->pos().y() && MouseY < label1->pos().y()+label1->height()))
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
             if (MouseX2 < label1->pos().x())
             {
                 MouseX2 = label1->pos().x();
             }
             if (MouseY2 < label1->pos().y())
             {
                 MouseY2 = label1->pos().y();
             }
             /*if (MouseX > labelPrecis->pos().x() + pixmap_imgPrecise->width())
             {
                 MouseX = labelPrecis->pos().x() + pixmap_imgPrecise->width();
             }
             if (MouseY > labelPrecis->pos().y() + pixmap_imgPrecise->height())
             {
                 MouseY = labelPrecis->pos().y() + pixmap_imgPrecise->height();
             }*/

             X1 = MouseX2 - label1->pos().x();
             Y1 = MouseY2 - label1->pos().y();
             X2 = MouseX;
             Y2 = MouseY;
             W = MouseX - MouseX2;
             H = MouseY - MouseY2;

             if(!Parser.getImageXY(slice, QRect(X1, Y1, X2, Y2), image))
             {
                 std::cerr << "Unable to get rect imageYZ " << Parser.width / 2 << std::endl;
                 exit(1);
             }
             image.save("sliceYZrect2.jpg");
             //fprintf(stderr,"position rectangle zoom \n X %d Y %d X2 %d Y2 %d \n W %d H %d \n ",X1,Y1,X2,Y2,W,H);
         }
         image.save("sliceZoom.jpg");
         filename = "sliceZoom.jpg";
         affichageImages();

    }
}
