#ifndef STATISTICSWINDOW_H
#define STATISTICSWINDOW_H

#include <QDialog>
#include <QtCharts>
#include <QChartView>
#include "yearchart.h"
#include "monthchart.h"
#include "monthsstatisticswindow.h"


namespace Ui {
class StatisticsWindow;
}


class StatisticsWindow : public QDialog
{
    Q_OBJECT

public:
    explicit StatisticsWindow(QWidget *parent = nullptr);
    ~StatisticsWindow();
    void setAirportName(const QString& airportName);
    void displayYearStatisticsData(QVector<int>& values);
    void displayMonthStatisticsData(QVector<QDate>& x, QVector<int>& y);

private slots:
    void on_pb_close_clicked();
    void sl_MonthChanged(const QString& monthName);

private:
    Ui::StatisticsWindow *ui;
    QChart* yearStatisticsChart;
    QChartView* yearStatisticsChartView;
    YearChart* yearChart;
    QChart* monthStatisticsChart;
    QChartView* monthStatisticsChartView;
    MonthChart* monthChart;
    MonthsStatisticsWindow* monthsStatWindow;
    void closeEvent(QCloseEvent *event);
};

#endif // STATISTICSWINDOW_H
