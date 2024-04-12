#ifndef MONTHSSTATISTICSWINDOW_H
#define MONTHSSTATISTICSWINDOW_H

#include <QWidget>
#include <QChartView>
#include <QSizePolicy>

namespace Ui {
class MonthsStatisticsWindow;
}

class MonthsStatisticsWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MonthsStatisticsWindow(QWidget *parent = nullptr);
    ~MonthsStatisticsWindow();
    void addChart(QChartView* chartView);
    QString getMonthName();
    void resetMonthsComboBox();

private slots:
    void on_cb_months_currentTextChanged(const QString &arg1);

signals:
    void sig_MonthChanged(const QString& monthName);

private:
    Ui::MonthsStatisticsWindow *ui;
    QStringList months {
        "Январь", "Февраль", "Март", "Апрель", "Май",
        "Июнь", "Июль", "Август", "Сентябрь", "Октябрь",
        "Ноябрь", "Декабрь",
    };
};

#endif // MONTHSSTATISTICSWINDOW_H
