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

    for(int i=1;i<pol.size() + 1; i++)
    {
        // reduce sigularity effect point on the vertex
        if( fabs(q.x()-pol[i].x())<eps && fabs(q.y()-pol[i].y())<eps || fabs(q.x()-pol[0].x())<eps && fabs(q.y()-pol[0].y())<eps)
        {
            k = -1;
        }
        else
        {
            double xiir = pol[i%n].x() - q.x();
            double yiir = pol[i%n].y() - q.y();

            //Upper halfplane?
            if((yiir > 0) && (yir <= 0) || (yir > 0) && (yiir <= 0))
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
    }

    if(k==-1)
    {
        //Point q on the vertex
        return 3;
    }
    else
        return k%2;
}


int Algorithms::getPositionWinding(QPointF q, QPolygonF pol)
{
       int n = pol.size();
       double sum_fi=0.0;
       double eps = 1.0e-10;
       int k;


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
        return 3;
    }
    else
    {
       //Point q inside P
       if (fabs(fabs(sum_fi)-360)< eps)
           return 1;

       //Point q outside P
       if (fabs(sum_fi) < eps)
           return 0;

       //Point q on the boundary
       if (fabs(sum_fi)-180< eps)
           return 2;

       //something else ???
       else
           return -1;
    }
}


int Algorithms::getPointLinePosition(QPointF &q, QPointF &a, QPointF &b)
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
        return 1;

    //Point in the right half plane
    if(t < -eps)
        return 0;

    //Point on the line
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

