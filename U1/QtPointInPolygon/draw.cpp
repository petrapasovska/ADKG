#include "draw.h"
#include "algorithms.h"
#include <QtGui>
#include "qdebug.h"
#include <QMessageBox>

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

    //Draw import polygons
    for(unsigned int i = 0; i < polygons.size(); i++)
    {
        //Current polygon from file
        QPolygonF this_polygon = polygons[i];
        //Draw polygon
        painter.drawPolygon(this_polygon);
    }

    //Draw generate polygon
    //painter.drawPolygon(polygon_generate);

    QPolygonF polygon_result;
    // set Brush
    QBrush brush;
    brush.setColor(Qt::red);
    brush.setStyle(Qt::Dense3Pattern);
    QPainterPath path;

    // Draw polygon with point inside
    for(unsigned int i = 0; i < result.size(); i++)
    {
        qDebug() << "In Draw polygon result ";
        qDebug() << result[i];
        if(result[i] == 1)
        {
            polygon_result << polygons[i];
            qDebug() << polygon_result;
            path.addPolygon(polygon_result);
            painter.fillPath(path, brush);
            painter.drawPolygon(polygon_result);
            polygon_result.clear();

        }
    }

    //Draw the points
    //for(int i=0; i<pol.size(); i++)
        //painter.drawEllipse(pol[i].x() - 5, pol[i].y() -5, 10, 10);

    //Draw q
    painter.drawEllipse(static_cast<int>(q.x()) -10, static_cast<int>(q.y()) -10, 20, 20);

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

    /*Add point to the polygon
    else
    {
        QPointF point(e->x(), e->y());
        pol.push_back(point);
    }
    */
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
   // polygon_generate.clear();
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
        for(unsigned int i = 0;i<polygons.size();i++)
        {
            qDebug() << polygons[i];
        }
        myfile.close();

        QMessageBox msgBox;
        msgBox.setText("File has been succesfully imported!!");
        msgBox.exec();
        return true;
    }

    QMessageBox msgBox;
    msgBox.setText("Warning: There is unexpected problem with import file!!");
    msgBox.exec();
    return false;
}

void Draw::generatePolygon(int n_points, int coordinates_max)
{
    qDebug() << "Clean canvas " << endl;
    clearCanvas();
    qDebug() << "Generate polygons start into max " << coordinates_max << endl;
    qDebug() << "number of points in polyon " << n_points << endl;

    if(n_points < 4){
        QMessageBox msgBox;
        msgBox.setText("Warning: The input has to be at least 4");
        msgBox.exec();
        return;
    }


    // generate polygons by graham scan- dikobraz
    QPolygonF poly;
    //QPointF point;

    //generate x points and save into vector
    /*
    for(int i = 0; i < n_points - 1; i++)
    {
        QPointF point;
        point.setX(rand()%500);
        point.setY(rand()%500);
        poly.push_back(point);
    }
    */
    QPointF point1;
    point1.setX(0);
    point1.setY(0);
    poly.push_back(point1);

    QPointF point2;
    point2.setX(0);
    point2.setY(500);
    poly.push_back(point2);

    QPointF point4;
    point4.setX(500);
    point4.setY(500);
    poly.push_back(point4);

    QPointF point3;
    point3.setX(500);
    point3.setY(0);
    poly.push_back(point3);
    /*
    // generate center of polygons
    QPointF center;
    center.setX(rand()%500);
    center.setY(rand()%500);
    //polygon_generate.append(center);

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
      //  polygon_generate.append(poly[n]);
        poly.append(poly[n]);
        angles[n]=999;
    }

    centerX.setX(sum_x/n_points);
    centerX.setY(sum_y/n_points);
    //polygon_generate.push_back(center);
    poly.push_back(center);

    */
    polygons.push_back(poly);
    qDebug() << "Generate polygons stop";
}
