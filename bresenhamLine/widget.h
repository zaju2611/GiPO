#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

class Widget : public QWidget
{
    Q_OBJECT
    QImage image;

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    QPoint point1;
    QPoint point2;
    bool firstPointSet = false;
    bool secondPointSet = false;

protected:
    void mousePressEvent(QMouseEvent *e);
    void paintEvent(QPaintEvent *event);
};
#endif // WIDGET_H
