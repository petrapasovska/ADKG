#include <cmath>
#include <QtGui>
#include <QMessageBox>
#include "draw.h"
#include "algorithms.h"
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

    //Draw import polygons
    for(unsigned int i = 0; i < polygons.size(); i++)
    {
        //Current polygon from file
        QPolygonF this_polygon = polygons[i];
        //Draw polygon
        painter.drawPolygon(this_polygon);
    }

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

    //Draw q
    painter.drawEllipse(static_cast<int>(q.x()) -10, static_cast<int>(q.y()) -10, 20, 20);

    //Stop drawing
    painter.end();
}

void Draw::mousePressEvent(QMouseEvent *e)
{
    if(!draw_point)
    {
        return;
    }
    q.setX(e->x());
    q.setY(e->y());
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

void Draw::generatePolygon(int n_points)
{
    clearCanvas();

    // error about wrong input
    if(n_points < 4){
        QMessageBox msgBox;
        msgBox.setText("Warning: The input has to be at least 4");
        msgBox.exec();
        return;
    }

    // the last point in polygon would be center
    n_points = n_points -1;

    // generate polygons by graham scan- dikobraz
    QPolygonF poly;

    //generate x and y points in interval <0; 499>
    for(int i = 0; i < n_points; i++)
    {
        QPointF point;
        point.setX(rand()%500);
        point.setY(rand()%500);
        poly.push_back(point);
    }

    QPolygonF polygon;


    // sort points by minimum and the smallest number save into polygon
    while(!poly.empty()){
        int indexMin = 0;
        for(int i = 1; i < poly.size(); i++) {
            if(poly.at(indexMin).y() > poly.at(i).y()){
                indexMin = i;
            }
        }
        polygon<<(poly.takeAt(indexMin));
    }



    // generate center of polygon
    QPointF center;

    double prumerX = 0;
    double prumerY = 0;

    for(int i = 0; i < n_points; i++){
        prumerX = prumerX + polygon.at(i).x();
        prumerY = prumerY + polygon.at(i).y();
    }

    center.setX(prumerX/n_points);
    center.setY(prumerY/n_points);


    QPolygonF polygon2;


    // Sort points in polygon to get topological correct polygon
    while(!polygon.empty()){

        int indMin = 0;
        double uhelMin = 360;
        double uhel = 0;


        // Count angel between axis Y and line - this is called SMERNIK in Czech
        // Sorry for using czech expressions for variables :(
        for(int i = 0; i < polygon.size(); i++) {
            double smernik = atan(fabs((polygon[i].x()-center.x()))/fabs(polygon[i].y()-center.y()));


            // Check the quadrant and fix the angel
            if((polygon[i].x()-center.x())<0 && (polygon[i].y()-center.y())>0){
                uhel = smernik;
            }

            if((polygon[i].x()-center.x())<0 && (polygon[i].y()-center.y())<0){
                uhel = 180-smernik;
            }

            if((polygon[i].x()-center.x())>0 && (polygon[i].y()-center.y())<0){
                uhel = 180+smernik;
            }

            if((polygon[i].x()-center.x())>0 && (polygon[i].y()-center.y())>0){
                uhel = 360-smernik;
            }

            // find the smallest so the angels would be in clockwise and final polygon would be topological correct
            if(uhel<uhelMin){
                uhelMin = uhel;
                indMin = i;
            }
        }

        // Delete minimum and add it to another polygon so you can count with the rest
        polygon2<<(polygon.takeAt(indMin));
    }

    polygon2<<center;

    polygons.push_back(polygon2);

    qDebug()<<polygons;

    repaint();
}
