#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include <vector>
#include <QtGui>
#include <QDebug>


class Algorithms
{
public:
    Algorithms();
    static int getPositionRay(QPointF q, QPolygonF pol);
    static int getPositionWinding(QPointF q, QPolygonF pol);
    static int getPointLinePosition(QPointF &q, QPointF &a, QPointF &b);
    static double get2LinesAngle(QPointF &p1,QPointF &p2,QPointF &p3, QPointF &p4);
};

#endif // ALGORITHMS_H
