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

    qDebug() << "Clean canvas " << endl;
    clearCanvas();

    if(n_points < 4){
        QMessageBox msgBox;
        msgBox.setText("Warning: The input has to be at least 4");
        msgBox.exec();
        return;
    }

    n_points = n_points -3;

    // generate polygons by graham scan- dikobraz
    QPolygonF poly;


    //generate x points and save into vector
    for(int i = 0; i < n_points - 1; i++)
    {
        QPointF point;
        point.setX(rand()%500);
        point.setY(rand()%500);
        poly.push_back(point);
    }

    QPolygonF polygons;

    for(int j = 0; j < n_points -1; j++){
        QPointF min = poly.at(0);
        int indexSwap = 0;

        for(int i = 1; i < n_points - 1 -j; i++) {
            if(min.y() > poly.at(i).y())
            min = poly.at(i);
        indexSwap = i;
        }

        polygons<<(poly.takeAt(indexSwap));
    }

    qDebug() << polygons;

    // generate center of polygons
    QPointF center;

    double prumerX = 0;
    double prumerY = 0;

    for(int i = 0; i < n_points -1; i++){
        prumerX = prumerX + polygons.at(i).x();
        prumerY = prumerY + polygons.at(i).y();
    }

    center.setX(prumerX/n_points);
    center.setY(prumerY/n_points);

    polygons<<center;

    qDebug() << polygons;

    QPainter painter(this);
    painter.begin(this);
    painter.drawPolygon(polygons);
}
