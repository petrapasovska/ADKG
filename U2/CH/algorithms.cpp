#include "algorithms.h"
#include "sortbyxasc.h"
#include "sortbyyasc.h"

Algorithms::Algorithms()
{

}

TPosition Algorithms::getPointLinePosition(QPoint &q, QPoint &a, QPoint &b)
{
    //Point and line position
    double eps = 1.0e-10;

    double ux = b.x() - a.x();
    double uy = b.y() - a.y();
    double vx = q.x() - a.x();
    double vy = q.y() - a.y();

    //Determinant
    double t = (ux * vy - uy * vx);

    //Point in the left half plane
    if(t > eps)
        return LEFT;

    //Point in the right half plane
    if(t < -eps)
        return RIGHT;

    //Point on the line
    return ON;
}


double Algorithms::get2LinesAngle(QPoint &p1,QPoint &p2,QPoint &p3, QPoint &p4)
{
    //Angle between two lines
    double ux = p2.x() - p1.x();
    double uy = p2.y() - p1.y();
    double vx = p4.x() - p3.x();
    double vy = p4.y() - p3.y();

    //Dot product
    double dot = ux * vx + uy * vy;

    //Norms
    double nu = sqrt(ux * ux + uy * uy);
    double nv = sqrt(vx * vx + vy * vy);

    return fabs(acos(dot/(nu * nv)))*(180/M_PI);
}

QPolygon Algorithms::CHJarvis (vector<QPoint> &points)
{
    QPolygon ch;

    //Find pivot q
    std::sort(points.begin(), points.end(), sortByYAsc());
    QPoint q = points[0];

    std::sort(points.begin(), points.end(), SortByXAsc());
    QPoint s = points[0];

    //Create Pjj
    QPoint pjj (s.x(),q.y());
    QPoint pj = q;

    //Add pivot to CH
    ch.push_back(q);

    //Add all points to CH
    do
    {
        int i_max = -1;
        double fi_max = 0;

        //Find pi = arg max angle(pi, pj, pjj)
        for(int i = 0; i<points.size();i++)
        {
            //Get angle betwwen 2 segments
            double fi = get2LinesAngle(pj, pjj, pj, points[i]);

            //Find maximum
            if (fi>fi_max)
            {
                i_max=i;
                fi_max=fi;
            }
        }

        //Add the next point to CH
        ch.push_back(points[i_max]);

        //Assign CH verticess
        pjj = pj;
        pj = points[i_max];

    } while (!(pj == q));

    return ch;
}

double Algorithms::getPointLineDistance(QPoint &q, QPoint &a, QPoint &b)
{
    //Point and line distance
    double x12 = b.x()-a.x();
    double y12 = b.y()-a.y();

    double y1a = q.y()-a.y();
    double y2a = b.y()-q.y();

    double numerator = -q.x()*y12 + a.x()*y2a + b.x()*y1a;
    double denominator = sqrt(x12*x12 + y12*y12);

    return fabs(numerator)/denominator;

}

QPolygon Algorithms::QHull (vector<QPoint> &points)
{
    //Create convex hull using the qhull procedure
    QPolygon ch;
    std::vector<QPoint> su, sl;

    //Find q1, q3
    std::sort(points.begin(), points.end(), SortByXAsc());
    QPoint q1 = points[0];
    QPoint q3 = points[points.size()-1];

    // Add to SU,SL
    su.push_back(q1);
    su.push_back(q3);
    sl.push_back(q1);
    sl.push_back(q3);

    //Splite to SU or SL
    for(int i =0;i<points.size();i++)
    {
        // Add to SU
        if(getPointLinePosition(points[i],q1,q3)==LEFT)
        {
            su.push_back(points[i]);
        }

        // Add to SL
        if(getPointLinePosition(points[i],q1,q3)==RIGHT)
        {
            sl.push_back(points[i]);
        }
    }

    //Add q3 to ch
    ch.push_back(q3);

    //Process SU
    qh(1, 0, su, ch);

    //Add q1 to ch
    ch.push_back(q1);

    //Process SL
    qh(0, 1, sl, ch);

    return ch;
}

void Algorithms::qh(int s, int e, vector<QPoint> &p, QPolygon &h)
{
    //Recursive procedure of qhull
    int i_max = -1;
    double d_max = 0;

    //Browse all points
    for(int i = 2; i<p.size(); i++)
    {
        //Is the point in the right half-plane?
        if(getPointLinePosition(p[i], p[s], p[e])==RIGHT)
        {
           double d =  getPointLineDistance(p[i], p[s], p[e]);
           //Remember the fardest point
           if (d>d_max)
           {
                d_max = d;
                i_max = i;
           }
        }
    }

    //Point in the half plain exist
    if(i_max >1)
    {
        //Process first interval
        qh(s,i_max,p,h);

        //Add to CH
        h.push_back(p[i_max]);

        //Process second interval
        qh(i_max,e,p,h);
    }
}

void Algorithms::minimumAreaEnclosingBox (QPolygon &ch, QPolygon &rectangle, QLine &direction)
{
       QPolygon ch_sort = ch;              // main direction of building
       QPoint p_global, k_global;       // save points for transformation
       QPoint p1(0,0);                  // center of local system
       QPoint k1;
       k1.setY(0);
       int n = ch.size();

       // compute min_area for loop
       std::sort(ch_sort.begin(),ch_sort.end(), SortByXAsc());
       double x_min = ch_sort[0].x();
       double x_max = ch_sort[n-1].x();
       std::sort(ch_sort.begin(),ch_sort.end(), sortByYAsc());
       double y_min = ch_sort[0].y();
       double y_max = ch_sort[n-1].y();
       double min_area = (x_max-x_min)*(y_max-y_min);

       //compute coordinates od rectangle
       for(int i = 0; i<n-1 ;i++)
       {
            k1.setX(length2Points(ch[i], ch[i+1])); // define axis +X of local system

            // save transformation key to polygon
            p_global = ch[i];
            k_global = ch[i+1];

            // transform convex hull to local system
            polygonTransform(ch[i], ch[i+1], p1, k1, ch );

            // find min max coordinates
            ch_sort = ch;
            std::sort(ch_sort.begin(),ch_sort.end(), SortByXAsc());
            double x_min = ch_sort[0].x();
            double x_max = ch_sort[n-1].x();
            std::sort(ch_sort.begin(),ch_sort.end(), sortByYAsc());
            double y_min = ch_sort[0].y();
            double y_max = ch_sort[n-1].y();

            //compute min area
            double area = (x_max-x_min)*(y_max-y_min);
            qDebug() << area;
/*
            if(area<min_area)
            {
                rectangle.clear();
                rectangle << QPoint(x_min, y_min) << QPoint(x_max, y_min) << QPoint(x_max, y_max) << QPoint(x_min, y_max);
                min_area = area;
                qDebug() << min_area;
            }

*/
       }

       /*
       if(rectangle[2].x()-rectangle[0].x()>rectangle[2].y()-rectangle[0].y())
       {
           direction.setP1(QPoint(rectangle[0].x(), 0.5*(rectangle[2].y()+rectangle[0].y()))); //QPoint(x_min, 0.5*(y_max+y_min))
           direction.setP2(QPoint(rectangle[2].x(), 0.5*(rectangle[2].y()+rectangle[0].y())));//QPoint(x_max, 0.5*(y_max+y_min))
       }
       else
       {
           direction.setP1(QPoint(0.5*(rectangle[2].x()+rectangle[0].x()), rectangle[0].y())); //QPoint(0.5*(x_max+x_min), y_min)
           direction.setP2(QPoint(0.5*(rectangle[2].x()+rectangle[0].x()), rectangle[2].y()));//QPoint(0.5*(x_max+x_min), y_max)
       }
*/
       // transform rectangle and direction back to global system
       //polygonTransform(p1,k1,p_global,k_global,rectangle);
       //polygonTransform(p1,k1,p_global,k_global,direction);
}

void Algorithms::polygonTransform(QPoint p, QPoint k, QPoint p1, QPoint k1, QPolygon &pol)
{
    // Xa = Xp + b*delxpa - a*delypa
    // Ya = Yp + a*delxpa + b*delypa
    // a =(delxpk*delYpk - delypk*delXpk)/(delxpk*delxpk +delypk*delypk)
    // b =(delxpk*delXpk - delypk*delYpk)/(delxpk*delxpk +delypk*delypk)

    QPolygon pol_transform;
    double angle = get2LinesAngle(p, k, p1, k1)*(M_PI/180);

    double a = ((p.x()-k.x())*(p1.y()-k1.y()) - (p.y()-k.y())*(p1.x()-k1.x())) / ((p.x()-k.x())*(p.x()-k.x()) - (p.y()-k.y())*(p.y()-k.y()));
    double b = ((p.x()-k.x())*(p1.y()-k1.y()) - (p.y()-k.y())*(p1.x()-k1.x())) / ((p.x()-k.x())*(p.x()-k.x()) - (p.y()-k.y())*(p.y()-k.y()));

    for(int i = 0; i<pol.size();i++)
    {
        QPoint point = pol[i];
        pol[i].setX(cos(angle)*(point.x()-p.x()) - sin(angle)*(point.y()-p.y()));
        pol[i].setY(sin(angle)*(point.x()-p.x()) + cos(angle)*(point.y()-p.y()));
        //pol[i].setX(p1.x() + b*(point.x()-p.x()) - a*(point.y()-p.y()));
        //pol[i].setY(p1.y() + a*(point.x()-p.x()) + b*(point.y()-p.y()));
    }

}

void Algorithms::polygonTransform(QPoint p, QPoint k, QPoint p1, QPoint k1, std::vector<QPoint> &pol)
{
    // Xa = Xp + b*delxpa - a*delypa
    // Ya = Yp + a*delxpa + b*delypa
    // a =(delxpk*delYpk - delypk*delXpk)/(delxpk*delxpk +delypk*delypk)
    // b =(delxpk*delXpk - delypk*delZpk)/(delxpk*delxpk +delypk*delypk)

    QPolygon pol_transform;
    double angle = get2LinesAngle(p, k, p1, k1);
    double a = ((p.x()-k.x())*(p1.y()-k1.y()) - (p.y()-k.y())*(p1.x()-k1.x())) / ((p.x()-k.x())*(p.x()-k.x()) - (p.y()-k.y())*(p.y()-k.y()));
    double b = ((p.x()-k.x())*(p1.y()-k1.y()) - (p.y()-k.y())*(p1.x()-k1.x())) / ((p.x()-k.x())*(p.x()-k.x()) - (p.y()-k.y())*(p.y()-k.y()));

    for(int i = 0; i<pol.size();i++)
    {
        QPoint point = pol[i];
        pol[i].setX(cos(angle)*(point.x()-p.x()) - sin(angle)*(point.y()-p.y()));
        pol[i].setY(sin(angle)*(point.x()-p.x()) + cos(angle)*(point.y()-p.y()));

        //pol[i].setX(p1.x() + b*(point.x()-p.x()) - a*(point.y()-p.y()));
        //pol[i].setY(p1.y() + a*(point.x()-p.x()) + b*(point.y()-p.y()));
    }
}

void Algorithms::polygonTransform(QPoint p, QPoint k, QPoint p1, QPoint k1, QLine &pol)
{
    // Xa = Xp + b*delxpa - a*delypa
    // Ya = Yp + a*delxpa + b*delypa
    // a =(delxpk*delYpk - delypk*delXpk)/(delxpk*delxpk +delypk*delypk)
    // b =(delxpk*delXpk - delypk*delZpk)/(delxpk*delxpk +delypk*delypk)

    QPolygon pol_transform;
    double a = ((p.x()-k.x())*(p1.y()-k1.y()) - (p.y()-k.y())*(p1.x()-k1.x())) / ((p.x()-k.x())*(p.x()-k.x()) - (p.y()-k.y())*(p.y()-k.y()));
    double b = ((p.x()-k.x())*(p1.y()-k1.y()) - (p.y()-k.y())*(p1.x()-k1.x())) / ((p.x()-k.x())*(p.x()-k.x()) - (p.y()-k.y())*(p.y()-k.y()));

    QPoint first = pol.p1();
    QPoint second = pol.p2();

    pol.setP1(QPoint(p1.x() + b*(first.x()-p.x()) - a*(first.y()-p.y()),p1.y() + a*(first.x()-p.x()) + b*(first.y()-p.y())));
    pol.setP1(QPoint(p1.x() + b*(second.x()-p.x()) - a*(second.y()-p.y()),p1.y() + a*(second.x()-p.x()) + b*(second.y()-p.y())));

}
