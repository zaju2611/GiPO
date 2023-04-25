#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QScrollBar>
#include <QMainWindow>
#include <QImage>
#include <qmath.h>
#include <QChart>
#include <QLabel>
#include <QMainWindow>
#include <QChartView>
#include <QBarSeries>
#include <QBarSet>
#include <QLegend>
#include <QBarCategoryAxis>
#include <QValueAxis>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

template<class T>
T clamp(T v, T min_val, T max_val){
    if(v>max_val) return max_val;
    if(v<min_val) return min_val;
    return v;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    QImage orginalImage;
    QImage processImage;


public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
/*
protected:
    void mousePressEvent(QMouseEvent*);
   // void mouseMoveEvent(QMouseEvent*);
   // void mouseDoubleClickEvent(QMouseEvent*);
   // void mouseReleaseEvent(QMouseEvent*);

    void keyPressEvent(QKeyEvent*);
   // void keyReleaseEvent(QKeyEvent*);

   // void wheelEvent(QWheelEvent*);
*/
private:
    Ui::MainWindow *ui;
    QScrollBar *scrollBar;

public slots:
    void openImage(bool);
    void changeBrightness(int);
    void gammaCorrection(int);
    void drawHistogram(QImage &);




};
#endif // MAINWINDOW_H
