#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <QDebug>

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
    QImage secondImage;
    QImage finalImage;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

public slots:
    void openImage(bool);
    void filter();

};

#endif // MAINWINDOW_H
