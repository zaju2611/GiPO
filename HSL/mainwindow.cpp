#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->actionopen, SIGNAL(triggered(bool)), this, SLOT(openImage(bool)));
    connect(ui->horizontalSlider, &QSlider::valueChanged,this, &MainWindow::changePicture);
    connect(ui->horizontalSlider_2, &QSlider::valueChanged,this, &MainWindow::changePicture);
    connect(ui->horizontalSlider_3, &QSlider::valueChanged,this, &MainWindow::changePicture);
}

int *tab1 = new int[3];
float *tab= new float[3];
float *tabHSL = new float[3];
int *tabRGB = new int[3];

float Min( float fR, float fG, float fB )
{
    float fMin = fR;
    if (fG < fMin)
    {
        fMin = fG;
    }
    if (fB < fMin)
    {
        fMin = fB;
    }
    return fMin;
}


float Max( float fR, float fG, float fB)
{
    float fMax = fR;
    if (fG > fMax)
    {
        fMax = fG;
    }
    if (fB > fMax)
    {
        fMax = fB;
    }
    return fMax;
}


float* RGBtoHSL(int R, int G, int B ){
    float H=0.0, S=0.0, L=0.0;

    float nR = R/255.0f;
    float nG = G/255.0f;
    float nB = B/255.0f;

    float min_value = Min(nR, nG, nB);
    float max_value = Max(nR, nG, nB);

    float dM = max_value - min_value;

    L = (max_value + min_value)/2.0f;

    if (min_value == max_value){
        S = 0;
        H = 0;
    }
    if (L==0){
        S =0;
    }
    if (0 < L && L <= 0.5){
        S = dM/(2*L);
    }
    if (L > 0.5){
        S = dM /(2 -(2*L));
    }

    if (nR == max_value && G >= B){
        H = 60 *((nG-nB)/dM);
    }
    if (nR == max_value && G < B){
        H = 60 * ((nG-nB)/dM)+360;
    }
    if (nG == max_value){
        H = 60 * ((nB-nR)/dM)+120;
    }
    if (nB == max_value){
        H = 60 * ((nR-nG)/dM)+240;
    }
    tab[0] = H;
    tab[1]= S;
    tab[2] = L;

    return tab;
}

int * HSLtoRGB (float H, float S, float L){

    float R=0, G=0, B=0;
    float Q=0, P=0, Tr=0, Tg=0, Tb=0;
    if (S == 0){
        R = B = G = L;
    }
    if (L < 0.5)
    {
        Q = L * (1+S);
    }
    if (L >=0.5){
        Q = L + S - (L*S);
    }
    P = (2* L) - Q;
    H = H /360;
    Tr = H +0.3333333333333;
    Tg = H;
    Tb = H - 0.333333333333333333;
    if (Tr < 0){
        Tr = Tr +1;
    }
    if (Tr >1){
        Tr = Tr -1;
    }
    if (Tg < 0){
        Tg = Tg +1;
    }
    if (Tg >1){
        Tg = Tg -1;
    }
    if (Tb < 0){
        Tb = Tb +1;
    }
    if (Tb >1){
        Tb = Tb -1;
    }
    if (Tr < 0.166666666666666){
        R = P + (Q- P)*6*Tr;
    }
    if (0.1666666666666<= Tr && Tr < 0.5){
        R =Q;
    }
    if (0.5 <= Tr && Tr < 0.6666666666666){
        R = P + (Q - P) * 6 * (0.6666666666666666 - Tr);
    }
    if (0.66666666666666 <= Tr){
        R = P;
    }
    if (Tg < 0.166666666666666){
        G = P + (Q- P)*6*Tg;
    }
    if (0.1666666666666<= Tg && Tg < 0.5){
        G =Q;
    }
    if (0.5 <= Tg && Tg < 0.6666666666666){
        G = P + (Q - P) * 6 * (0.6666666666666666 - Tg);
    }
    if (0.66666666666666 <= Tg){
        G = P;
    }
    if (Tb < 0.166666666666666){
        B = P + (Q- P)*6*Tb;
    }
    if (0.1666666666666<= Tb && Tb < 0.5){
        B =Q;
    }
    if (0.5 <= Tb && Tb < 0.6666666666666){
        B = P + (Q - P) * 6 * (0.6666666666666666 - Tb);
    }
    if (0.66666666666666 <= Tb){
        B = P;
    }

    R = R *255.0f;
    G = G * 255.0f;
    B = B * 255.0f;
    R = (int) round(R);
    G = (int) round(G);
    B = (int) round(B);
    tab1[0] = R;
    tab1[1]= G;
    tab1[2] = B;

    return tab1;
}

void changeHSL(const QImage& src, QImage& dst, int h, int s, int l){
    float H, S, L;
    int R, G, B;

    for (int y=0; y<dst.height();y++){
        QRgb* pixel_src = (QRgb*)src.scanLine(y);
        QRgb* pixel_dst = (QRgb*)dst.scanLine(y);
        for(int x=0; x<dst.width();x++){

            uchar r = qRed(pixel_src[x]);
            uchar g = qGreen(pixel_src[x]);
            uchar b = qBlue(pixel_src[x]);
            R = (int) r;
            G = (int) g;
            B = (int) b;
            tabHSL = RGBtoHSL(R,G,B);

            H = tabHSL[0]+h;
            if(H>360){
                H-=360;
            }
            else if(H<0){
                H+=360;
            }
            S=clamp<float>(tabHSL[1]+s/100.0f,0,1);
            L=clamp<float>(tabHSL[2]+l/100.0f,0,1);

            tabRGB = HSLtoRGB(H, S, L);
            tabRGB[0] = clamp<int>(tabRGB[0],0 ,255);
            tabRGB[1] = clamp<int>(tabRGB[1],0 ,255);
            tabRGB[2] = clamp<int>(tabRGB[2],0 ,255);


            pixel_dst[x]=qRgb(tabRGB[0],tabRGB[1],tabRGB[2]);

        }
    }
}

void MainWindow::changePicture(){
    int h = ui->horizontalSlider->value();
    int s = ui->horizontalSlider_2->value();
    int l = ui->horizontalSlider_3->value();
    changeHSL(orginalImage, processImage, h,s,l);
    ui-> label -> setPixmap(QPixmap::fromImage(processImage));
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


MainWindow::~MainWindow()
{
    delete ui;
}


