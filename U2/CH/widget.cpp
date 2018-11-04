#include "widget.h"
#include "ui_widget.h"
#include "algorithms.h"

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

    //End time
    clock_t e = std::clock();
    clock_t time = float (e - s) / CLOCKS_PER_SEC;

    qDebug()<<time;
    //Write time
    ui->time->setText(QString::number(time));


    ui->Canvas->setCH(ch);
    repaint();
}

void Widget::on_generate_clicked()
{
    bool b = 0;

    std::vector<QPoint> points;
    //Generate
    if (ui->comboBox_2->currentIndex()==0)
        points = Draw::generateCircle(ui->n_points->text().toInt(&b));
    else if (ui->comboBox_2->currentIndex()==1)
        points = Draw::generateSquare(ui->n_points->text().toInt(&b));
    else if (ui->comboBox_2->currentIndex()==2)
        points = Draw::generateEclipse(ui->n_points->text().toInt(&b));
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
    ui->Canvas->setCH(rectangle);
    repaint();
}
