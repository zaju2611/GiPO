#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QScrollBar>
#include <QFileDialog>
#include <qmath.h>
#include <QMouseEvent>
#include <iostream>
#include <QVBoxLayout>
#include <QRgb>
#include <QPainter>
#include <QApplication>


void brightness(const QImage& src, QImage& dst, int v){
    for (int y=0; y<dst.height();y++){
        QRgb* pixel_src = (QRgb*)src.scanLine(y);
        QRgb* pixel_dst = (QRgb*)dst.scanLine(y);
        for(int x=0; x<dst.width();x++){

            uchar r = qRed(pixel_src[x]);
            uchar g = qGreen(pixel_src[x]);
            uchar b = qBlue(pixel_src[x]);

            r=clamp<int>(r+v,0,255);
            g=clamp<int>(g+v,0,255);
            b=clamp<int>(b+v,0,255);

            pixel_dst[x]=qRgb(r,g,b);
        }
    }

}

/*
void MainWindow::mousePressEvent(QMouseEvent *e){

    QPoint p = e->pos();
    //std::cout << p.x()<< " "<<p.y() << std::endl;
   if( e -> button() == Qt:: LeftButton){
       // ...
   }
}
void MainWindow::keyPressEvent(QKeyEvent *e){

    if(e-> key() == Qt::Key_0){
        //...
    }
    if (e-> modifiers() == Qt::ShiftModifier){
        //...
    }
}

*/

void gamma(const QImage &src, QImage &dst, int v){
    for (int y=0; y<dst.height();y++){
        QRgb* pixel_src = (QRgb*)src.scanLine(y);
        QRgb* pixel_dst = (QRgb*)dst.scanLine(y);
        for(int x=0; x<dst.width();x++){

            uchar r = qRed(pixel_src[x]);
            uchar g = qGreen(pixel_src[x]);
            uchar b = qBlue(pixel_src[x]);

            float nr=r/255.0f;
            float ng = g/255.0f;
            float nb=b/255.0f;

            if(v>0){
                r = clamp<int>(255.0f *(pow(nr,(100.0f/v))),0,255);
                g = clamp<int>(255.0f *(pow(ng,(100.0f/v))),0,255);
                b = clamp<int>(255.0f *(pow(nb,(100.0f/v))),0,255);
            }
            if (v == 0){
                r =0;
                g =0;
                b =0;
            }

            pixel_dst[x]=qRgb(r,g,b);
        }
    }
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->actionopen, SIGNAL(triggered(bool)),this,SLOT(openImage(bool)));
    connect(ui->horizontalSlider, SIGNAL(valueChanged(int)),this, SLOT(changeBrightness(int)));
    connect(ui->horizontalSlider1, SIGNAL(valueChanged(int)),this, SLOT(gammaCorrection(int)));
}
int *histogram = new int[256];

void MainWindow::drawHistogram(QImage &src){
    for(int i=0 ;i<256;i++){
        histogram[i]=0;
    }

    QBarSet *dataSET = new QBarSet("data");
    dataSET -> setColor(Qt::gray);
    for (int y=0; y<src.height();y++){
        for(int x=0; x<src.width();x++){
            QRgb pixel = src.pixel(x,y);
            int value = qGray(pixel);
            histogram[value]++;
        }
    }

    QBarSeries *series = new QBarSeries();
    for(int i=0; i<256;i++){
        *dataSET << histogram[i];
        series->append(dataSET);
    }

    series -> setBarWidth(1.2);
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("histogram odcieni szarości");
    chart -> setAnimationOptions(QChart::NoAnimation);

    QValueAxis *axisX = new QValueAxis();
    axisX->setRange(0,255);
    axisX->setLabelFormat("%i");
    axisX->setTitleText("Odcień szarości");
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
        axisY->setRange(0, 4000);
        axisY->setTitleText("Liczba wystąpień");
        chart->addAxis(axisY, Qt::AlignLeft);
        series->attachAxis(axisY);

        QChartView *chartView = new QChartView(chart);
        chartView->setRenderHint(QPainter::Antialiasing);
        chartView->resize(800, 600);
        chartView->show();
}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::openImage(bool)
{
    QString filename = QFileDialog::getOpenFileName();
    if (!filename.isNull()){
        orginalImage.load(filename);
        processImage = orginalImage;
        ui->label->setPixmap(QPixmap::fromImage(processImage));
        drawHistogram(processImage);
    }

}

void MainWindow::changeBrightness(int v)
{
    brightness(orginalImage, processImage, v);
    ui->label->setPixmap(QPixmap::fromImage(processImage));

}

void MainWindow::gammaCorrection(int v)
{
    gamma(orginalImage, processImage, v);
    ui->label->setPixmap(QPixmap::fromImage(processImage));

}

