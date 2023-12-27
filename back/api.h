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
    //перечисление доступных ручек
    enum handlers{
        GET_CITY =0,
        GET_TICKET//1
    };

public:
    //структура для формирования ответа
    struct Header{
        int contentLength;
        int errorCode;
        std::string response;
        std::string status;
    } typedef Header;

    //ручка для получения списка городов по части названия
    static QString getCity(QString name,dbcontroller *dbCtrl);
    //ручка для получения маршрута с учетом переданных параметров
    static QString getRoutes(std::map<std::string, std::string> params);
    //обработка запроса пользователя
    static Header requestProcessing(std::string msg,dbcontroller *dbCtrl);
private:
    api(){};//статический класс
    //питон скрипт для получения списка билетов от яндекса
    static QString execPython(QString city1,QString city2,QString date);//временно (перенести в отдельный класс)
    //имя ручки к int
    static int handlerToInt(std::string handler);
    //проверка валидности переданных параметров
    static bool requestParamsCheck(int handler,std::map<std::string, std::string> params);

    //список доступных видов транспорта
    static QStringList availableTransports;
    //список доступных вариантов маршрута
    static QStringList routesVariants;

};

#endif // API_H
