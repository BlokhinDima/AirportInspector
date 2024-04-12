#include "statisticswindow.h"
#include "ui_statisticswindow.h"

StatisticsWindow::StatisticsWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StatisticsWindow)
{
    ui->setupUi(this);
    ui->tw_statistics->tabBar()->setDocumentMode(true);
    ui->tw_statistics->tabBar()->setExpanding(true);

    yearChart = new YearChart();
    yearStatisticsChart = new QChart();
    yearStatisticsChartView = new QChartView(yearStatisticsChart);

    monthChart = new MonthChart();
    monthStatisticsChart = new QChart();
    monthStatisticsChartView = new QChartView(monthStatisticsChart);
    monthsStatWindow = new MonthsStatisticsWindow(this);
    monthsStatWindow->addChart(monthStatisticsChartView);

    ui->tw_statistics->insertTab(0, yearStatisticsChartView, "Статистика за год");
    ui->tw_statistics->insertTab(1, monthsStatWindow, "Статистика за месяц");
    yearStatisticsChartView->show();
    monthStatisticsChartView->show();

    connect(monthsStatWindow, &MonthsStatisticsWindow::sig_MonthChanged, this, &StatisticsWindow::sl_MonthChanged);
}


StatisticsWindow::~StatisticsWindow()
{ 
    delete ui;
    delete yearChart;
    delete yearStatisticsChart;
    delete yearStatisticsChartView;
    delete monthChart;
    delete monthStatisticsChart;
    delete monthStatisticsChartView;
}


void StatisticsWindow::setAirportName(const QString& airportName)
{
    ui->lb_airportName->setText(airportName);
}


void StatisticsWindow::on_pb_close_clicked()
{
    monthsStatWindow->resetMonthsComboBox();
    monthChart->reset();
    this->close();
}


void StatisticsWindow::displayYearStatisticsData(QVector<int>& values)
{
    yearChart->clearGraph(yearStatisticsChart);
    yearChart->addDataToBars(values);
    yearChart->updateGrpah(yearStatisticsChart);
}


void StatisticsWindow::displayMonthStatisticsData(QVector<QDate>& x, QVector<int>& y)
{
    auto monthName = monthsStatWindow->getMonthName();
    monthChart->clearGraph(monthStatisticsChart);
    monthChart->setDates(x);
    monthChart->setMonthsData(y);
    monthChart->updateGraph(monthStatisticsChart, monthName);
}


void StatisticsWindow::sl_MonthChanged(const QString& monthName)
{
    monthChart->clearGraph(monthStatisticsChart);
    monthChart->updateGraph(monthStatisticsChart, monthName);
}


void StatisticsWindow::closeEvent(QCloseEvent *event)
{
    on_pb_close_clicked();
    event->accept();
}
