#include<cmath>
#include "draw.h"
#include <QMessageBox>


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

   //Draw points
   for(unsigned int i = 0; i < points.size(); i++)
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
    repaint();
}

std::vector<QPoint> Draw::generateSquare(int n)
{
    std::vector<QPoint> random_points;

    if(n<4){
        QMessageBox msgBox;
        msgBox.setText("Warning: The input has to be at least 4 to make a square.");
        msgBox.exec();
        return random_points;
    }

    if(!(n%4==0)){
        QMessageBox msgBox;
        msgBox.setText("Warning: The input has to be dividing by 4 to make a square. That's why the input is reduced to appropriate number");
        msgBox.exec();
    }

    QPoint p(rand()%100+10,rand()%100+10);
    double length = rand()%400;

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
    QPoint center(rand()%300+200,rand()%300+200);
    double radius = rand()%200;
    double fi = (2*M_PI)/n;

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
    QPoint center(rand()%300+200,rand()%300+200);
    double a = rand()%200;
    double b = rand()%200;
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
    QPoint center(rand()%300+200,rand()%300+200);
    double a = rand()%200;
    double b = rand()%200;
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
         p.setX(rand()%600+10);
         p.setY(rand()%600+10);
         random_points.push_back(p);
     }
     return random_points;
}
std::vector<QPoint> Draw::generateGrid(int n)
{
    std::vector<QPoint> random_points;

    if(n<4){
        QMessageBox msgBox;
        msgBox.setText("Warning: The input has to be at least 4 to make a grid.");
        msgBox.exec();
        return random_points;
    }

    if(!(n%4==0)){
        QMessageBox msgBox;
        msgBox.setText("Warning: The input has to be reduced to make regular grid");
        msgBox.exec();
    }

     //generate size of window
     double a = rand()%200;
     double b = rand()%200;

     //size of grid
     int size = sqrt(n);
     int pocet = 0;

     // set first point
     random_points.push_back(QPoint(10,10));

     for(int i = 0;i<size;i++)
     {

         for(int j =0;j<size;j++)
         {
             random_points.push_back(QPoint(random_points[0].x()+i*a,random_points[0].y()+j*b));
             pocet = pocet +1;

         }
     }

     qDebug() << pocet;

     return random_points;
}
