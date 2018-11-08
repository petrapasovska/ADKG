#include "draw.h"


Draw::Draw(QWidget *parent) : QWidget(parent)
{

}

void Draw::paintEvent(QPaintEvent *e)
{
   QPainter painter(this);
   painter.begin(this);
   painter.setPen(Qt::red);

   //Draw convex hull
   painter.drawPolygon(ch);

   //Draw rectangle minBoundingBox
   painter.setPen(Qt::green);
   painter.drawPolygon(rectangle);


   //Draw direction minBoundingBox
   painter.setPen(Qt::blue);
   painter.drawLine(direction);

   //Draw points
   painter.setPen(Qt::black);
   for(int i = 0; i < points.size(); i++)
   {
       painter.drawEllipse(points[i].x()-5, points[i].y()-5, 10, 10);
   }


   painter.end();


}

void Draw::mousePressEvent(QMouseEvent *e)
{
    QPoint p(e->x(), e->y());

    points.push_back(p);

    repaint();

}

void Draw::clearCanvas()
{
    //Clear the Canvas
    ch.clear();
    points.clear();
    rectangle.clear();
    repaint();
}

std::vector<QPoint> Draw::generateSquare(int n)
{
    std::vector<QPoint> random_points;
    QPoint p(QRandomGenerator::global()->bounded(100)+10,QRandomGenerator::global()->bounded(100)+10);
    double length = QRandomGenerator::global()->bounded(400);

    QPoint p1(p.x()+length,p.y());
    QPoint p2(p.x()+length,p.y()+length);
    QPoint p3(p.x(),p.y()+length);
    random_points.push_back(p);
    random_points.push_back(p1);
    random_points.push_back(p2);
    random_points.push_back(p3);

    double length_small = length/(n/4);
    for(int i = 0;i < n/4;i++)
    {
        random_points.push_back(QPoint(p.x()+(i*length_small),p.y()));
        random_points.push_back(QPoint(p.x()+length,p.y()+(i*length_small)));
        random_points.push_back(QPoint(p.x()+(i*length_small),p.y()+length));
        random_points.push_back(QPoint(p.x(),p.y()+(i*length_small)));
    }
    return random_points;
}

std::vector<QPoint> Draw::generateCircle(int n)
{
    std::vector<QPoint> random_points;
    QPoint p;
    QPoint center(QRandomGenerator::global()->bounded(300)+200,QRandomGenerator::global()->bounded(300)+200);
    double radius = QRandomGenerator::global()->bounded(200);
    double fi = (2*M_PI)/n;
    double x, y;

    for(int i = 0;i<n;i++)
    {

        p.setX(center.x() + radius*cos(i*fi));
        p.setY(center.y() + radius*sin(i*fi));
        random_points.push_back(p);
    }
    return random_points;
}

std::vector<QPoint> Draw::generateEclipse(int n)
{
    std::vector<QPoint> random_points;
    QPoint p;
    QPoint center(QRandomGenerator::global()->bounded(300)+200,QRandomGenerator::global()->bounded(300)+200);
    double a = QRandomGenerator::global()->bounded(200);
    double b = QRandomGenerator::global()->bounded(200);
    double fi = (2*M_PI)/n;

    for(int i = 0;i<n;i++)
    {

        p.setX(center.x() + a*cos(i*fi));
        p.setY(center.y() + b*sin(i*fi));
        random_points.push_back(p);
    }
    return random_points;
}

std::vector<QPoint> Draw::generateStarShape(int n)
{
    std::vector<QPoint> random_points;
    n/=2;
    QPoint p;
    QPoint center(QRandomGenerator::global()->bounded(300)+200,QRandomGenerator::global()->bounded(300)+200);
    double a = QRandomGenerator::global()->bounded(200);
    double b = QRandomGenerator::global()->bounded(200);
    double fi = (2*M_PI)/n;

    for(int i = 0;i<n;i++)
    {

        p.setX(center.x() + a*cos(i*fi));
        p.setY(center.y() + a*sin(i*fi));
        random_points.push_back(p);

        p.setX(center.x() + b*cos(i*fi+fi/2));
        p.setY(center.y() + b*sin(i*fi+fi/2));
        random_points.push_back(p);
    }
    return random_points;
}

std::vector<QPoint> Draw::generateRandomPoints(int n)
{
     std::vector<QPoint> random_points;
     QPoint p;
     for(int i = 0;i<n;i++)
     {
         p.setX(QRandomGenerator::global()->bounded(600));
         p.setY(QRandomGenerator::global()->bounded(600));
         random_points.push_back(p);
     }
     return random_points;
}
std::vector<QPoint> Draw::generateGrid(int n)
{
     std::vector<QPoint> random_points;

     //generate size of window
     double a = QRandomGenerator::global()->bounded(600);
     double b = QRandomGenerator::global()->bounded(600);

     n/=2;
     //size of grid
     a/=n;

     // set first point
     random_points.push_back(QPoint(0,0));

     for(int i = 0;i<n;i++)
     {

         for(int j =0;j<n;j++)
         {
             random_points.push_back(QPoint(random_points[0].x()+i*a,random_points[0].y()+j*a));
         }
     }
     return random_points;
}
