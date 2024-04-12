#ifndef MONTHCHART_H
#define MONTHCHART_H

#include <QObject>
#include <QChart>
#include <QLineSeries>
#include <QValueAxis>


class MonthChart : public QObject
{
    Q_OBJECT
public:
    MonthChart(QObject* parent = nullptr);
    void updateGraph(QChart* chart, const QString& monthName);
    void clearGraph(QChart* chart);
    void setDates(QVector<QDate>& dates);
    void setMonthsData(QVector<int>& monthsData);
    void reset();

private:
    QLineSeries* graph;
    QValueAxis* axisX;
    QValueAxis* axisY;
    QVector<QDate> dates;
    QVector<int> monthsData;
    void updateAxisYRange(QVector<int> y);
    int getMonthIndex(const QString& monthName);
    void addDataToGraph(QVector<int> x, QVector<int> y);
};

#endif // MONTHCHART_H
