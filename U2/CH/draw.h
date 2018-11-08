#ifndef DRAW_H
#define DRAW_H

#include <QWidget>
#include <QtGui>
#include <math.h>
#include <QRandomGenerator>
#include <QDebug>

class Draw : public QWidget
{
    Q_OBJECT
private:
    std::vector<QPoint> points;
    QPolygon ch;
    QPolygon rectangle;
    QLine direction;

public:
    explicit Draw(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void clearCanvas();
    void setCH(QPolygon ch_) {ch = ch_;}
    void setRectangle(QPolygon rectangle_) {rectangle = rectangle_;}
    void setDirection(QLine direction_){direction = direction_;}
    void setPoints(std::vector<QPoint> points_){points = points_;}
    std::vector<QPoint> getPoints(){return points;}
    QPolygon getConvexHull(){return ch;}
    static std::vector<QPoint> generateCircle(int n);
    static std::vector<QPoint> generateEclipse(int n);
    static std::vector<QPoint> generateSquare(int n);
    static std::vector<QPoint> generateStarShape(int n);
    static std::vector<QPoint> generateRandomPoints(int n);
    static std::vector<QPoint> generateGrid(int n);



signals:

public slots:
};

#endif // DRAW_H
