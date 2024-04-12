#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QDate>
#include <QSqlError>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QSqlQueryModel>
#include <QSqlRecord>

#define POSTGRE_DRIVER "QPSQL"
#define DB_NAME "MyDB"


struct ConnectionData
{
    QString host = "981757-ca08998.tmweb.ru";
    QString databaseName = "demo";
    QString login = "netology_usr_cpp";
    QString pass = "CppNeto3";
    QString port = "5432";
};


class Database : public QObject
{
    Q_OBJECT

public:
    explicit Database(QObject* parent = nullptr);
    ~Database();

    void addDatabase(QString driver, QString databaseName = "");
    void connectToDatabase(const ConnectionData& connectionData);
    void disconnectFromDatabase(QString databaseName = "");
    QSqlError GetLastError(void);
    bool sendRequest(const QString& request);
    QString requestAirportCode(const QString& airportName);
    void requestAirportsList();
    void requestArrivalsData(const QString& airportName, const QDate& date);
    void requestDeparturesData(const QString& airportName, const QDate& date);
    void requestYearStatistics(const QString& airportName);
    void requestMonthsStatistics(const QString& airportName);


public slots:
    void sl_CreateTableModel(bool status);


signals:
    void sig_SendStatusConnection(bool status);
    void sig_SendStatusRequest(QSqlError err);
    void sig_SendAirportsListData(QSqlQueryModel* queryModel);
    void sig_SendFlightsData(QSqlQueryModel* queryModel);
    void sig_SendYearStatisticsData(QSqlQueryModel* queryModel);
    void sig_SendMonthsStatisticsData(QSqlQueryModel* queryModel);


private:
    QSqlDatabase* dataBase;
    QSqlTableModel* tableModel;
    QSqlQueryModel* queryModel;

};

#endif // DATABASE_H
