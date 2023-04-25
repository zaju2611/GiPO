#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <iostream>


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
    secondImage = QImage(imageWidth,imageHeight,processImage.format());
    finalImage = QImage(imageWidth,imageHeight,processImage.format());

    float sigma = 5.0f;
    float pi = 3.14159265358979323846f;
    int maskSize = 10;
    int start = floor(maskSize/2);
    float sum = 0;

    std::vector<float> mask_x;
    std::vector<float> mask_y;

        for (int k = -start; k <= start; k++) {
            float gaussian = (1 / (2 * pi * sigma * sigma)) * exp(-(k*k) / (2 * sigma * sigma));

            mask_x.push_back(gaussian);
            mask_y.push_back(gaussian);
        }

        for(int i=0 ;i <maskSize; i++){
            sum += mask_x[i];
        }

    //przejscie po wierszach
    for (int y = 0; y < imageHeight; y++) {
        for (int x = 0; x < imageWidth; x++) {
            float sum_r = 0;
            float sum_g = 0;
            float sum_b = 0;
            for (int k = -start; k <= start; k++) {
                    int x2 = x+k;
                    int y2= y+k;
                    if (x2 < 0) x2 = r;
                    if (x2 >= imageWidth) x2 = imageWidth - r;
                    if (y2 < 0) y2 = r;
                    if (y2 >= imageHeight) y2 = imageHeight - r;
                    QRgb pixel = processImage.pixel(x2, y2);
                    int r = qRed(pixel);
                    int g = qGreen(pixel);
                    int b = qBlue(pixel);
                    float rN = r/255.0f;
                    float gN = g/255.0f;
                    float bN = b/255.0f;
                    sum_r += mask_x[k + start] * rN;
                    sum_g += mask_x[k + start] * gN;
                    sum_b += mask_x[k + start] * bN;

            }
            int new_r = (sum_r/sum)*255.0f;
            int new_g = (sum_g/sum)*255.0f;
            int new_b = (sum_b/sum)*255.0f;
            new_r = clamp<int>(new_r,0,255);
            new_g = clamp<int>(new_g,0,255);
            new_b = clamp<int>(new_b,0,255);
            secondImage.setPixel(x,y,qRgb(new_r,new_g, new_b));
        }
    }

    //przejscie po kolumnach
   for (int x = 0; x < imageWidth; x++) {
         for (int y = 0; y < imageHeight; y++){
            float sum_r = 0;
            float sum_g = 0;
            float sum_b = 0;
            for (int k = -start; k <= start; k++) {
                int x2 = x+k;
                int y2= y+k;
                if (x2 < 0) x2 = r;
                if (x2 >= imageWidth) x2 = imageWidth - r;
                if (y2 < 0) y2 = r;
                if (y2 >= imageHeight) y2 = imageHeight - r;
                QRgb pixel = secondImage.pixel(x2, y2);
                    int r = qRed(pixel);
                    int g = qGreen(pixel);
                    int b = qBlue(pixel);
                    float rN = r/255.0f;
                    float gN = g/255.0f;
                    float bN = b/255.0f;
                    sum_r += mask_y[k + start] * rN;
                    sum_g += mask_y[k + start] * gN;
                    sum_b += mask_y[k + start] * bN;

            }
            int new_r = (sum_r/sum)*255.0f;
            int new_g = (sum_g/sum)*255.0f;
            int new_b = (sum_b/sum)*255.0f;
            new_r = clamp<int>(new_r,0,255);
            new_g = clamp<int>(new_g,0,255);
            new_b = clamp<int>(new_b,0,255);
            finalImage.setPixel(x,y,qRgb(new_r,new_g, new_b));
        }
    }

    QImage copiedfinal = finalImage.copy(r,r,w,h);
    ui->label->setPixmap(QPixmap::fromImage(copiedfinal));
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
