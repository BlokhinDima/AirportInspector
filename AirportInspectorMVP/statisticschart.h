#ifndef STATISTICSCHART_H
#define STATISTICSCHART_H

#include <QDialog>

namespace Ui {
class StatisticsChart;
}

class StatisticsChart : public QDialog
{
    Q_OBJECT

public:
    explicit StatisticsChart(QWidget *parent = nullptr);
    ~StatisticsChart();

private:
    Ui::StatisticsChart *ui;
};

#endif // STATISTICSCHART_H
