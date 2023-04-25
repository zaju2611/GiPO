#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <iostream>
#include <cmath>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->actionopen, SIGNAL(triggered(bool)),this,SLOT(openImage(bool)));
    connect(ui->actionfilter, SIGNAL(triggered(bool)),this,SLOT(filter( )));
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
    }
}

void MainWindow::filter(){
    int r = 10;
    int w = orginalImage.width();
    int h = orginalImage.height();

    processImage = QImage(w+2*r, h +2*r, orginalImage.format());
    processImage.fill(0);
    for (int i = 0; i < orginalImage.height(); i++)
    {
    memcpy(processImage.bits() + (i+r) * processImage.bytesPerLine() + r*4, orginalImage.bits() + i * orginalImage.bytesPerLine(), orginalImage.bytesPerLine());
    }

    int imageWidth = processImage.width();
    int imageHeight = processImage.height();
    finalImage = QImage(imageWidth,imageHeight,processImage.format());

    int maskLine = 3;
    int maskSize = pow(maskLine,2);
    float sigmaI = 100.0f;
    float sigmaD = 100.0f;
    int start = floor(maskSize/2);


    for (int x = 0; x < imageWidth; x++) {
            for (int y = 0; y < imageHeight; y++) {
                float r = 0.0f, g = 0.0f, b = 0.0f, weightSum = 0.0f;

                for (int i = -start; i <= start; i++) {
                    for (int j = -start; j <= start; j++) {
                        if (x+i >= 0 && x+i < imageWidth && y+j >= 0 && y+j < imageHeight)          {
                            float dist = sqrt(pow(i,2) + pow(j,2));
                            float hd = exp(-pow(dist,2) / (2*pow(sigmaD,2)));
                            QRgb pixel = processImage.pixel(x+i, y+j);
                            QRgb dst_pixel = processImage.pixel(x,y);
                            int rP = qRed(pixel);
                            int gP = qGreen(pixel);
                            int bP = qBlue(pixel);
                            int rdP = qRed(dst_pixel);
                            int gdP = qGreen(dst_pixel);
                            int bdP = qBlue(dst_pixel);
                            float hI = exp(-((pow((rP-rdP),2)+pow((gP-gdP),2)+pow((bP-bdP),2))/(2*(sigmaI*sigmaI))));
                            float weight = hd*hI;

                            r += weight * qRed(pixel);
                            g += weight * qGreen(pixel);
                            b += weight * qBlue(pixel);
                            weightSum += weight;
                        }
                    }
                }

                int new_r = r/weightSum;
                int new_g = g/weightSum;
                int new_b = b/weightSum;
                new_r = clamp<int>(new_r,0,255);
                new_g = clamp<int>(new_g,0,255);
                new_b = clamp<int>(new_b,0,255);

                finalImage.setPixel(x, y, qRgb(new_r, new_g, new_b));
            }
        }

    QImage copiedfinal = finalImage.copy(r,r,w,h);
    ui->label->setPixmap(QPixmap::fromImage(copiedfinal));
}
