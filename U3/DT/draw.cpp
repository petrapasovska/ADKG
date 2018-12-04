#include "draw.h"

#include <QtGui>
#include <QBrush>
#include <Qt>

Draw::Draw(QWidget *parent) : QWidget(parent)
{
    srand((unsigned)time(0));
}

void Draw::paintEvent(QPaintEvent *e)
{
   QPainter painter(this);
   painter.begin(this);
   painter.setPen(Qt::red);

   //Draw points
   for(int i = 0; i < points.size(); i++)
   {
       painter.drawEllipse(points[i].x() - 5, points[i].y() - 5, 10, 10);
       //painter.drawText(points[i].x() + 10, points[i].y() + 10, QString::number(points[i].getZ()));
   }

   //Draw Delaunay edges
   for(int i = 0; i < dt.size(); i++)
   {
       painter.drawLine(dt[i].getS(), dt[i].getE());
   }

   //Draw contour lines
   painter.setPen(Qt::black);

   for(int i = 0; i < contours.size(); i++)
   {
       painter.drawLine(contours[i].getS(), contours[i].getE());
   }

   if(slope == TRUE)
   {
       //Draw slope
       double c = 255.0/180;
       for(int i = 0; i < dtm.size(); i++)
       {
           //Get triangle and its vertices
           Triangle t = dtm[i];
           QPoint3D p1 = t.getP1();
           QPoint3D p2 = t.getP2();
           QPoint3D p3 = t.getP3();

           //Get slope and set the brush
           int c_slope = c * t.getSlope();
           painter.setBrush(QColor(c_slope,c_slope,c_slope));

           //Create and draw the polygon
           QPolygon triangle;
           triangle.append(QPoint(p1.x(), p1.y()));
           triangle.append(QPoint(p2.x(), p2.y()));
           triangle.append(QPoint(p3.x(), p3.y()));

           painter.drawPolygon(triangle);
       }
   }

   if(aspect == TRUE)
   {
       //Draw aspect
       for(int i =0; i<dtm.size(); i++)
       {
           //Get triangle and its vertices
           Triangle t = dtm[i];
           QPoint3D p1 = t.getP1();
           QPoint3D p2 = t.getP2();
           QPoint3D p3 = t.getP3();

           int count_aspect = t.getAspect();

           if((count_aspect>=0) && (count_aspect<22.5)){
               painter.setBrush(QColor(0, 206, 209));
           }
           else if((count_aspect>=22.5) && (count_aspect<67.5)){
               painter.setBrush(QColor(65, 105, 225));
           }
           else if((count_aspect>=67.5) && (count_aspect<112.5)){
               painter.setBrush(QColor(186, 85, 211));
           }
           else if((count_aspect>=112.5) && (count_aspect<157.5)){
              painter.setBrush(QColor(255, 20, 147));
           }
           else if((count_aspect>=157.5) && (count_aspect<180)){
               painter.setBrush(QColor(220, 20, 60));
           }
           else if((count_aspect>=-180) && (count_aspect<-157.5)){
               painter.setBrush(QColor(220, 20, 60));
           }
           else if((count_aspect>=-157.5) && (count_aspect<-112.5)){
                painter.setBrush(QColor(255, 165, 0));
           }
           else if((count_aspect>=-112.5) && (count_aspect<-67.5)){
               painter.setBrush(QColor(255, 255, 0));
           }
           else if((count_aspect>=-67.5) && (count_aspect<-22.5)){
               painter.setBrush(QColor(124, 252, 0));
           }
           else if((count_aspect>=-22.5) && (count_aspect<0)){
               painter.setBrush(QColor(0, 206, 209));
           }


           //Create and draw the polygon
           QPolygon triangle;
           triangle.append(QPoint(p1.x(), p1.y()));
           triangle.append(QPoint(p2.x(), p2.y()));
           triangle.append(QPoint(p3.x(), p3.y()));

           painter.drawPolygon(triangle);

       }

   }

   painter.end();
}

void Draw::mousePressEvent(QMouseEvent *e)
{
    /*
    //Add new point
    int z = (rand()%101);
    QPoint3D p(e->x(), e->y(), z);
    points.push_back(p);
    repaint();
    */
}

void Draw::clearDT()
{
    //Clear all
    points.clear();
    dt.clear();
    dtm.clear();
    contours.clear();
}

void Draw::importPolygons(std::string &path, std::vector<QPoint3D> &points,  QSizeF &canvas_size, double &min_z, double &max_z)
{
    double x, y, z;
    QPoint3D p;


    //go through file and load points into poly_pol (storing all polygons)
    double min_x = std::numeric_limits<double>::max();
    double min_y = std::numeric_limits<double>::max();
    double max_x = std::numeric_limits<double>::min();
    double max_y = std::numeric_limits<double>::min();
    min_z = std::numeric_limits<double>::max();
    max_z = std::numeric_limits<double>::min();


    std::ifstream myfile(path);
    if(myfile.is_open())
    {
        qDebug() << "File is open";
        while(myfile >> x >> y >> z)        //read file line by line
        {
            p.setX(x);
            p.setY(y);
            p.setZ(z);

            points.push_back(p);

            if(x < min_x) min_x = x;
            if(x > max_x) max_x = x;
            if(y < min_y) min_y = y;
            if(y > max_y) max_y = y;
            if(z < min_z) min_z = z;
            if(z > max_z) max_z = z;
        }

        myfile.close();
    }

    //scale points to canvas size
        double h = canvas_size.height() - 40;
        double w = canvas_size.width() - 40;

        double x_coef = w/(max_x-min_x);
        double y_coef = h/(max_y-min_y);

        for(unsigned int i = 0; i < points.size(); i++)
        {
            points[i].setX((points[i].x()-min_x)*x_coef);
            points[i].setY((points[i].y()-min_y)*y_coef);
        }
}
