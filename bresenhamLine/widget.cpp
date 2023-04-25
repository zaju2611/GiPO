#include "widget.h"

#include <QPainter>
#include <QMouseEvent>
#include <QPoint>
#include <iostream>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
}

Widget::~Widget()
{
}


void drawLineBresenham(QPainter &painter, const QPoint &p1, const QPoint &p2)
    {
    int x0 = p1.x();
       int y0 = p1.y();
       int x1 = p2.x();
       int y1 = p2.y();

       int dx =  abs(x1-x0), sx = x0<x1 ? 1 : -1;
       int dy = -abs(y1-y0), sy = y0<y1 ? 1 : -1;
       int err = dx+dy, e2;

       for (;;) {
           painter.drawPoint(x0, y0);
           if (2*err >= dy) {
               if (x0 == x1) break;
               err += dy;
               x0 += sx;
           }
           if (2*err <= dx) {
               if (y0 == y1) break;
               err += dx;
               y0 += sy;
           }
        }
    }


void Widget::mousePressEvent(QMouseEvent *event){
    if (event->button() == Qt::LeftButton)
            {
                if (firstPointSet)
                {
                    secondPointSet = true;
                    point2 = QPoint(event->x(), event->y());
                    update();
                    firstPointSet = false;
                    secondPointSet=false;
                }
                else
                {
                    firstPointSet = true;
                    point1 = QPoint(event->x(), event->y());
                }
            }
}



void Widget::paintEvent(QPaintEvent *event){
    Q_UNUSED(event);
    QPainter paint(this);
    drawLineBresenham(paint, point1, point2);
}
