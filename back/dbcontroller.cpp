#include "dbcontroller.h"

dbcontroller::dbcontroller(QString HostName,qint32 port,QString dbName,QString UserName,QString Password,QString schema)
{
    this->HostName=HostName;
    this->port=port;
    this->dbName=dbName;
    this->UserName=UserName;
    this->Password=Password;
    this->schema=schema;
    db=QSqlDatabase::addDatabase("QPSQL");
    db.setDatabaseName(dbName);
    db.setHostName(HostName);
    db.setUserName(UserName);
    db.setPassword(Password);
    db.setPort(port);
    db.open();
}

QSqlDatabase dbcontroller::getDB()
{
    return db;
}

QString dbcontroller::getSchema()
{
    return schema;
}
