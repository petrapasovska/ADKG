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
    QPointF q = ui->Canvas->getPoint();
    std::vector<QPointF> pol = ui->Canvas->getPolygon();
    int res = ui->comboBox->currentIndex() == 0? Algorithms::getPositionWinding(q, pol) : Algorithms::getPositionRay(q, pol);

    if (res == 1)
    {
        ui->label_2->setText("inside");
        ui->Canvas->fillPolygon(res);
    }
    else if (res == 2)
    {
        ui->label_2->setText("on boundary");
        ui->Canvas->fillPolygon(res);
    }
    else if (res == 3)
    {
        ui->label_2->setText("on the vertex");
        ui->Canvas->fillPolygon(res);
    }
    else
    {
        ui->label_2->setText("outside");
        ui->Canvas->fillPolygon(res);
    }
    //    ui->

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
    ui->Canvas->generatePolygon(10,500); // ui n_points, n_polygons, coordinates_max
}
