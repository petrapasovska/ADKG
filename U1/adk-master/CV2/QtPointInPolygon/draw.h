#ifndef DRAW_H
#define DRAW_H

#include <QWidget>
#include <vector>
#include <fstream>
#include <string.h>
#include <QRandomGenerator>
#include <QDebug>


class Draw : public QWidget
{
    Q_OBJECT

private:
    bool draw_point;                //Switch, whether to draw a point or a polygon
    QPointF q;                       //The analyzed point q
    std::vector<QPointF> pol;        //The polygon
    std::vector<QPolygonF> polygons;
    int result;

public:
    void paintEvent(QPaintEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void clearCanvas();
    void setDrawPoint();
    QPointF getPoint() {return q;}
    std::vector<QPointF> getPolygon(){return pol;}
    explicit Draw(QWidget *parent = nullptr);
    bool importPolygons(std::string &path);
    void generatePolygon(int n_points, int coordinates_max);
    void fillPolygon(int res);
    int selectMinIndex(std::vector<double> v);


signals:

public slots:
};

#endif // DRAW_H
