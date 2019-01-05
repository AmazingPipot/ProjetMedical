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
    if (name2 == "")
    {
        Zoom = 5;
    }
    else
    {
        Zoom = 2;
    }
    ui->setupUi(this);
    affichageImages();
    //paintEvent();
    //this->setWindowTitle(name+name2+" : "+QString::number(slice)+" / "+QString::number(Axe));
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
void MainWindow::keyReleaseEvent(QKeyEvent *e)
{
    switch(e->key()){
        case Qt::Key_Plus:
        if (name2 != "")
        {
            Zoom++;
            fprintf(stderr,"%d \n",Zoom);
        }
        else
        {
            slice += 20;
            MouseX = cursor().pos().x();
            MouseY = cursor().pos().y();
            gestionSliceImage(0);
        }
        break;
        case Qt::Key_Minus:
        if (name2 != "")
        {
            Zoom--;
            fprintf(stderr,"%d \n",Zoom);
        }
        else
        {
            slice -= 20;
            MouseX = cursor().pos().x();
            MouseY = cursor().pos().y();
            gestionSliceImage(0);
        }
        break;
        case Qt::Key_Left:
            GD -= 200/Zoom;
        break;
        case Qt::Key_Right:
            GD += 200/Zoom;;
        break;
        case Qt::Key_Up:
            HB -= 200/Zoom;
            fprintf(stderr,"%d \n",Zoom);
        break;
        case Qt::Key_Down:
            HB += 200/Zoom;
            fprintf(stderr,"%d \n",Zoom);
        break;
        default : break;
    }
    if (Zoom < 1)
    {
        Zoom = 1;
    }
    if (name2 != "")
    {

    affichageImages();
    }
}
void MainWindow::paintEvent(QPaintEvent *e) {
    if (name2 != "")
    {
        QRectF source;
        QRectF target(0,0,pixmap_img1->width()/2,pixmap_img1->height()/2);
        if (Zoom == 1)
        {
            source.adjust(0,0,pixmap_img1->width(),pixmap_img1->height());
            ZoomX1 = 0;
            ZoomX2 = pixmap_img1->width();
            ZoomY1 = 0;
            ZoomY1 = pixmap_img1->height();
            HB = 0;
            GD = 0;
        }
        else
        {
            int w1 = pixmap_img1->width()/Zoom;
            int w2 = pixmap_img1->width()/2;

            ZoomX1 = w2-w1+GD;
            ZoomX2 = w2+w1+GD;
            w1 = pixmap_img1->height()/Zoom;
            w2 = pixmap_img1->height()/2;
            ZoomY1 = w2-w1+HB;
            ZoomY2 = w2+w1+HB;

            if (ZoomX1 < 0)
            {
                ZoomX1 = 0;
            }
            if (ZoomX2 > pixmap_img1->width())
            {
                ZoomX2 = pixmap_img1->width();
            }
            if (ZoomY1 < 0)
            {
                ZoomY1 = 0;
            }
            if (ZoomY2 > pixmap_img1->height())
            {
                ZoomY2 = pixmap_img1->height();
            }
            source.adjust(ZoomX1,ZoomY1,ZoomX2,ZoomY2);
        }
        QPainter painter(this);
        painter.drawPixmap(target,*pixmap_img1,source);

       // painter.end();
    }
    else
    {
        QRectF source(0,0,pixmap_img1->width(),pixmap_img1->height());
        QRectF target(0,0,pixmap_img1->width()/Zoom,pixmap_img1->height()/Zoom);
        QPainter painter(this);
        painter.drawPixmap(target,*pixmap_img1,source);
    }
    fprintf(stderr,"ZOOM");
    //update();

}
void MainWindow::affichageImages()
{
    pixmap_img1 = new QPixmap(filename);

    if (name2 == "")
    {
        //this->setCentralWidget(widget);
        //this.setCentralWidget(widget);
        //label1->setPixmap(pixmap_img1->scaled(pixmap_img1->width()/5,pixmap_img1->height()/5,Qt::KeepAspectRatio));
        //QGridLayout *gridLayout = new QGridLayout(this);

        //gridLayout->addWidget(label1, 0, 0);
        this->setWindowTitle(name+name2+" : "+QString::number(slice)+" / "+QString::number(Axe));

        this->resize(pixmap_img1->width()/Zoom,pixmap_img1->height()/Zoom);
        //this->show();
    }
    else
    {
        //label1->setPixmap(pixmap_img1->scaled(pixmap_img1->width(),pixmap_img1->height(),Qt::KeepAspectRatio));
        QGridLayout *gridLayout = new QGridLayout(widget);
        gridLayout->addWidget(label1, 1, 0);
        this->setWindowTitle(name+name2+" : "+QString::number(slice)+" / "+QString::number(Axe));
        //fprintf(stderr,"RESIZE %d %d",pixmap_img1->width(),pixmap_img1->height());
        this->resize(pixmap_img1->width()/2,pixmap_img1->height()/2);
        //label1->
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
             fprintf(stderr,"JE SUIS DANS LE LABEL 1&&&&&&& ");
             image.save(filename);
         }
         else if (filename == "sliceYZ")
         {
             if(!Parser.getImageYZ(slice, image))
             {
                 std::cerr << "Unable to get imageYZ " <<slice << std::endl;
                 exit(1);
             }
             fprintf(stderr,"JE SUIS DANS LE LABEL 1ééééééé ");
             image.save(filename);
         }
         else if (filename == "sliceXZ")
         {
             if(!Parser.getImageXZ(slice, image))
             {
                 std::cerr << "Unable to get imageXZ " <<slice<< std::endl;
                 exit(1);
             }
             fprintf(stderr,"JE SUIS DANS LE LABEL 1rrrrrr");
             image.save(filename);
         }

         affichageImages();
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
    /*QImage image;

    fprintf(stderr,"pos %d %d \n",label1->pos().y()+pixmap_img1->height(),label1->pos().y()+label1->height());

    fprintf(stderr,"pos mouse %d %d \n",MouseX,MouseY);
    if ((MouseX > label1->pos().x() && MouseX < label1->pos().x()+pixmap_img1->width())
    && (MouseY > label1->pos().y() && MouseY < label1->pos().y()+pixmap_img1->height()))
    {
         fprintf(stderr,"JE SUIS DANS LE LABEL PRECIS ");
         int X1, X2;
         int Y1, Y2;
         int W;
         int H;

         if (abs(MouseX-MouseX2) > 1 && abs(MouseY-MouseY2) > 1)
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
             if (MouseX2 < label1->pos().x()+9)
             {
                 MouseX2 = label1->pos().x()+9;
             }
             if (MouseY2 < label1->pos().y()+9)
             {
                 MouseY2 = label1->pos().y()+9;
             }

             if (MouseX > label1->pos().x() + pixmap_img1->width())
             {
                 MouseX = label1->pos().x() + pixmap_img1->width();
             }
             if (MouseY > label1->pos().y() + pixmap_img1->height())
             {
                 MouseY = label1->pos().y() + pixmap_img1->height();
             }

             X1 = MouseX2 - label1->pos().x()-9;
             Y1 = MouseY2 - label1->pos().y()-9;
             X2 = MouseX-9;
             Y2 = MouseY-9;
             W = MouseX - MouseX2;
             H = MouseY - MouseY2;

             if (filename == "sliceXY")
             {
                 if(!Parser.getImageXY(slice, QRect(X1, Y1, X2, Y2), image))
                 {
                     std::cerr << "Unable to get rect imageXY " << slice << std::endl;
                     exit(1);
                 }
             }
             else if (filename == "sliceYZ")
             {
                 if(!Parser.getImageYZ(slice, QRect(X1, Y1, X2, Y2), image))
                 {
                     std::cerr << "Unable to get rect imageYZ " << slice << std::endl;
                     exit(1);
                 }
             }
             else if (filename == "sliceXZ")
             {
                 if(!Parser.getImageXZ(slice, QRect(X1, Y1, X2, Y2), image))
                 {
                     std::cerr << "Unable to get rect imageXZ " << slice << std::endl;
                     exit(1);
                 }
             }
             //image.save("sliceYZrect2.jpg");
             fprintf(stderr,"position rectangle zoom \n X %d Y %d X2 %d Y2 %d \n W %d H %d \n ",X1,Y1,X2,Y2,W,H);
         }
         image.save("sliceZoom.jpg");
         filename = "sliceZoom.jpg";
         affichageImages();

    }*/
}
