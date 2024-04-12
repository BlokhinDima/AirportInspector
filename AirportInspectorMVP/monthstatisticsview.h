#ifndef MONTHSTATISTICSVIEW_H
#define MONTHSTATISTICSVIEW_H

#include <QWidget>

namespace Ui {
class monthstatisticsview;
}

class monthstatisticsview : public QWidget
{
    Q_OBJECT

public:
    explicit monthstatisticsview(QWidget *parent = nullptr);
    ~monthstatisticsview();

private:
    Ui::monthstatisticsview *ui;
};

#endif // MONTHSTATISTICSVIEW_H
