#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->actionopen, SIGNAL(triggered(bool)), this, SLOT(openImage(bool)));
    connect(ui->horizontalSlider, &QSlider::valueChanged,this, &MainWindow::modify);
    connect(ui->horizontalSlider_2, &QSlider::valueChanged,this, &MainWindow::modify);
    connect(ui->horizontalSlider_3, &QSlider::valueChanged,this, &MainWindow::modify);
}

MainWindow::~MainWindow()
{
    delete ui;
}

float *tabXYZ = new float[3];
float *tabLab = new float[3];
float *tabXYZLab = new float[3];
int *tabRGB = new int[3];

float * rgbDoxyz (int r, int g, int b) {
    float *tab = new float[3];
    float x,y,z;
    float rN = r/255.0f;
    float gN= g/255.0f;
    float bN = b/255.0f;

    x= rN*0.41242+gN*0.35759+bN* 0.18046;
    y = rN*0.21266+gN*0.71517+bN* 0.07218;
    z = rN*0.01933+ gN*0.11919+bN*0.95044;

    tab[0]=x;
    tab[1]=y;
    tab[2]=z;
    return tab;
}

int * xyzDorgb (float x, float y, float z) {
    int *tab = new int[3];
    float r, g, b;
    r= x*3.24071+y*(-1.53726)+z*( -0.49857);
    g = x*(-0.96925)+y*1.87599+z*0.04155;
    b= x*0.05563+y*(-0.20399)+z*1.05707;

    r = r*255.0f;
    g = g*255.0f;
    b = b*255.0f;

    r = (int) round(r);
    g = (int) round(g);
    b = (int) round(b);

    tab[0] = r;
    tab[1] = g;
    tab[2] = b;

    return tab;
}

float * xyz2lab(float x, float y, float z){
    float * tab = new float [3];
    float L, a, b;
    float e = 0.008856f;
    float k = 903.3f;
    float Xr = 0.9505f;
    float Yr = 1.0000f;
    float Zr = 1.0891f;
    float xr=0, yr=0, zr=0;
    float fx=0, fy=0, fz=0;

    xr= x/ Xr;
    yr = y/ Yr;
    zr = z/ Zr;
    if (xr > e){
        fx = pow(xr, 0.33333333333333);
    }
    if (xr <= e){
        fx = (k*xr+16)/116.0f;
    }
    if (yr > e){
        fy = pow(yr, 0.33333333333333);
    }
    if (yr <= e){
        fy = ((k*yr+16)/116.0f);
    }
    if (zr > e){
        fz = pow(zr, 0.33333333333333);
    }
    if (zr <= e){
        fz = ((k*zr+16)/116.0f);
    }
    L = 116.0f *fy -16;
    a = 500.0f * (fx - fy);
    b = 200.0f * (fy - fz);
    tab[0] = L;
    tab[1] = a;
    tab[2] = b;
    return tab;
}

float * lab2xyz(float L , float a, float b){
    float * tab = new float [3];
    float x, y, z;
    float e = 0.008856f;
    float k = 903.3f;
    float Xr = 0.9505f;
    float Yr = 1.0000f;
    float Zr = 1.0891f;
    float xr=0, yr=0, zr=0;
    float fx=0, fy=0, fz=0;

    if (L > (k*e)){
        yr = pow (((L +16)/116.0f),3);
    }
    if (L <= (k*e)){
        yr = L/k;
    }

    if (yr > e){
        fy = (L + 16)/116.0f;
    }
    if (yr <= e){
        fy = ((k * yr)+16)/116.0f;
    }

    fx = (a/500.0f) + fy;
    fz = fy - (b/200.0f);

    if (pow(fx,3)>e){
        xr = pow (fx,3);
    }
    if (pow(fx,3)<=e){
        xr = (116.0f* fx -16)/k;
    }

    if (pow(fz,3)>e){
        zr = pow (fz,3);
    }
    if (pow(fz,3)<=e){
        zr = (116.0f* fz -16)/k;
    }
    x = xr * Xr;
    y = yr * Yr;
    z = zr * Zr;
    tab[0] = x;
    tab[1] = y;
    tab[2] = z;
    return tab;
}

void lab_modify(QImage src, QImage &dst, int dl, int da, int db){

    float L, a, ba;
    uchar R,G,B;
    for (int y=0; y<dst.height();y++){
        QRgb* pixel_src = (QRgb*)src.scanLine(y);
        QRgb* pixel_dst = (QRgb*)dst.scanLine(y);
        for(int x=0; x<dst.width();x++){

            uchar r = qRed(pixel_src[x]);
            uchar g = qGreen(pixel_src[x]);
            uchar b = qBlue(pixel_src[x]);

            tabXYZ = rgbDoxyz(r,g,b);

            tabLab = xyz2lab(tabXYZ[0],tabXYZ[1],tabXYZ[2]);
            L = clamp <float> (tabLab[0]+dl, 0, 100);
            a = clamp <float> (tabLab[1]+da, -128, 127);
            ba = clamp <float> (tabLab[2]+db, -128, 127);

            tabXYZLab = lab2xyz(L,a,ba);
            tabRGB = xyzDorgb(tabXYZLab[0],tabXYZLab[1],tabXYZLab[2]);
            R = clamp<int>(tabRGB[0],0,255);
            G = clamp<int>(tabRGB[1],0,255);
            B = clamp<int>(tabRGB[2],0,255);

            pixel_dst[x]=qRgb(R,G,B);
        }
    }
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
void MainWindow::modify(){
    int L = ui->horizontalSlider->value();
    int a = ui->horizontalSlider_2->value();
    int b = ui->horizontalSlider_3->value();
    lab_modify(orginalImage,processImage,L ,a, b);
    ui -> label -> setPixmap(QPixmap::fromImage(processImage));
}
