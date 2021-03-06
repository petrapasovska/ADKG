#ifndef WIDGET_H
#define WIDGET_H
#include <QObject>
#include <QWidget>
#include <QFileDialog>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_pushButton_3_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_importPolygons_clicked();

    void on_generatePolygon_clicked();

private:
    Ui::Widget *ui;
};

#endif // WIDGET_H
