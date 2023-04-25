#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <iostream>
#include <QVector>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->actionopen, SIGNAL(triggered(bool)),this,SLOT(openImage(bool)));
    connect(ui->actionfilter, SIGNAL(triggered(bool)),this,SLOT(medianFilter( )));
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


void MainWindow::medianFilter(){
    int r = 10;
    int w = orginalImage.width();
    int h = orginalImage.height();

    processImage = QImage(w+2*r, h +2*r, orginalImage.format());
    processImage.fill(0);
    for (int i = 0; i < orginalImage.height(); i++)
    {
    memcpy(processImage.bits() + (i+r) * processImage.bytesPerLine() + r*4, orginalImage.bits() + i * orginalImage.bytesPerLine(), orginalImage.bytesPerLine());
    }

    finalImage = QImage(processImage.width(), processImage.height(), processImage.format());
    int rows = 10;
    int columns = 10;
    int size = rows*columns;
    int start = floor(rows/2);

    for (int x = 0; x < processImage.width(); ++x) {
            for (int y = 0; y < processImage.height(); ++y) {
                QVector<int> valuesR;
                QVector<int> valuesG;
                QVector<int> valuesB;
                for (int i = -start; i <= start; ++i) {
                    for (int j = -start; j <= start; ++j) {
                        int px = clamp<int>( x + i,r, processImage.width() - r);
                        int py = clamp<int>(y + j,r, processImage.height() - r);
                        valuesR.append(qRed(processImage.pixel(px, py)));
                        valuesG.append(qGreen(processImage.pixel(px, py)));
                        valuesB.append(qBlue(processImage.pixel(px, py)));
                        }
                    }
                std::sort(valuesR.begin(),valuesR.end());
                std::sort(valuesG.begin(),valuesG.end());
                std::sort(valuesB.begin(),valuesB.end());

                int medianR = valuesR[size/2];
                int medianG = valuesG[size/2];
                int medianB = valuesB[size/2];
                finalImage.setPixel(x,y,qRgb(medianR,medianG, medianB));
            }
    }
    QImage copiedfinal = finalImage.copy(r,r,w-1,h-1);
    ui->label->setPixmap(QPixmap::fromImage(copiedfinal));

}
