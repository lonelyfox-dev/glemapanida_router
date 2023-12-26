#ifndef API_H
#define API_H
#include<QJsonArray>
#include<QJsonDocument>
#include <QJsonObject>
#include <QSqlQuery>
#include <QProcess>
#include<QTextCodec>

#include "dbcontroller.h"
#include "requestparser.h"
#include "topology.h"

class api
{
    enum handlers{
        GET_CITY =0,
        GET_TICKET,
        GET_ROUTES
    };

public:

    struct Header{
        int contentLength;
        int errorCode;
        std::string response;
        std::string status;
    } typedef Header;

    static QString getCity(QString name,dbcontroller *dbCtrl);
    static QString getRoutes(std::map<std::string, std::string> params);

    static Header requestProcessing(std::string msg,dbcontroller *dbCtrl);
private:
    api(){};
    //Topology *tp=new Topology();
    static QString execPython(QString city1,QString city2,QString date);//временно (перенести в отдельный класс)
    static int handlerToInt(std::string handler);
    static bool requestParamsCheck(int handler,std::map<std::string, std::string> params);
    static QJsonObject getLowDurationTicket(QJsonObject obj,QStringList transports);
    static QString formOutputJson(QJsonObject obj);
    static QString test();
};

#endif // API_H
