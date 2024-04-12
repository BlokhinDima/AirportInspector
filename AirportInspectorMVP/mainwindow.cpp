#include "mainwindow.h"
#include "./ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->statusbar->showMessage("Отключено");
    ui->rb_arrivals->setChecked(true);
    enableControlWidgets(false);

    QDate minDate(2016, 8, 15);
    QDate maxDate(2017, 9, 14);
    ui->de_date->setMinimumDate(minDate);
    ui->de_date->setMaximumDate(maxDate);

    dataBase = new Database(this);
    connectionData = new ConnectionData();

    msg = new QMessageBox(this);
    msg->setWindowFlags(Qt::Dialog | Qt::WindowTitleHint | Qt::CustomizeWindowHint);

    statWindow = new StatisticsWindow(this);

    connect(dataBase, &Database::sig_SendStatusConnection, this, &MainWindow::sl_ReceiveConnectionSatus);
    connect(dataBase, &Database::sig_SendStatusRequest, this, &MainWindow::sl_ReceiveRequestStatus);
    connect(dataBase, &Database::sig_SendAirportsListData, this, &MainWindow::sl_DisplayAirportsList);
    connect(dataBase, &Database::sig_SendFlightsData, this, &MainWindow::sl_DisplayFlightsData);
    connect(dataBase, &Database::sig_SendYearStatisticsData, this, &MainWindow::sl_DisplayYearStatisticsData);
    connect(dataBase, &Database::sig_SendMonthsStatisticsData, this, &MainWindow::sl_DisplayMonthsStatisticsData);
    connect(msg, &QMessageBox::buttonClicked, this, &MainWindow::runReconnectionTimer);

    dataBase->addDatabase(POSTGRE_DRIVER, DB_NAME);
    connectToDatabase();
}


MainWindow::~MainWindow()
{
    delete ui;
    delete connectionData;
}


void MainWindow::enableControlWidgets(const bool enableFlag)
{
    ui->cb_airports->setEnabled(enableFlag);
    ui->pb_getFlights->setEnabled(enableFlag);
    ui->pb_getFlightsStat->setEnabled(enableFlag);
    ui->de_date->setEnabled(enableFlag);
    ui->rb_arrivals->setEnabled(enableFlag);
    ui->rb_departures->setEnabled(enableFlag);
}


void MainWindow::connectToDatabase()
{
    auto conn = [&]{dataBase->connectToDatabase(*connectionData);};
    QtConcurrent::run(conn);
}


void MainWindow::sl_ReceiveConnectionSatus(const bool status)
{
    if (status)
    {
        dataBase->requestAirportsList();
        ui->statusbar->showMessage("Подключено");
        enableControlWidgets(true);
    }
    else
    {
        dataBase->disconnectFromDatabase(DB_NAME);
        msg->setIcon(QMessageBox::Critical);
        msg->setText(dataBase->GetLastError().text());
        ui->statusbar->showMessage("Отключено");
        msg->exec();
    }
}


void MainWindow::sl_ReceiveRequestStatus(QSqlError err)
{
    if(err.type() != QSqlError::NoError)
    {
        msg->setText(err.text());
        msg->exec();
    }
}


void MainWindow::sl_DisplayAirportsList(const QSqlQueryModel* queryModel)
{
    auto rows_count = queryModel->rowCount();
    for (int i = 0; i < rows_count; ++i)
    {
        auto airportName = queryModel->record(i).value(0).toString();
        ui->cb_airports->addItem(airportName);
    }
}


void MainWindow::sl_DisplayFlightsData(QSqlQueryModel* queryModel)
{
    queryModel->setHeaderData(0, Qt::Horizontal, QObject::tr("Номер рейса"));
    queryModel->setHeaderData(1, Qt::Horizontal, QObject::tr("Время вылета"));

    if (ui->rb_arrivals->isChecked())
    {
        queryModel->setHeaderData(2, Qt::Horizontal, QObject::tr("Аэропорт отправления"));
    }
    else if (ui->rb_departures->isChecked())
    {
        queryModel->setHeaderData(2, Qt::Horizontal, QObject::tr("Аэропорт назначения"));
    }

    ui->tv_flights->setModel(queryModel);
    ui->tv_flights->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}


void MainWindow::sl_DisplayYearStatisticsData(const QSqlQueryModel* queryModel)
{
    QVector<int> values;
    auto rows_count = queryModel->rowCount();

    for (int i = 0; i < rows_count; ++i)
    {
        values.append(queryModel->record(i).value(0).toInt());
    }

    statWindow->displayYearStatisticsData(values);
}


void MainWindow::sl_DisplayMonthsStatisticsData(const QSqlQueryModel* queryModel)
{
    QVector<QDate> dates;
    QVector<int> values;
    auto rows_count = queryModel->rowCount();

    for (int i = 0; i < rows_count; ++i)
    {
       values.append(queryModel->record(i).value(0).toInt());
       auto date_str = queryModel->record(i).value(1).toString();
       auto dateTime = QDateTime::fromString(date_str, Qt::ISODateWithMs);
       dates.append(dateTime.date());
    }

    statWindow->displayMonthStatisticsData(dates, values);
}


void MainWindow::on_pb_getFlights_clicked()
{
    ui->tv_flights->setUpdatesEnabled(true);
    auto airportName = ui->cb_airports->currentText();
    auto date = ui->de_date->date();
    if (ui->rb_arrivals->isChecked())
    {
        dataBase->requestArrivalsData(airportName, date);
    }
    else if (ui->rb_departures->isChecked())
    {
        dataBase->requestDeparturesData(airportName, date);
    }
}


void MainWindow::on_pb_getFlightsStat_clicked()
{
    ui->tv_flights->setUpdatesEnabled(false);
    auto airportName = ui->cb_airports->currentText();
    statWindow->setAirportName(airportName);
    statWindow->setWindowModality(Qt::WindowModal);
    dataBase->requestYearStatistics(airportName);
    dataBase->requestMonthsStatistics(airportName);
    statWindow->show();
}


void MainWindow::runReconnectionTimer()
{
    QTimer::singleShot(5000, this, &MainWindow::connectToDatabase);
}
