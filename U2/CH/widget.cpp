#include "widget.h"
#include "ui_widget.h"
#include "algorithms.h"
#include <QMessageBox>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_clicked()
{
    QPolygon ch;

    std::vector<QPoint> points = ui->Canvas->getPoints();

    //Start time
    clock_t s = std::clock();

    //Construct CH
    if (ui->comboBox->currentIndex()==0)
        ch = Algorithms::CHJarvis(points);
    else if (ui->comboBox->currentIndex()==1)
        ch = Algorithms::QHull(points);
    else if (ui->comboBox->currentIndex()==2)
        ch = Algorithms::CHSweep(points);
    else if (ui->comboBox->currentIndex()==3)
        ch = Algorithms::GrahamScanNew(points);

    //End time
    clock_t e = std::clock();
    clock_t time = e-s;
    ui->time->setText(QString::number(time) + " ms");


    ui->Canvas->setCH(ch);
    repaint();
}

void Widget::on_generate_clicked()
{
    bool b = 0;

    ui->Canvas->clearCanvas();

    std::vector<QPoint> points;
    //Generate
    if (ui->comboBox_2->currentIndex()==0)
        points = Draw::generateCircle(ui->n_points->text().toInt(&b));
    else if (ui->comboBox_2->currentIndex()==1)
        points = Draw::generateSquare(ui->n_points->text().toInt(&b));
    else if (ui->comboBox_2->currentIndex()==2)
        points = Draw::generateEllipse(ui->n_points->text().toInt(&b));
    else if (ui->comboBox_2->currentIndex()==3)
        points = Draw::generateStarShape(ui->n_points->text().toInt(&b));
    else if (ui->comboBox_2->currentIndex()==4)
        points = Draw::generateRandomPoints(ui->n_points->text().toInt(&b));
    else if (ui->comboBox_2->currentIndex()==5)
        points = Draw::generateGrid(ui->n_points->text().toInt(&b));

    ui->Canvas->setPoints(points);
    ui->Canvas->repaint();
}


void Widget::on_pushButton_2_clicked()
{
    ui->Canvas->clearCanvas();
}

void Widget::on_minimumAreaEnclosingBox_clicked()
{
    QLine direction;
    QPolygon rectangle;
    QPolygon ch = ui->Canvas->getConvexHull();
    Algorithms::minimumAreaEnclosingBox(ch, rectangle, direction);
    ui->Canvas->setRectangle(rectangle);
    ui->Canvas->setDirection(direction);
    repaint();
}

void Widget::on_pushButton_3_clicked()
{
    int result = QMessageBox::warning(this, tr("Warning!"),
                                   tr("Are you sure you want to close this application?"),
                                   QMessageBox::Ok | QMessageBox::Cancel);

    if(result == QMessageBox::Ok){
        close();
    }

}


void Widget::on_pushButton_4_clicked()
{

    QMessageBox::information( this, "Information",
        "At first you need to create points. You can do it by yourself or generate it by inserting "
        "the number of points at the application. Application is capable of creating different shapes,"
        " such as circle, ellipse, square, etc. \n \n"
        "After creating points you should choose one of method for creating convex hull. The default "
        "setting is Jarvis Scan." );
}
