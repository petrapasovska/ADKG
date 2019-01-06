#include "draw.h"

Draw::Draw(QWidget *parent) : QWidget(parent)
{
    ab = true;
    /*
    QPointFB p1(0,0);
    QPointFB p2(100,0);
    QPointFB p3(0,100);

    polA.push_back(p1);
    polA.push_back(p2);
    polA.push_back(p3);

    QPointFB p4(50,10);
    QPointFB p5(150,10);
    QPointFB p6(50,110);

    polB.push_back(p4);
    polB.push_back(p5);
    polB.push_back(p6);
    */
}

void Draw::paintEvent(QPaintEvent *e)
{
    QPainter painter (this);

    painter.begin(this);

    //Draw polygon A
    painter.setPen(Qt::green);
    drawPol(polA, painter);

    //Draw polygon B
    painter.setPen(Qt::blue);
    drawPol(polB, painter);

    //Draw result
    painter.setPen(QPen(Qt::magenta, 3));
    painter.setBrush(Qt::BDiagPattern);
    for(std::vector<QPointFB> vec: res)
    {
        drawPol(vec, painter);
    }

    //Draw offset
    painter.setPen(Qt::cyan);
    for(std::vector<QPointFB> b: buff)
    {
        drawPol(b, painter);
    }
}

void Draw::drawPol(std::vector<QPointFB> &pol, QPainter &painter)
{
    QPolygon pol_q;

    for(QPointFB p: pol)
    {
        QPoint pb (p.x(), p.y());
        pol_q.append(pb);
    }

    painter.drawPolygon(pol_q);
}

void Draw::mousePressEvent(QMouseEvent *e)
{
    QPointFB p(e->x(), e->y());

    //Add to polA
    if(ab)
    {
        polA.push_back(p);
    }

    //Add to polB
    else
    {
        polB.push_back(p);
    }

    repaint();
}

void Draw::clearAll()
{
    // Clear all in canvas
    polA.clear();
    polB.clear();
    res.clear();
}

void Draw::clearResults()
{
    // Clear results in canvas
    res.clear();
}

void Draw::importPolygons(std::string &path,std::vector<QPointFB> &A, std::vector<QPointFB> &B,  QSizeF &canvas_size)
{
    int n;
    double x, y;
    QPointFB p;

    //go through file and load points into poly_pol (storing all polygons)
    double min_x = std::numeric_limits<double>::max();
    double min_y = std::numeric_limits<double>::max();
    double max_x = std::numeric_limits<double>::min();
    double max_y = std::numeric_limits<double>::min();

    std::ifstream myfile(path);
    if(myfile.is_open())
    {
        A.clear();
        B.clear();
        while(myfile >> n >> x >> y)        //read file line by line
               {
                    // Set min max for Canvas
                    if(x < min_x) min_x = x;
                    if(x > max_x) max_x = x;
                    if(y < min_y) min_y = y;
                    if(y > max_y) max_y = y;

                   p.setX(x);
                   p.setY(y);

                   if(n == 1)
                   {
                      A.push_back(p);
                   }
                   else
                   {
                       B.push_back(p);
                   }
                }
    myfile.close();

    }

    //scale points to canvas size
    double h = canvas_size.height() - 50;
    double w = canvas_size.width() - 50;

    double x_coef = w/(max_x-min_x);
    double y_coef = h/(max_y-min_y);

    //Size of bigger polygon

    for(unsigned int i = 0; i < A.size(); i++)
    {
        A[i].setX((A[i].x()-min_x)*x_coef);
        A[i].setY((A[i].y()-min_y)*y_coef);
    }
    for(unsigned int i = 0; i < B.size(); i++)
    {
        B[i].setX((B[i].x()-min_x)*x_coef);
        B[i].setY((B[i].y()-min_y)*y_coef);
    }
}
