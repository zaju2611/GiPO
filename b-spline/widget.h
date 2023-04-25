#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QImage>
#include <QPoint>
#include <QPainter>

class Widget : public QWidget
{
    Q_OBJECT
    QImage image;

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:

   QPoint previousPoint;
    bool previousPointSet = false;
    void drawCurve(QVector<QPoint> points);
    QVector<QPoint> points;
    QPainter painter;


protected:
    void mousePressEvent(QMouseEvent *e);
    void paintEvent(QPaintEvent *event);
};


#endif // WIDGET_H
