#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QtConcurrent>
#include <QTimer>
#include "database.h"
#include "statisticswindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void sl_ReceiveConnectionSatus(const bool status);
    void sl_ReceiveRequestStatus(const QSqlError err);
    void sl_DisplayAirportsList(const QSqlQueryModel* queryModel);
    void sl_DisplayFlightsData(QSqlQueryModel* queryModel);
    void sl_DisplayYearStatisticsData(const QSqlQueryModel* queryModel);
    void sl_DisplayMonthsStatisticsData(const QSqlQueryModel* queryModel);

private slots:
    void on_pb_getFlights_clicked();
    void on_pb_getFlightsStat_clicked();

private:
    Ui::MainWindow *ui;
    Database* dataBase;
    ConnectionData* connectionData;
    QMessageBox* msg;
    StatisticsWindow* statWindow;
    void connectToDatabase();
    void enableControlWidgets(const bool enableFlag);
    void runReconnectionTimer();
};
#endif // MAINWINDOW_H
