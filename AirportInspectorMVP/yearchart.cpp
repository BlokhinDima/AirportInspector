#include "yearchart.h"

YearChart::YearChart(QObject* parent) : QObject{parent}
{
    barSeries = new QBarSeries(this);
    airportTraffic = new QBarSet("Загруженность аэропорта", this);

    axisY = new QValueAxis(this);
    axisY->setLabelFormat("%i");
    axisY->setTickType(QValueAxis::TicksDynamic);

    axisX = new QBarCategoryAxis(this);
    axisX->append(months);
    axisY->setTitleText("Общее количество прилетов и вылетов");   
}


void YearChart::addDataToBars(QVector<int>& values)
{
    for (int i = 0; i < values.size(); i++)
    {
        airportTraffic->insert(i, values[i]);
    }
    barSeries->append(airportTraffic);
    updateAxisYRange(values);
}


void YearChart::updateGrpah(QChart* chart)
{
    chart->addSeries(barSeries);
    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);
    barSeries->attachAxis(axisX);
    barSeries->attachAxis(axisY);
}


void YearChart::clearGraph(QChart *chart)
{
    if (chart->series().isEmpty() == false)
    {
        chart->removeSeries(barSeries);
        chart->removeAxis(axisX);
        chart->removeAxis(axisY);
    }
}


void YearChart::updateAxisYRange(QVector<int>& data)
{
    auto max = *std::max_element(data.constBegin(), data.constEnd());
    axisY->setRange(0, max);
    axisY->setTickInterval(max / 4);
}
