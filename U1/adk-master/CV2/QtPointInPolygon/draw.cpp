#include "draw.h"
#include "algorithms.h"
#include <QtGui>
#include "qdebug.h"

Draw::Draw(QWidget *parent) : QWidget(parent)
{
    //Set the initial values
    draw_point = false;
    q.setX(-5);
    q.setY(-5);
}

void Draw::paintEvent(QPaintEvent *e)
{
    //Draw the polygon and the point
    QPainter painter(this);

    //Start drawing
    painter.begin(this);

    //Create the polygon
    QPolygonF p;

    //Add points to the polygon
    for(unsigned int i=0; i<pol.size(); i++)
        p.append(pol[i]);

    for(unsigned j = 0; j < polygons.size(); j++)
    {
        //Current polygon from file
        QPolygonF this_polygon = polygons[j];
        qDebug() << polygons.size();
        //Draw polygon
        painter.drawPolygon(this_polygon);
    }

    // color polygon if point is inside
    if(result==1)
    {
        // Brush
        QBrush brush;
        brush.setColor(Qt::red);
        brush.setStyle(Qt::SolidPattern);

        // Fill polygon
        QPainterPath path;
        path.addPolygon(p);

        painter.fillPath(path, brush);
    }

    // Draw polygon
    painter.drawPolygon(p);

    //Draw the points
    for(unsigned int i=0; i<pol.size(); i++)
        painter.drawEllipse(pol[i].x() - 5, pol[i].y() -5, 10, 10);

    //Draw q
    painter.drawEllipse(q.x() - 2.5, q.y() - 2.5, 5, 5);

    //Stop drawing
    painter.end();
}

void Draw::mousePressEvent(QMouseEvent *e)
{
    //Set point q
    if(draw_point)
    {
        q.setX(e->x());
        q.setY(e->y());
    }

    //Add point to the polygon
    else
    {
        QPointF point(e->x(), e->y());
        pol.push_back(point);
    }

    //Repaint the screen
    repaint();

}

void Draw::setDrawPoint()
{
    //Switch, whether to draw a point or a polygon
    draw_point = !draw_point;
}

void Draw::clearCanvas()
{
    //Clear the Canvas
    pol.clear();
    q.setX(-5);
    q.setY(-5);

    repaint();
}

bool Draw::importPolygons(std::string &path)
{
    qDebug() << "Cliked import polygon";

    // iner variables for import
    int n;                                  //nuber of vertex in polygon
    double x;
    double y;
    QPolygonF polygon;

    std::ifstream myfile(path);
    if(myfile.is_open())
    {
        qDebug() << "File is open";
        while(myfile >> n >> x >> y)        //read file line by line
        {
            if(n == 1)                      // if read vertex of new polygon, push_back vector pol to vector polygons
            {
               polygons.push_back(polygon);
               polygon.clear();
               polygon << QPointF(x, y);
               qDebug() << x << y;
            }
            else
            {
                polygon << QPointF(x, y);
                qDebug() << x << y;
            }
        }
        myfile.close();
    }
    qDebug() << polygons.size();
}

void Draw::generatePolygon(int n_points, int coordinates_max)
{
    qDebug() << "Generate polygons start";
    // generate polygons by graham scan- dikobraz
    QPolygonF poly;
    QPointF point;

    //generate x points and save into vector
    for(unsigned int i = 0; i<n_points; i++)
    {
        point.setX(QRandomGenerator::global()->bounded(coordinates_max));
        point.setY(QRandomGenerator::global()->bounded(coordinates_max));

        poly.push_back(point);
        qDebug() << point.x() << point.y();
    }

    // generate center of polygons
    QPointF center;
    center.setX(QRandomGenerator::global()->bounded(coordinates_max));
    center.setY(QRandomGenerator::global()->bounded(coordinates_max));

    // parralel axes with axes X
    QPointF centerX;
    centerX.setX(center.x()+coordinates_max);
    centerX.setY(center.y());

    // sort angle by size with index point
    // every polygon will have center and some points
    QPolygonF polygo;
    std::vector<double> angles;

    for(unsigned int i = 0; i<n_points;i++)
    {
       double angle = Algorithms::get2LinesAngle(center,centerX,center,poly[i]);
       angles.push_back(angle);
    }

    for(unsigned int i = 0;i<n_points;i++)
    {
        int n = selectMinIndex(angles);
        polygo.append(poly[n]);
        angles[n]=999;
    }
    polygo.append(center);

    for(unsigned int i = 0;i<n_points;i++)
    {
        qDebug() << polygo[i];
    }
    qDebug() << center;
    qDebug() << "Generate polygons stop";
}

int Draw::selectMinIndex(std::vector<double> v)
    {
        int min_position = 0;

        for(unsigned int i = 0; i<v.size();i++)
        {
            if (v[i] < v[min_position]) // Found a smaller min
            {
                min_position = i;
            }
        }
        return min_position;
    }

void Draw::fillPolygon(int res)
{

    result = res;

}
