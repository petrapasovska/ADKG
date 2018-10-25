#include "algorithms.h"

Algorithms::Algorithms()
{

}

int Algorithms::getPositionRay(QPointF q, QPolygonF pol)
{
    //Analyze point and polygon position
    int k = 0;
    int n = pol.size();
    double eps = 1.0e-10;

    //Initialize the coordinates
    double xir = pol[0].x() - q.x();
    double yir = pol[0].y() - q.y();

    for(int i=0; i<pol.size(); i++){
        //vertex?
        if((fabs(q.x()-pol[i].x()) < eps) && (fabs(q.y()-pol[i].y()) < eps)){
            return 1;
        }
    }


    for(int i=1;i<pol.size() + 1; i++)
    {
        double xiir = pol[i%n].x() - q.x();
        double yiir = pol[i%n].y() - q.y();

        //find out if point lies on the line
        int t = getPointLinePosition(q, pol[i%n], pol[(i+1)%n]);

        if(t == -2){
            return 1;
        }

        //Upper halfplane?
        if(((yiir > eps) && (yir <= eps)) || ((yir > eps) && (yiir <= eps)))
        {
            //Right half plane?
            double xm = (xiir * yir - xir * yiir) / (yiir - yir);

            //Increment intersection
            if(xm >0)
                k++;
        }

        //Assign coordinates
        xir = xiir;
        yir = yiir;
    }

    return k%2;
}


int Algorithms::getPositionWinding(QPointF q, QPolygonF pol)
{
       int n = pol.size();
       double sum_fi=0.0;
       double eps = 1.0e-10;
       int k = 0;


       //Process all polygon segments
       for (int i=0; i < n; i++)
       {
           // reduce sigularity effect point on the vertex
           if( fabs(q.x()-pol[i].x())<eps && fabs(q.y()-pol[i].y())<eps )
           {
               k = -1;
           }
           else
           {
                //Get angle
                double fi = get2LinesAngle(pol[i], q, pol[(i+1)%n], q);

                //Get position
                int t = getPointLinePosition(q, pol[i], pol[(i+1)%n]);

                // q point on the line
                if(t==-2)
                    return 1;

                //Point q in the left halfplane
                if(t > 0)
                    sum_fi += fi;

                //Point in the right halfplane
                else
                    sum_fi -= fi;
           }
       }

    if(k==-1)
    {
        //Point q on the vertex
        return 1;
    }
    else
    {
       //Point q inside P
       if (fabs(fabs(sum_fi)-360)< eps)
           return 1;

       //Point q outside P
       if (fabs(sum_fi) < eps)
           return 0;

       //something else ???
       else
           return -1;
    }
}


int Algorithms::getPointLinePosition(QPointF &q, QPointF &a, QPointF &b)
{
    //Point and line position
    double eps = 1.0e-8;

    double ux = b.x() - a.x();
    double uy = b.y() - a.y();
    double vx = q.x() - a.x();
    double vy = q.y() - a.y();

    //Determinant
    double t = (ux * vy - uy * vx);

    //Point in the left half plane
    if(t > eps)
        return 1;

    //Point in the right half plane
    if(t < -eps)
        return 0;

    //Point on the line
    //distance between point q, a, b
    double eps2 = 2.0;
    double dist_qa = sqrt((vx*vx)+(vy*vy));
    double dist_qb = sqrt((q.x()-b.x())*(q.x()-b.x()) + (q.y()-b.y())*(q.y()-b.y()));
    double dist_ab = sqrt((ux*ux)+(uy*uy));
    double dist = dist_qa + dist_qb;

    if((fabs(dist-dist_ab))<=eps2){
        return -2;
    }

    return -1;
}


double Algorithms::get2LinesAngle(QPointF &p1,QPointF &p2,QPointF &p3, QPointF &p4)
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
