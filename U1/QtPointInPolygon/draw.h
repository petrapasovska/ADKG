#ifndef DRAW_H
#define DRAW_H

#include <QWidget>
#include <vector>
#include <fstream>
#include <string.h>
//#include <QRandomGenerator>
#include <QDebug>


class Draw : public QWidget
{
    Q_OBJECT

private:
    bool draw_point;                //Draw a point
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
    QPolygonF getPolygon(unsigned int index){return polygons.at(index);}
    explicit Draw(QWidget *parent = nullptr);
    bool importPolygons(std::string &path);
    void generatePolygon(int n_points);
    void setResult(std::vector<int> res){result = res;}
    int selectMinIndex(std::vector<double> v);
    std::vector<QPolygonF> getPolygons(){return polygons;}


signals:

public slots:
};

#endif // DRAW_H
