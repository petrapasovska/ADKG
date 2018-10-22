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
    QPointF q;                      //The analyzed point q
    QPolygonF pol;                  //The polygon
    std::vector<QPolygonF> polygons;
    std::vector<int> result;
    QPolygonF polygon_generate;

public:
    void paintEvent(QPaintEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void clearCanvas();
    void setDrawPoint();
    QPointF getPoint() {return q;}
    QPolygonF getPolygon(int index){return polygons[index];}
    explicit Draw(QWidget *parent = nullptr);
    bool importPolygons(std::string &path);
    void generatePolygon(int n_points, int coordinates_max);
    void setResult(std::vector<int> res){result = res;}
    int selectMinIndex(std::vector<double> v);
    int getNumberOfPolygons(){return polygons.size();}


signals:

public slots:
};

#endif // DRAW_H
