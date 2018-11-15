#ifndef ALGORITHMS_H
#define ALGORITHMS_H
#include <QtGui>
#include <math.h>
using namespace std;

typedef enum{
    LEFT = 0,
    RIGHT,
    ON
} TPosition;

struct vec_angle{
    QPoint p;
    double a;
    double d;
};

class Algorithms
{

private:
    static double length2Points(QPoint q, QPoint p){return sqrt( (q.x()-p.x())*(q.x()-p.x())+(q.y()-p.y())*(q.y()-p.y()) );}
    static void polygonTransform(QPoint p, QPoint k, QPoint p1, QPoint k1, QPolygon &pol);
    static void polygonTransform(QPoint p, QPoint k, QPoint p1, QPoint k1, QLine &pol);
    static void rotateByAngle(QPolygon &points, double angle);
    static void rotateByAngle(QLine &points, double angle);

public:
    Algorithms();

    static TPosition getPointLinePosition(QPoint &q, QPoint &a, QPoint &b);
    static double get2LinesAngle(QPoint &p1,QPoint &p2,QPoint &p3, QPoint &p4);
    static QPolygon CHJarvis (vector<QPoint> &points);
    static void minimumAreaEnclosingBox (QPolygon &ch, QPolygon &rectangle, QLine &direction);
    static double getPointLineDistance(QPoint &q, QPoint &a, QPoint &b);
    static QPolygon QHull (vector<QPoint> &points);
    static void qh (unsigned int s, unsigned int e, vector<QPoint> &p, QPolygon &h);
    static QPolygon GrahamScan (vector<QPoint> &points);
    static QPolygon GrahamScanNew (vector<QPoint> &points);
    static QPolygon CHSweep (vector<QPoint> &points);
    static QPolygon exatlyCH(QPolygon ch);
    static QPolygon deleteDuplicityCH(QPolygon ch);
};

#endif // ALGORITHMS_H
