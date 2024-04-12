#include "statisticschart.h"
#include "ui_statisticschart.h"

StatisticsChart::StatisticsChart(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StatisticsChart)
{
    ui->setupUi(this);
}

StatisticsChart::~StatisticsChart()
{
    delete ui;
}
