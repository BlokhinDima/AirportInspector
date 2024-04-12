#ifndef YEARCHART_H
#define YEARCHART_H

#include <QObject>
#include <QChart>
#include <QBarSeries>
#include <QStringList>
#include <QBarSet>
#include <QBarCategoryAxis>
#include <QValueAxis>

class YearChart : public QObject
{
    Q_OBJECT
public:
    explicit YearChart(QObject* parent = nullptr);
    void addDataToBars(QVector<int>& values);
    void updateGrpah(QChart* chart);
    void clearGraph(QChart* chart);

private:
    QBarSeries* barSeries;
    QStringList months {
        "Сентябрь", "Октябрь", "Ноябрь", "Декабрь", "Январь",
        "Февраль", "Март", "Апрель", "Май", "Июнь", "Июль", "Август",
    };
    QBarSet* airportTraffic;
    QBarCategoryAxis* axisX;
    QValueAxis* axisY;
    void updateAxisYRange(QVector<int>& data);
};

#endif // YEARCHART_H
