#include "widget.h"
#include "ui_widget.h"

#include <vector>
#include <fstream>
#include <QtGui>
#include <QMessageBox>


#include "edge.h"
#include "algorithms.h"
#include "triangle.h"

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
    std::vector<QPoint3D> points = ui->Canvas->getPoints();

    std::vector<Edge> dt = Algorithms::DT(points);
    ui->Canvas->setDT(dt);
    repaint();
}

void Widget::on_pushButton_3_clicked()
{
    //Clear and repaint
    ui->Canvas->clearDT();
    repaint();
}

void Widget::on_pushButton_2_clicked()
{
    //Create contour lines
    std::vector<Edge> dt = ui->Canvas->getDT();
    int dz = readNumber();
    if(dz < 1){
        QMessageBox msgBox;
        msgBox.setText("Warning: The input has to be integer!");
        msgBox.exec();
        return;
    }
    std::vector<Edge> contours = Algorithms::createContours(dt, z_min, z_max, dz);
    std::vector<Edge> mainContours = Algorithms::createContours(dt, z_min, z_max, 5*dz);

    ui->Canvas->setContours(contours);
    ui->Canvas->setMainContours(mainContours);
    repaint();
}

int Widget::readNumber(){
    try{
        return ui->dz->text().toInt();
    }catch(int e){
        return 0;
    }
}

void Widget::on_pushButton_4_clicked()
{
    bool slope = FALSE;
    bool aspect = FALSE;
    //Analyze slope and aspect
    std::vector<Edge> dt = ui->Canvas->getDT();
    std::vector<Triangle> dtm = Algorithms::analyzeDTM(dt);
    ui->Canvas->setDTM(dtm);

    if (ui->comboBox_2->currentIndex()==0)
    {
        slope = TRUE;
        aspect = FALSE;
    }
    else if (ui->comboBox_2->currentIndex()==1)
    {
        slope = FALSE;
        aspect = TRUE;
    }

    ui->Canvas->setAspect(aspect);
    ui->Canvas->setSlope(slope);

    repaint();

}

void Widget::on_pushButton_5_clicked()
{
       ui->Canvas->clearDT();

       std::vector<QPoint3D> points;
       //Generate
       if (ui->comboBox->currentIndex()==0)
           points = Algorithms::generateHill();
       else if (ui->comboBox->currentIndex()==1)
          points = Algorithms::generateValley();
       else if (ui->comboBox->currentIndex()==2)
          points = Algorithms::generateMountains();
       else if (ui->comboBox->currentIndex()==3)
          points = Algorithms::generateGrid(100);
       else if (ui->comboBox->currentIndex()==4)
           points = Algorithms::generateSaddle();
       else if (ui->comboBox->currentIndex()==5)
           points = Algorithms::generateCol();

       ui->Canvas->setPoints(points);

       ui->Canvas->repaint();
}


void Widget::on_pushButton_6_clicked()
{
    std::vector<QPoint3D> points;

    QSizeF canvas_size = ui->Canvas->size();

    QString path = QFileDialog::getOpenFileName(
                this,
                tr("Select file"),
                "/",
                "Text file (*.txt);;All files (*.*)");

    std::string path_utf8 = path.toUtf8().constData();

    QString msg;


   Draw::importPolygons(path_utf8, points,canvas_size, z_min, z_max);

   ui->Canvas->setPoints(points);

   ui->Canvas->repaint();
}



void Widget::on_Save_clicked()
{
    QString path = QFileDialog::getSaveFileName(
                this,
                tr("Select file"),
                "/",
                "Image file (*.png);;All files (*.*)");

    ui->Canvas->grab().save(path);

    QMessageBox msgBox;
    msgBox.setText("Canvas has been saved!");
    msgBox.exec();

}
