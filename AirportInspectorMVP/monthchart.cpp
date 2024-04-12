#include "monthchart.h"
#include <QDate>

MonthChart::MonthChart(QObject* parent) : QObject{parent}
{
    graph = new QLineSeries(this);
    graph->setName("Загруженность аэропорта");

    axisX = new QValueAxis(this);
    axisX->setTitleText("Число");
    axisX->setLabelFormat("%i");
    axisX->setTickInterval(1);
    axisX->setTickType(QValueAxis::TicksDynamic);

    axisY = new QValueAxis(this);
    axisY->setLabelFormat("%i");
    axisY->setTickInterval(1);
    axisY->setTickType(QValueAxis::TicksDynamic);
    axisY->setTitleText("Общее количество прилетов и вылетов");
}


void MonthChart::setDates(QVector<QDate>& dates)
{
    this->dates = dates;
}


void MonthChart::setMonthsData(QVector<int>& monthsData)
{
    this->monthsData = monthsData;
}


void MonthChart::addDataToGraph(QVector<int> x, QVector<int> y)
{
    uint32_t size = 0;

    if(x.size() >= y.size())
    {
        size = y.size();

    }
    else
    {
        size = x.size();
    }

    for(int i = 0; i < size; i++)
    {
        graph->append(x[i], y[i]);
    }

    updateAxisYRange(y);
}


void MonthChart::updateAxisYRange(QVector<int> y)
{
    auto max = *std::max_element(y.constBegin(), y.constEnd());
    auto min = *std::min_element(y.constBegin(), y.constEnd());
    if (max == min)
    {
        max++;
        min--;
    }
    axisY->setRange(min, max);
}


void MonthChart::clearGraph(QChart *chart)
{
    if (chart->series().isEmpty() == false)
    {
        graph->clear();
        chart->removeSeries(graph);
        chart->removeAxis(axisY);
        chart->removeAxis(axisX);
    }
}


void MonthChart::updateGraph(QChart *chart, const QString& monthName)
{
    auto monthIndex = getMonthIndex(monthName);
    QVector<int> days;
    QVector<int> values;

    for(int i = 0; i < dates.size(); i++)
    {
        if (monthIndex == dates[i].month())
        {
            days.append(dates[i].day());
            values.append(monthsData[i]);
        }
    }

    addDataToGraph(days, values);
    chart->addSeries(graph);
    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);
    graph->attachAxis(axisX);
    graph->attachAxis(axisY);
}


int MonthChart::getMonthIndex(const QString& monthName)
{
    QMap<QString, int> months
        {
            { "Январь", 1 },
            { "Февраль", 2 },
            { "Март", 3 },
            { "Апрель", 4 },
            { "Май", 5 },
            { "Июнь", 6 },
            { "Июль", 7 },
            { "Август", 8 },
            { "Сентябрь", 9 },
            { "Октябрь", 10 },
            { "Ноябрь", 11 },
            { "Декабрь", 12 }
        };

    const auto iter = months.find(monthName);

    if(iter != months.cend())
        return iter.value();
    return -1;
}


void MonthChart::reset()
{
    dates.clear();
    monthsData.clear();
}
