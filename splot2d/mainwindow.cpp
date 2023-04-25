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

    const int n = 3;
    const int m= 3;
    int start = floor(n/2);

    int maskSum = 0;
    int mask[m][n]{
        {0,-1,0},
        {-1,4,-1},
        {0,-1,0}

    };
    for(int i=0;i<n;i++){
        for(int j=0;j<m;j++){
            maskSum+=mask[i][j];
        }
    }

    int imageWidth = processImage.width();
    int imageHeight = processImage.height();
    finalImage = QImage(imageWidth,imageHeight,processImage.format());

    for (int x = 0; x < imageWidth; x++) {
            for (int y = 0; y < imageHeight; y++) {
                int rResult=0;
                int gResult=0;
                int bResult=0;
                for (int dx=-start;dx<start+1;dx++){
                    for(int dy=-start; dy<start+1;dy++){
                        int x2 = x + dx;
                        int y2 = y + dy;
                        if (x2 < 0) x2 = r;
                        if (x2 >= imageWidth) x2 = imageWidth - r;
                        if (y2 < 0) y2 = r;
                        if (y2 >= imageHeight) y2 = imageHeight - r;

                        QRgb pixel = processImage.pixel(x2,y2);

                        int r = qRed(pixel);
                        int g = qGreen(pixel);
                        int b = qBlue(pixel);

                        rResult+= r * mask[dx+start][dy+start];
                        gResult+= g * mask[dx+start][dy+start];
                        bResult+= b * mask[dx+start][dy+start];

                    }
                }
                if(maskSum>0){
                rResult = clamp<int>(rResult/maskSum,0,255);
                gResult = clamp<int>(gResult/maskSum,0,255);
                bResult = clamp<int>(bResult/maskSum,0,255);
                finalImage.setPixel(x,y,qRgb(rResult,gResult,bResult));
                }
                else if (maskSum<0){
                    maskSum = abs(maskSum);
                    rResult = clamp<int>(rResult/maskSum,0,255);
                    gResult = clamp<int>(gResult/maskSum,0,255);
                    bResult = clamp<int>(bResult/maskSum,0,255);
                    finalImage.setPixel(x,y,qRgb(rResult,gResult,bResult));
                }
                else{
                    rResult = clamp<int>(rResult,0,255);
                    gResult = clamp<int>(gResult,0,255);
                    bResult = clamp<int>(bResult,0,255);
                    finalImage.setPixel(x,y,qRgb(rResult,gResult,bResult));
                }
            }
    }

    QImage copiedfinal = finalImage.copy(r+1,r+1,w-2,h-2);
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






