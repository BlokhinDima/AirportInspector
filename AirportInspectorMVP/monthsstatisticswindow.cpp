#include "monthsstatisticswindow.h"
#include "ui_monthsstatisticswindow.h"

MonthsStatisticsWindow::MonthsStatisticsWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MonthsStatisticsWindow)
{
    ui->setupUi(this);
    ui->cb_months->addItems(months);
}


MonthsStatisticsWindow::~MonthsStatisticsWindow()
{
    delete ui;
}


void MonthsStatisticsWindow::addChart(QChartView* chartView)
{
    ui->lo_monthsStatisticsWindow->addWidget(chartView);
}


QString MonthsStatisticsWindow::getMonthName()
{
    return ui->cb_months->currentText();
}


void MonthsStatisticsWindow::on_cb_months_currentTextChanged(const QString &arg1)
{
    emit sig_MonthChanged(arg1);
}


void MonthsStatisticsWindow::resetMonthsComboBox()
{
    ui->cb_months->setCurrentIndex(0);
}
