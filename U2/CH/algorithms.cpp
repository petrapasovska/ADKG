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
    int index = 0;

    //Find pivot q
    std::sort(points.begin(), points.end(), sortByYAsc());

    //Reduce singulary case  by kolinear points

    for(int i = 1;i<points.size();i++)
    {
        if(points[0].y()== points[i].y())
        {
            index++;
        }
    }


    // pivot with min y and max x
    QPoint q = points[index];

    //Find s
    std::sort(points.begin(), points.end(), SortByXAsc());
    QPoint s = points[0];

    qDebug()<< "q is"<<q;
    qDebug()<<"s is"<<s;

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

    exatlyCH(ch);
    deleteDuplicityCH(ch);

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

    //Delete duplicit points
    for(int j = 0; j<(ch.size()-1); j++){
        if((ch[j].x() == ch[j+1].x())&&(ch[j].y() == ch[j+1].y())){
            ch.remove(j);
            j--;
        }
    }

    exatlyCH(ch);
    deleteDuplicityCH(ch);

    qDebug() << ch;

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
       QPolygon ch_used;              // main direction of building
       QPolygon ch_sort;     // save points for transformation
       QPoint  p_global_rectangle, k_global_rectangle, p_rectangle, k_rectangle;
       QPoint p1(0,0);                  // center of local system
       QPoint k1;
       double min_angle;
       double x_min,y_min,x_max,y_max,x_min_rectangle,y_min_rectangle,x_max_rectangle,y_max_rectangle;
       k1.setY(0);
       int n = ch.size();

       /* // compute min_area for loop
       std::sort(ch_sort.begin(),ch_sort.end(), SortByXAsc());
       double x_min = ch_sort[0].x();
       double x_max = ch_sort[n-1].x();
       std::sort(ch_sort.begin(),ch_sort.end(), sortByYAsc());
       double y_min = ch_sort[0].y();
       double y_max = ch_sort[n-1].y();
       double min_area = (x_max-x_min)*(y_max-y_min);
       */
       double min_area = std::numeric_limits<double>::max();

       //compute coordinates od rectangle
       for(int i = 0; i<n-1 ;i++)
       {

            // define axis +X of local system
            k1.setX(length2Points(ch[i], ch[i+1]));

            double angle = get2LinesAngle(ch[i], ch[i+1], p1, k1);

            // transform convex hull to local system
            ch_used = ch;
            rotateByAngle(ch_used,angle*(M_PI/180));
            //polygonTransform(ch[i], ch[i+1], p1, k1, ch_used);
            qDebug()<<ch_used;

            // find min max coordinates
            ch_sort = ch_used;
            std::sort(ch_sort.begin(),ch_sort.end(), SortByXAsc());
            x_min = ch_sort[0].x();
            x_max = ch_sort[n-1].x();
            std::sort(ch_sort.begin(),ch_sort.end(), sortByYAsc());
            y_min = ch_sort[0].y();
            y_max = ch_sort[n-1].y();

            //compute min area
            double area = (x_max-x_min)*(y_max-y_min);

            if(area<min_area)
            {
                x_min_rectangle = x_min;
                y_min_rectangle = y_min;
                x_max_rectangle = x_max;
                y_max_rectangle = y_max;
                min_area = area;
                min_angle = angle;

                //save transform key
                p_global_rectangle = ch[i];
                k_global_rectangle = ch[i+1];
                p_rectangle = p1;
                k_rectangle = k1;
            }
       }

       // Add points to rectangle
       rectangle << QPoint(x_min_rectangle, y_min_rectangle) << QPoint(x_max_rectangle, y_min_rectangle) << QPoint(x_max_rectangle, y_max_rectangle) << QPoint(x_min_rectangle, y_max_rectangle);

       //Decide Main Line of Rectangle
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

       // transform  direction and rectangle back to global system
       rotateByAngle(rectangle,-min_angle*(M_PI/180));
       rotateByAngle(direction,-min_angle*(M_PI/180));
       //polygonTransform(p_rectangle, k_rectangle, p_global_rectangle, k_global_rectangle, rectangle);
       //polygonTransform(p_rectangle, k_rectangle, p_global_rectangle, k_global_rectangle, direction);


}

void Algorithms::polygonTransform(QPoint p, QPoint k, QPoint p1, QPoint k1, QPolygon &pol)
{
    // Xa = Xp + b*delxpa - a*delypa
    // Ya = Yp + a*delxpa + b*delypa
    // a =(delxpk*delYpk - delypk*delXpk)/(delxpk*delxpk +delypk*delypk)
    // b =(delxpk*delXpk - delypk*delYpk)/(delxpk*delxpk +delypk*delypk)

    double angle = get2LinesAngle(p, k, p1, k1)*(M_PI/180);

    //double a = ((p.x()-k.x())*(p1.y()-k1.y()) - (p.y()-k.y())*(p1.x()-k1.x())) / ((p.x()-k.x())*(p.x()-k.x()) - (p.y()-k.y())*(p.y()-k.y()));
    //double b = ((p.x()-k.x())*(p1.y()-k1.y()) - (p.y()-k.y())*(p1.x()-k1.x())) / ((p.x()-k.x())*(p.x()-k.x()) - (p.y()-k.y())*(p.y()-k.y()));

    for(int i = 0; i<pol.size();i++)
    {
        QPoint point = pol[i];
        pol[i].setX(cos(angle)*(point.x()) + sin(angle)*(point.y()));
        pol[i].setY(-sin(angle)*(point.x()) + cos(angle)*(point.y()));
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

    double angle = get2LinesAngle(p, k, p1, k1)*(M_PI/180);

    //double a = ((p.x()-k.x())*(p1.y()-k1.y()) - (p.y()-k.y())*(p1.x()-k1.x())) / ((p.x()-k.x())*(p.x()-k.x()) - (p.y()-k.y())*(p.y()-k.y()));
    //double b = ((p.x()-k.x())*(p1.y()-k1.y()) - (p.y()-k.y())*(p1.x()-k1.x())) / ((p.x()-k.x())*(p.x()-k.x()) - (p.y()-k.y())*(p.y()-k.y()));

    QPoint first = pol.p1();
    QPoint second = pol.p2();

    pol.setP1(QPoint(cos(angle)*(first.x()) + sin(angle)*(first.y()), -sin(angle)*(first.x()) + cos(angle)*(first.y())));
    pol.setP2(QPoint(cos(angle)*(second.x()) + sin(angle)*(second.y()), -sin(angle)*(second.x()) + cos(angle)*(second.y())));
    //pol.setP1(QPoint(p1.x() + b*(first.x()-p.x()) - a*(first.y()-p.y()),p1.y() + a*(first.x()-p.x()) + b*(first.y()-p.y())));
    //pol.setP1(QPoint(p1.x() + b*(second.x()-p.x()) - a*(second.y()-p.y()),p1.y() + a*(second.x()-p.x()) + b*(second.y()-p.y())));
}

// Borrow from kalator, my transform function not work
void Algorithms::rotateByAngle(QPolygon &points, double angle)
{
    for(int i = 0; i < points.size(); i++)
    {
        QPointF temp_point = points[i];
        points[i].setX(cos(angle) * temp_point.x() + sin(angle) * temp_point.y());
        points[i].setY(-sin(angle) * temp_point.x() + cos(angle) * temp_point.y());
    }
}

// Borrow from kalator, my transform function not work
void Algorithms::rotateByAngle(QLine &points, double angle)
{
    QPointF p1 = points.p1();
    QPointF p2 = points.p2();

    points.setP1(QPoint(cos(angle) * p1.x() + sin(angle) * p1.y(), -sin(angle) * p1.x() + cos(angle) * p1.y()));
    points.setP2(QPoint(cos(angle) * p2.x() + sin(angle) * p2.y(), -sin(angle) * p2.x() + cos(angle) * p2.y()));
}

QPolygon Algorithms::GrahamScan (vector<QPoint> &points){

    double eps = 0.001;

    // sort by Y and add point with smallest Y to the convex hull
    std::sort(points.begin(), points.end(), sortByYAsc());

    // pivot with min y and max x
    QPoint q = points[0];

    QPolygon poly;

    for(unsigned int j =1; j < points.size(); j++){
        poly.push_back(points[j]);
    }


    QPolygon pointsByAngle;

    // sort by angle
    while(!poly.empty()){

        int ind_max = -1;
        double fi_max = 0;
        double fi;

        for (int i =0; i<poly.size(); i++){

            // count the angel with axis X according to quadrant
            //fi = (atan2(fabs((q.x()-poly[i].x())),fabs(q.y()-poly[i].y())))*180/M_PI;

            double alfa = (atan(fabs((q.x()-poly[i].x()))/fabs(q.y()-poly[i].y())))*180/M_PI;


            // Check the quadrant and fix the angel according to the axis X
            if((poly[i].x()-q.x())<0 && (poly[i].y()-q.y())>0){
                fi = 90 + alfa;
            }

            if((poly[i].x()-q.x())<0 && (poly[i].y()-q.y())<0){
                fi = 270 - alfa;
            }

            if((poly[i].x()-q.x())>0 && (poly[i].y()-q.y())<0){
                fi = 270 + alfa;
            }

            if((poly[i].x()-q.x())>0 && (poly[i].y()-q.y())>0){
                fi = 90 - alfa;
            }


            // check points with the same angel and remove point which is closer


            if(fabs((fi-fi_max))<eps){
                double distNew = sqrt((q.x()-poly[i].x())*(q.x()-poly[i].x()) + (q.y()-poly[i].y())*(q.y()-poly[i].y()));
                double distOld = sqrt((q.x()-poly[ind_max].x())*(q.x()-poly[ind_max].x()) + (q.y()-poly[ind_max].y())*(q.y()-poly[ind_max].y()));

                if(distOld<distNew){
                    poly.remove(ind_max);
                    ind_max = i;

                }
                else{
                    poly.remove(i);

                }
                i--;

            }


            // find the biggest angle with axis X
            if(fi>fi_max){
                fi_max = fi;
                ind_max = i;
            }

        }

        pointsByAngle<<(poly.takeAt(ind_max));
    }


    QPolygon ch;

    // add pivot and the first point from sorted points
    ch.push_back(q);
    ch.push_back(pointsByAngle[0]);

    // find out the position of point to line
    for(int i = 1; i < pointsByAngle.size(); i++)
       {
           bool doEvaluation  = true;

           while(doEvaluation )
           {
               if(getPointLinePosition(ch[ch.size()-1], ch[ch.size()-2], pointsByAngle[i]) == RIGHT)
                   ch.pop_back();

               else if(getPointLinePosition(ch[ch.size()-1], ch[ch.size()-2], pointsByAngle[i]) == ON)
                   ch.pop_back();

               else
                   doEvaluation  = false;
           }
           ch.push_back(pointsByAngle[i]);
       }

    exatlyCH(ch);

    return ch;

}

QPolygon Algorithms::exatlyCH(QPolygon ch)
{
    QPoint first = ch[0];
    QPoint second = ch[1];

    //Delete points on the same line
    for(int i=0; i<(ch.size()-2); i++){
        if(getPointLinePosition(ch[i+2],ch[i],ch[i+1])==ON){
            ch.remove(i+1);
            i--;
        }
    }

    return ch;
}

QPolygon Algorithms::deleteDuplicityCH(QPolygon ch)
{
    //Delete duplicit points
    for(int i = 0; i<(ch.size()-1); i++){
        for(int j = i+1; j<(ch.size()); j++){
            if((ch[j].x() == ch[i].x())&&(ch[j].y() == ch[i].y())){
                ch.remove(j);
                j--;
            }
        }
    }
    return ch;
}

QPolygon Algorithms::CHSweep (vector<QPoint> &points)
{

    //create convex hull using the Sweep line procedure
    QPolygon poly_ch;

    //sort by x coord asc
    std::sort(points.begin(), points.end(), SortByXAsc());

    //delete duplicit points on sorted data
    for(int i =0; i<points.size(); i++){
        if((points[i].x()==points[i+1].x()) && (points[i].y()==points[i+1].y())  ){
            points.erase(points.begin()+i);
            i--;
        }
    }

    //create list of predecessors (p) and successors (n)
    std::vector<int> p(points.size()), n(points.size());

    //create triangle from first 3 points
/*    if(getPointLinePosition(points[2], points[0], points[1]) == LEFT)
    {
        n[0] = 1;
        n[1] = 2;
        n[2] = 0;
        p[0] = 2;
        p[1] = 0;
        p[2] = 1;
    }
    else
    {
        n[0] = 2;
        n[2] = 1;
        n[1] = 0;
        p[0] = 1;
        p[2] = 0;
        p[1] = 2;
    }
*/    //create initial bi-angle from first 2 points
    n[0] = 1;
    n[1] = 0;

    p[0] = 1;
    p[1] = 0;

    for(unsigned int i = 2; i < points.size(); i++)
    {
        //does new point lie on upper/lower halfplane?
        //link i with predecessor/succesor

        if(getPointLinePosition(points[i], points[p[i-1]], points[i-1]) == LEFT)
        //if(points[i].y() >= points[i-1].y())
        {
            p[i] = i-1;
            n[i] = n[i-1];
        }
        else
        {
            p[i] = p[i-1];
            n[i] = i-1;
        }

        //link predecessor/successor with i
        n[p[i]] = i;
        p[n[i]] = i;

        //fix upper tangent
        while(getPointLinePosition(points[n[n[i]]], points[i], points[n[i]]) == RIGHT)
        {
            p[n[n[i]]] = i;
            n[i] = n[n[i]];

        }

        //fix lower tangent
        while(getPointLinePosition(points[p[p[i]]], points[i], points[p[i]]) == LEFT)
        {
            n[p[p[i]]] = i;
            p[i] = p[p[i]];
        }
    }

    //add points to poly_ch
    poly_ch.push_back(points[0]);

    int index = n[0];

    while(index != 0)
    {
        poly_ch.push_back(points[index]);
        index = n[index];
    }

    exatlyCH(poly_ch);

    return poly_ch;
}

QPolygon Algorithms::GrahamScanNew (vector<QPoint> &points)
{
    QPolygon ch;
    std::vector<vec_angle> angles;
    double eps = 0.001;
    int index = 0;

    // sort by Y and add point with smallest Y to the convex hull
    std::sort(points.begin(), points.end(), sortByYAsc());

    //Reduce singulary case  by kolinear points
    for(int i = 1;i<points.size();i++)
    {
        if(points[0].y()== points[i].y())
        {
            index++;
        }
    }

    // pivot with min y and max x
    QPoint q = points[index];
    ch.push_back(q);
    //points.erase(index);

    //Find s
    std::sort(points.begin(), points.end(), SortByXAsc());
    QPoint s(points[0].x(),q.y());

    //calculate angles beetwen pivot-axes X and pivot-some points
    vec_angle point;
    for(int i = 0; i<points.size();i++)
    {
        point.p.setX(points[i].x());
        point.p.setY(points[i].y());
        point.a = get2LinesAngle(q,s,q, points[i]);
        angles.push_back(point);
    }






}

