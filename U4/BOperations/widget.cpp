#include "widget.h"
#include "ui_widget.h"

#include <QFileDialog>
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
    ui->Canvas->setAB();
}

void Widget::on_pushButton_4_clicked()
{
    ui->Canvas->clearAll();

    repaint();
}

void Widget::on_pushButton_2_clicked()
{
    //Perform boolean operation

    TBooleanOperation operation;
    switch(ui->comboBox->currentIndex())
    {
        case 0: operation = INTERSECTION; break;
        case 1: operation = UNION; break;
        case 2: operation = DIFFAB; break;
        case 3: operation = DIFFBA;
    }

    // Get those polygons
    std::vector<QPointFB> A = ui->Canvas->getA();
    std::vector<QPointFB> B = ui->Canvas->getB();

    std::vector<std::vector<QPointFB>> result = Algorithms::BooleanOper(A, B, operation);

    ui->Canvas->setRes(result);

    repaint();
}

void Widget::on_pushButton_3_clicked()
{
    ui->Canvas->clearResults();

    repaint();
}

void Widget::on_pushButton_5_clicked()
{
    // Create buffer over polygons
    std::vector<QPointFB> A = ui->Canvas->getA();
    std::vector<QPointFB> B = ui->Canvas->getB();

    std::vector<std::vector<QPointFB> > bufferA = Algorithms::polygonOffset(A, 20,  30.0 * M_PI / 180);
    //std::vector<std::vector<QPointFB> > bufferB = Algorithms::polygonOffset(B, 20,  7.0 * M_PI / 180);

    //bufferB.insert(bufferB.end(), bufferA.begin(), bufferA.end());

    ui->Canvas->setBuff(bufferA);

    repaint();

}

void Widget::on_pushButton_6_clicked()
{
    std::vector<QPointFB> A, B;

    QSizeF canvas_size = ui->Canvas->size();

    QString path = QFileDialog::getOpenFileName(
                this,
                tr("Select file"),
                "/",
                "Text file (*.txt);;All files (*.*)");

    std::string path_utf8 = path.toUtf8().constData();

    QString msg;


   Draw::importPolygons(path_utf8, A, B, canvas_size);

   ui->Canvas->setA(A);

   ui->Canvas->setB(B);

   ui->Canvas->repaint();
}
