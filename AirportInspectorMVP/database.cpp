#include "database.h"

Database::Database(QObject* parent) : QObject{parent}
{
    dataBase = new QSqlDatabase();
    queryModel = new QSqlQueryModel(this);
    connect(this, &Database::sig_SendStatusConnection, this, &Database::sl_CreateTableModel);
}


Database::~Database()
{
    delete dataBase;
}


void Database::addDatabase(QString driver, QString databaseName)
{
    *dataBase = QSqlDatabase::addDatabase(driver, databaseName);
}


void Database::connectToDatabase(const ConnectionData& connectionData)
{
    dataBase->setHostName(connectionData.host);
    dataBase->setDatabaseName(connectionData.databaseName);
    dataBase->setUserName(connectionData.login);
    dataBase->setPassword(connectionData.pass);
    dataBase->setPort(connectionData.port.toInt());

    bool status = dataBase->open();
    emit sig_SendStatusConnection(status);
}


void Database::disconnectFromDatabase(QString databaseName)
{
    *dataBase = QSqlDatabase::database(databaseName);
    dataBase->close();
}


void Database::sl_CreateTableModel(bool status)
{
    if (status)
        tableModel = new QSqlTableModel(this, *dataBase);
}


QSqlError Database::GetLastError()
{
    return dataBase->lastError();
}


bool Database::sendRequest(const QString& request)
{
    QSqlError err;
    queryModel->setQuery(request, *dataBase);

    if (queryModel->lastError().isValid())
    {
        err = queryModel->lastError();
        emit sig_SendStatusRequest(err);
        return false;
    }
    return true;
}


void Database::requestAirportsList()
{
    QString request = "SELECT airport_name->>'ru' as \"airportName\", airport_code FROM bookings.airports_data";
    if (sendRequest(request))
        emit sig_SendAirportsListData(queryModel);
}


void Database::requestArrivalsData(const QString& airportName, const QDate& date)
{
    auto next_day_str = date.addDays(1).toString(Qt::ISODate);
    auto date_str = date.toString(Qt::ISODate);
    auto airportCode = requestAirportCode(airportName);

    QString request = "SELECT flight_no, scheduled_arrival, ad.airport_name->>'ru' as \"Name\" from bookings.flights f "
            "JOIN bookings.airports_data ad on ad.airport_code = f.departure_airport "
            "WHERE (f.scheduled_arrival >= '" + date_str +"' AND f.scheduled_arrival < '" + next_day_str +"') "
            "AND f.arrival_airport = '" + airportCode + "'";

    if (sendRequest(request))
        emit sig_SendFlightsData(queryModel);
}


void Database::requestDeparturesData(const QString& airportName, const QDate& date)
{
    auto next_day_str = date.addDays(1).toString(Qt::ISODate);
    auto date_str = date.toString(Qt::ISODate);
    auto airportCode = requestAirportCode(airportName);

    QString request = "SELECT flight_no, scheduled_departure, ad.airport_name->>'ru' as \"Name\" from bookings.flights f "
            "JOIN bookings.airports_data ad on ad.airport_code = f.arrival_airport "
            "WHERE (f.scheduled_arrival >= '" + date_str +"' AND f.scheduled_arrival < '" + next_day_str +"') "
            "AND f.departure_airport = '" + airportCode + "'";

    if (sendRequest(request))
        emit sig_SendFlightsData(queryModel);
}


QString Database::requestAirportCode(const QString& airportName)
{
    QString request = "SELECT airport_code FROM bookings.airports_data "
            "WHERE bookings.airports_data.airport_name->>'ru' LIKE '" + airportName +"'";

    if (sendRequest(request))
        return queryModel->record(0).value(0).toString();
    return "";
}


void Database::requestYearStatistics(const QString& airportName)
{
    auto airportCode = requestAirportCode(airportName);

    QString request = "SELECT count(flight_no), date_trunc('month', scheduled_departure) as \"Month\" from bookings.flights f "
            "WHERE (scheduled_departure::date > date('2016-08-31') AND "
            "scheduled_departure::date <= date('2017-08-31')) AND "
            "(departure_airport = '" + airportCode + "' OR arrival_airport = '" + airportCode + "') "
            "GROUP BY \"Month\"";

    if (sendRequest(request))
        emit sig_SendYearStatisticsData(queryModel);
}


void Database::requestMonthsStatistics(const QString& airportName)
{
    auto airportCode = requestAirportCode(airportName);

    QString request = "SELECT count(flight_no), date_trunc('day', scheduled_departure) as \"Day\" from bookings.flights f "
            "WHERE (scheduled_departure::date > date('2016-08-31') AND "
            "scheduled_departure::date <= date('2017-08-31')) AND "
            "(departure_airport = '" + airportCode + "' OR arrival_airport = '" + airportCode + "') "
            "GROUP BY \"Day\" "
            "ORDER BY  \"Day\" ASC";

    if (sendRequest(request))
        emit sig_SendMonthsStatisticsData(queryModel);
}
