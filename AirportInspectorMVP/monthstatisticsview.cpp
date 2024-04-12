#include "monthstatisticsview.h"
#include "ui_monthstatisticsview.h"

monthstatisticsview::monthstatisticsview(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::monthstatisticsview)
{
    ui->setupUi(this);
}

monthstatisticsview::~monthstatisticsview()
{
    delete ui;
}
