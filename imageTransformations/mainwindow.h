#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QMenu>
#include <QSlider>

class MainWindow : public QMainWindow
{
    Q_OBJECT
    QLabel *imageLabel, *scaleLabelX,*scaleLabelY, *rotateLabel, *translationXLabel, *translationYLabel,*shearXLabel, *shearYLabel;
    QSlider *sliderScaleX,*sliderScaleY, *sliderRotate, *sliderTranslationX,*sliderTranslationY, *sliderShearX,*sliderShearY;
    QImage mainImage, procImage;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void openImage();
    void slotScale();
    void slotRotate(int);
    void slotTranslation();
    void slotShear();
};
#endif // MAINWINDOW_H
