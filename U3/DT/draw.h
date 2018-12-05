#ifndef DRAW_H
#define DRAW_H

#include <QWidget>
#include <vector>
#include <fstream>
#include <QMessageBox>

#include "edge.h"
#include "qpoint3d.h"
#include "triangle.h"
#include "algorithms.h"

class Draw : public QWidget
{
    Q_OBJECT
    private:
        std::vector<QPoint3D> points;     //List of points
        std::vector<Edge> dt;             //List of Delaunay edges
        std::vector<Edge> contours;       //List of contours
        std::vector<Triangle> dtm;
        bool slope, aspect;

    public:
        explicit Draw(QWidget *parent = nullptr);
        void paintEvent(QPaintEvent *e);
        void mousePressEvent(QMouseEvent *e);
        void clearPoints() {points.clear();}
        void clearDT();
        void setPoints(std::vector<QPoint3D> points_){points = points_;}
        std::vector<QPoint3D> & getPoints(){return points;}
        std::vector<Edge> & getDT(){return dt;}
        void setDT(std::vector<Edge> &dt_){dt = dt_;}
        void setContours(std::vector<Edge> &contours_){contours = contours_;}
        void setDTM(std::vector<Triangle> &dtm_){dtm = dtm_;}
        static void importPolygons(std::string &path, std::vector<QPoint3D> &points,  QSizeF &canvas_size, double &min_z, double &max_z);
        void setSlope(bool slope_){slope = slope_;}
        void setAspect(bool aspect_){aspect = aspect_;}


    signals:

    public slots:

};

#endif // DRAW_H
