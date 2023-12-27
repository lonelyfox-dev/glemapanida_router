#ifndef DBCONTROLLER_H
#define DBCONTROLLER_H
#include <QString>
#include <QSqlDatabase>
#include <QDebug>
#include <QSqlError>
class dbcontroller
{
public:
    dbcontroller(QString HostName,qint32 port,QString dbName,QString UserName,QString Password,QString schema);
    //получение экземпляра базы данных
    QSqlDatabase getDB();
    QString getSchema();
private:
    QString dbName;
    QString HostName;
    QString UserName;
    qint32 port;
    QString Password;
    QSqlDatabase db;
    QString schema;

};

#endif // DBCONTROLLER_H
