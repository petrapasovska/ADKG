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
    for(int i=0; i<pol.size(); i++)
        p.append(pol[i]);

    //Draw import polygons
    for(int i = 0; i < polygons.size(); i++)
    {
        //Current polygon from file
        QPolygonF this_polygon = polygons[i];
        //Draw polygon
        painter.drawPolygon(this_polygon);
    }

    //Draw generate polygon
    painter.drawPolygon(polygon_generate);

    // set Brush
    QPolygonF polygon_result;
    QBrush brush;
    brush.setColor(Qt::red);
    brush.setStyle(Qt::SolidPattern);
    QPainterPath path;

    // Draw polygon with point inside
    for(int i = 0; i < result.size(); i++)
    {
        if(result[i] == 1 /*|| result[i] == 2 || result[i] == 3*/)
        {
            for(int j = 0; j < polygons.size(); j++)
                {
                    polygon_result << polygons[j];
                }
                painter.drawPolygon(polygon_result);
                painter.fillPath(path, brush);
                polygon_result.clear();
        }
    }

    // Draw polygon
    painter.drawPolygon(p);

    //Draw the points
    for(int i=0; i<pol.size(); i++)
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
    polygons.clear();
    polygon_generate.clear();
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
               if(polygon.empty() == FALSE)
               {
                   polygons.push_back(polygon);
               }
               polygon.clear();
               polygon << QPointF(x, y);
            }
            else
            {
                polygon << QPointF(x, y);
            }
        }

        polygons.push_back(polygon); // save last polygon
        polygon.clear();
        for(int i = 0;i<polygons.size();i++)
        {
            qDebug() << polygons[i];
        }
        myfile.close();
    }
}

void Draw::generatePolygon(int n_points, int coordinates_max)
{
    qDebug() << "Generate polygons start";
    // generate polygons by graham scan- dikobraz
    QPolygonF poly;
    QPointF point;

    //generate x points and save into vector
    for(int i = 0; i<n_points; i++)
    {
        point.setX(QRandomGenerator::global()->bounded(coordinates_max));
        point.setY(QRandomGenerator::global()->bounded(coordinates_max));

        poly.push_back(point);
    }

    // generate center of polygons
    QPointF center;
    center.setX(QRandomGenerator::global()->bounded(coordinates_max));
    center.setY(QRandomGenerator::global()->bounded(coordinates_max));

    // parralel axes with axes X
    QPointF centerX;
    centerX.setX(center.x()+coordinates_max);
    centerX.setY(center.y());
    double sum_x = 0;
    double sum_y = 0;

    // sort angle by size with index point
    // every polygon will have center and some points
    std::vector<double> angles;

    for(int i = 0; i<n_points;i++)
    {
       double angle = Algorithms::get2LinesAngle(center,centerX,center,poly[i]);
       angles.push_back(angle);
       sum_x += poly[i].x();
       sum_y += poly[i].y();
    }

    for(int i = 0;i<n_points;i++)
    {
        int n = selectMinIndex(angles);
        polygon_generate.append(poly[n]);
        angles[n]=999;
    }
    polygon_generate.append(center);
    centerX.setX(sum_x/n_points);
    centerX.setY(sum_y/n_points);

    qDebug() << "Generate polygons stop";
}

int Draw::selectMinIndex(std::vector<double> v)
    {
        int min_position = 0;

        for(int i = 0; i<v.size();i++)
        {
            if (v[i] < v[min_position]) // Found a smaller min
            {
                min_position = i;
            }
        }
        return min_position;
    }
