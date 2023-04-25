#include "mainwindow.h"
#include <QMenuBar>
#include <QKeySequence>
#include <QFileDialog>
#include <QLayout>
#include <iostream>
#include <QGraphicsItem>

void scaleImage(const QImage& src,QImage& dst,float sx, float sy){
    int w = src.width() *sx;
    int h = src.height() *sy;
    dst = QImage(w,h,src.format());
    for(int y=0;y<dst.height();y++){
        for(int x=0;x<dst.width();x++){
            int xs = floor(x/sx);
            int ys= floor(y/sy);
            dst.setPixel(x,y,src.pixel(xs,ys));
        }
    }
}

void rotateImage(const QImage& src,QImage& dst,float a){
    int w = src.width();
    int h = src.height();
    dst = QImage(w,h,src.format());
    dst.fill(0);
    float aR = qDegreesToRadians(a);
    float cs = cos(-aR);
    float sn = sin(-aR);
    for(int y=0;y<dst.height();y++){
        for(int x=0;x<dst.width();x++){
            float xs = x*cs - y*sn;
            float ys = x*sn + y*cs;
            int xd = floor(xs);
            int yd = floor(ys);
            QRgb pixel = src.pixel(xd,yd);
            dst.setPixel(x,y,pixel);
        }
    }
}

void translationImage(const QImage& src,QImage& dst,float tx, float ty){
    int w = src.width();
    int h = src.height();
    dst = QImage(w,h,src.format());
    dst.fill(0);
    for(int y=0;y<dst.height();y++){
        for(int x=0;x<dst.width();x++){
            float xs = x+tx;
            float ys = y+ty;
            int xd = floor(xs);
            int yd = floor(ys);
            QRgb pixel = src.pixel(xd,yd);
            dst.setPixel(x,y,pixel);
        }
    }
}

void shearImage(const QImage& src,QImage& dst,float ax, float ay){
    int w = src.width();
    int h = src.height();
    dst = QImage(w,h,src.format());
    dst.fill(0);
    for(int y=0;y<dst.height();y++){
        for(int x=0;x<dst.width();x++){
            float xs = x+y*ax;
            float ys = x*ay+y;
            int xd = floor(xs);
            int yd = floor(ys);
            QRgb pixel = src.pixel(xd,yd);
            dst.setPixel(x,y,pixel);
        }
    }
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QMenu* menuFile = menuBar()->addMenu("File");
    menuFile->addAction("Open",this, SLOT(openImage()), QKeySequence("Ctrl+O"));

    imageLabel = new QLabel();

    setCentralWidget(new QWidget());
    QVBoxLayout* layout = new QVBoxLayout();
    centralWidget() -> setLayout(layout);

    sliderScaleX = new QSlider(Qt::Orientation::Horizontal);
    sliderScaleX->setMinimum(-100);
    sliderScaleX->setMaximum(100);
    sliderScaleX->singleStep();
    connect(sliderScaleX,&QSlider::valueChanged,this,&MainWindow::slotScale);

    sliderScaleY = new QSlider(Qt::Orientation::Horizontal);
    sliderScaleY->setMinimum(-100);
    sliderScaleY->setMaximum(100);
    sliderScaleY->singleStep();
    connect(sliderScaleY,&QSlider::valueChanged,this,&MainWindow::slotScale);

    sliderRotate = new QSlider(Qt::Orientation::Horizontal);
    sliderRotate->setMinimum(-180);
    sliderRotate->setMaximum(180);
    sliderRotate->singleStep();
    connect(sliderRotate,SIGNAL(valueChanged(int)),this,SLOT(slotRotate(int)));

    sliderTranslationX = new QSlider(Qt::Orientation::Horizontal);
    sliderTranslationX->setMinimum(-360);
    sliderTranslationX->setMaximum(360);
    sliderTranslationX->singleStep();
    connect(sliderTranslationX,&QSlider::valueChanged,this,&MainWindow::slotTranslation);

    sliderTranslationY = new QSlider(Qt::Orientation::Horizontal);
    sliderTranslationY->setMinimum(-360);
    sliderTranslationY->setMaximum(360);
    sliderTranslationY->singleStep();
    connect(sliderTranslationY,&QSlider::valueChanged,this,&MainWindow::slotTranslation);

    sliderShearX = new QSlider(Qt::Orientation::Horizontal);
    sliderShearX->setMinimum(-360);
    sliderShearX->setMaximum(360);
    sliderShearX->singleStep();
    connect(sliderShearX,&QSlider::valueChanged,this,&MainWindow::slotShear);

    sliderShearY = new QSlider(Qt::Orientation::Horizontal);
    sliderShearY->setMinimum(-360);
    sliderShearY->setMaximum(360);
    sliderShearY->singleStep();
    connect(sliderShearY,&QSlider::valueChanged,this,&MainWindow::slotShear);

    scaleLabelX = new QLabel("Skalowanie X");
    scaleLabelY = new QLabel("Skalowanie Y");
    rotateLabel = new QLabel("Rotacja");
    translationXLabel = new QLabel("Przesunięcie X");
    translationYLabel = new QLabel("Przesunięcie Y");
    shearXLabel = new QLabel("Pochylenie X");
    shearYLabel = new QLabel("Pochylenie Y");

    layout -> addWidget(scaleLabelX);
    layout ->addWidget(sliderScaleX);
    layout -> addWidget(scaleLabelY);
    layout ->addWidget(sliderScaleY);
    layout -> addWidget(rotateLabel);
    layout ->addWidget(sliderRotate);
    layout -> addWidget(translationXLabel);
    layout ->addWidget(sliderTranslationX);
    layout -> addWidget(translationYLabel);
    layout ->addWidget(sliderTranslationY);
    layout -> addWidget(shearXLabel);
    layout ->addWidget(sliderShearX);
    layout -> addWidget(shearYLabel);
    layout ->addWidget(sliderShearY);
    layout-> addWidget(imageLabel);
}

MainWindow::~MainWindow()
{
}

void MainWindow::openImage(){
    QString name = QFileDialog::getOpenFileName();
    if(!name.isEmpty()){
        mainImage.load(name);
        procImage = mainImage;
        imageLabel->setPixmap(QPixmap::fromImage(mainImage));
    }
}

void MainWindow::slotScale(){
    int valueX = sliderScaleX->value();
    int valueY = sliderScaleY->value();

    float sx = 1.0f;
    if(valueX>0) sx=1.0f+valueX/10.0f; //powiekszenie :s <1,10>
    else sx = (valueX+100.0f) / 100.0f; //pomniejszenie s <0,1>

    float sy = 1.0f;
    if(valueY>0) sy=1.0f+valueY/10.0f; //powiekszenie :s <1,10>
    else sy = (valueY+100.0f) / 100.0f; //pomniejszenie s <0,1>

    scaleImage(mainImage, procImage, sx, sy);
    imageLabel->setPixmap(QPixmap::fromImage(procImage));
}

void MainWindow::slotRotate(int value){
    rotateImage(mainImage, procImage, value);
    imageLabel->setPixmap(QPixmap::fromImage(procImage));
}

void MainWindow::slotTranslation(){
    int valueX = sliderTranslationX->value();
    int valueY = sliderTranslationY->value();
    translationImage(mainImage, procImage, valueX, valueY);
    imageLabel->setPixmap(QPixmap::fromImage(procImage));
}

void MainWindow::slotShear(){
    int valueX = sliderShearX->value();
    int valueY = sliderShearY->value();
    float ax = valueX/360.0f;
    float ay = valueY/360.0f;
    shearImage(mainImage, procImage, ax, ay);
    imageLabel->setPixmap(QPixmap::fromImage(procImage));
}
