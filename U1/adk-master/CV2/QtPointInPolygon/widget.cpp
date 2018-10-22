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

void Widget::on_pushButton_3_clicked()
{
    ui->Canvas->clearCanvas();
}

void Widget::on_pushButton_clicked()
{
    ui->Canvas->setDrawPoint();
}

void Widget::on_pushButton_2_clicked()
{

    //get point
    QPointF q = ui->Canvas->getPoint();

    //get polygons one by one
    int polygon_count = ui->Canvas->getNumberOfPolygons();

    std::vector<int> res;

    //Compute result(in, out, boundary, vertex) by point and polygon
    for(int i = 0; i < polygon_count; i++)
    {
        QPolygonF pol = ui->Canvas->getPolygon(i);
        int result = ui->comboBox->currentIndex() == 0? Algorithms::getPositionWinding(q, pol) : Algorithms::getPositionRay(q, pol);
        res.push_back(result);
        ui->Canvas->setResult(res);
        qDebug() << "Result " << i << " is " << result;

    }
    ui->Canvas->repaint();

}

void Widget::on_importPolygons_clicked()
{
    QString path = QFileDialog::getOpenFileName(
                this,
                tr("Select file"),
                "/",
                "Text file (*.txt);;All files (*.*)");

    std::string path_utf8 = path.toUtf8().constData();

    QString msg;


    ui->Canvas->importPolygons(path_utf8);

}

void Widget::on_generatePolygon_clicked()
{
    ui->Canvas->generatePolygon(5,500); // ui n_points, n_polygons, coordinates_max
}
