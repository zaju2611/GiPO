#include "widget.h"
#include <QPainter>
#include <QMouseEvent>
#include <QPoint>
#include <math.h>
#include <iostream>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
}

Widget::~Widget()
{
}

void Widget::mousePressEvent(QMouseEvent *event){
    int x = event->x();
    int y = event-> y();
    points.push_back(QPoint(x,y));
    update();
}

void Widget::paintEvent(QPaintEvent* event){
    painter.begin(this);
    if(points.size()>3){
        drawCurve(points);
    }
    painter.end();
}


void Widget::drawCurve(QVector<QPoint> points)
{
    for (int i = 0; i < points.size() - 3; i=i+3) {
        double t = 0;
        while (t <= 1)
        {
            int x = pow((1 - t),3) * points[i].x() +
                    3 * t * pow((1 - t),2) * points[i+1].x() +
                    3 * t * t * (1 - t) * points[i+2].x() +
                    pow(t,3) * points[i+3].x();
            int y = pow((1 - t),3) * points[i].y() +
                    3 * t * pow((1 - t),2) * points[i+1].y() +
                    3 * t * t * (1 - t) * points[i+2].y() +
                    pow(t,3) * points[i+3].y();

            int x1 = std::round(x);
            int y1 = std::round(y);

            if (previousPointSet)
            {
                int x2 = previousPoint.x();
                int y2 = previousPoint.y();

                int dx = abs(x2 - x1);
                int dy = abs(y2 - y1);

                int sx = (x1 < x2) ? 1 : -1;
                int sy = (y1 < y2) ? 1 : -1;

                int err = dx - dy;

                int x = x1;
                int y = y1;
                while (true)
                {
                    painter.drawPoint(x, y);
                    if (x == x2 && y == y2)
                    {
                        break;
                    }

                    int e2 = 2 * err;
                    if (e2 > -dy)
                    {
                        err = err - dy;
                        x = x + sx;
                    }

                    if (e2 < dx)
                    {
                        err = err + dx;
                        y = y + sy;
                    }
                }
            }

            previousPoint = QPoint(x1, y1);
            previousPointSet = true;

            t += 0.001;
        }
        previousPointSet = false;
    }
}

