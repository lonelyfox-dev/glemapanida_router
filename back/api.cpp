#include "api.h"

QStringList api::availableTransports={"plane","train","bus"};
QStringList api::routesVariants={"money","travelTime"};

//ручка получения вариантов городов по промежуточному имени
QString api::getCity(QString name, dbcontroller *dbCtrl)
{
    if(name=="") return "[]";
    QSqlQuery query(QString("select name from %1.city where lower(name) like lower('%2%') limit 5;").arg(dbCtrl->getSchema(),name),(dbCtrl->getDB()));
    QList<QMap<QString,QString>>cities;
    while (query.next()) {
        QMap<QString,QString> city;
        city["name"]=query.value(0).toString();
        cities.append(city);
    }
    QJsonArray arr;
    for(int i=0;i<cities.size();i++)
    {
        QJsonObject obj;
        obj[cities[i].keys()[0]]=cities[i].value(cities[i].keys()[0]);
        arr.append(obj);
    }
    QJsonDocument doc;
    doc.setArray(arr);

    QString dataToString=QString::fromUtf8(doc.toJson(QJsonDocument::Compact));
    return dataToString;
}
//ручка получения маршрута с заданными параметрами в запросе
QString api::getRoutes(std::map<std::string, std::string> params)
{
    QList<QPair<QString,QString>> cityPairs;
    QStringList cities;
    QStringList transports;
    QString route;
    for(auto it=params.begin();it!=params.end();it++)
    {
        if (it->first.find("city") != std::string::npos)
        {
            cities.append(QString::fromStdString(it->second));
        }
        else
        {
            if(availableTransports.contains(QString::fromStdString(it->first)) && it->second.find("true")!=std::string::npos)
            {
                transports.append(QString::fromStdString(it->first));
            }
            if(routesVariants.contains(QString::fromStdString(it->first)) && it->second.find("true")!=std::string::npos)
            {
               route=QString::fromStdString(it->first);
            }
        }
    }
    if(transports.size()==0)
    {
        transports={"plane","bus","train"};
    }
    for(int i=1;i<cities.size();i++)
    {
        cityPairs.append(qMakePair(cities[i-1],cities[i]));
    }
    QJsonArray arr;
    QDateTime actualTime=QDateTime::currentDateTime();
    if(route=="money")
        foreach(auto item,cityPairs)
        {
            QJsonArray ticketArray=Topology::instance().LowCostRoute(item.first,item.second,transports,actualTime);
            if(ticketArray.size()==0) return "[]";
            actualTime=QDateTime::fromString(ticketArray[ticketArray.size()-1].toObject()["departure"].toString());
            foreach(auto ticket,ticketArray)
            {
                arr.append(ticket);
            }
        }
    if(route=="travelTime")
        foreach(auto item,cityPairs)
        {
            QJsonArray ticketArray=Topology::instance().LowTravelTimeRoute(item.first,item.second,transports,actualTime);
            if(ticketArray.size()==0) return "[]";
            actualTime=QDateTime::fromString(ticketArray[ticketArray.size()-1].toObject()["departure"].toString());
            foreach(auto ticket,ticketArray)
            {
                arr.append(ticket);
            }
        }
    QJsonDocument doc;
    doc.setArray(arr);

    QString dataToString=QString::fromUtf8(doc.toJson(QJsonDocument::Compact));
    return dataToString;
}
//обработка пользовательского запроса
api::Header api::requestProcessing(std::string msg, dbcontroller *dbCtrl)
{
    //получение ручки
    auto handler=requestparser::getHandler(msg);
    //получение параметров запроса
    auto params=requestparser::getParams(msg);
    //имя ручки к числу (switch/case принимает только int)
    int Handler=handlerToInt(handler);
    Header header;
    //проверка правильности переданных параметров
    bool res=requestParamsCheck(Handler,params);
    if(!res)
    {
        header.errorCode=400;
        header.response="{\"error\":\"Invalid options\"}";
        header.contentLength= header.response.length();
        header.status="Invalid options";
        return header;
    }
    //обработка конкретного запроса
    switch (Handler)
    {
    case GET_CITY:
    {
        auto response=getCity(QString::fromStdString(params["search"]),dbCtrl);
        header.errorCode=200;
        header.response=response.toStdString();
        QByteArray byteArray = QByteArray::fromPercentEncoding(QString::fromStdString(header.response).toUtf8());
        header.contentLength=byteArray.size();
        header.status="OK";
        return header;
    }
    case GET_TICKET:
    {
        header.errorCode=200;
        header.response=getRoutes(params).toStdString();//test().toStdString();
        QByteArray byteArray = QByteArray::fromPercentEncoding(QString::fromStdString(header.response).toUtf8());
        header.contentLength=byteArray.size();
        header.status="OK";
        return header;
    }
    default:
    {
        header.errorCode=404;
        header.response="{\"error\":\"Not found\"}";
        header.contentLength= header.response.length();
        header.status="Not found";
        break;
    }
    }
}
//запуск скрипта для получения списка билетов от яндекса
QString api::execPython(QString city1,QString city2,QString date)
{
    QProcess process;

       // Указание пути к интерпретатору Python
       process.setProgram("C:/Program Files (x86)/Microsoft Visual Studio/Shared/Python39_64/python.exe");

       // Передача скрипта на Python с параметрами
       QStringList arguments;
       arguments << "get_routes.py" <<city1 << city2<<date;//"2023-12-6"
       process.setArguments(arguments);

       // Запуск процесса
       process.start();
       process.waitForFinished();

       // Получение результатов выполнения скрипта
       QByteArray output = process.readAllStandardOutput();
       QByteArray errorOutput = process.readAllStandardError();
        QString DataAsString = QTextCodec::codecForMib(106)->toUnicode(output);
        QJsonDocument doc = QJsonDocument::fromJson(DataAsString.toUtf8());
        auto obj = doc.object();
        QJsonDocument document(obj);
        QString strJson(document.toJson(QJsonDocument::Compact));
        return strJson;

}

int api::handlerToInt(std::string handler)
{
    if(handler=="getCity")
    {
        return GET_CITY;
    }
    if(handler=="getTicket")
    {
        return GET_TICKET;
    }
    return -1;
}

bool api::requestParamsCheck(int handler, std::map<std::string, std::string> params)
{
    switch (handler)
    {
    case GET_CITY:
    {
        if(params.find("search")!=params.end())
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    }
    return true;
}
