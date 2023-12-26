#include "api.h"

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

QString api::getRoutes(std::map<std::string, std::string> params)
{
    QList<QPair<QString,QString>> cityPairs;
    QStringList cities;
    QStringList transports;
    for(auto it=params.begin();it!=params.end();it++)
    {
        if (it->first.find("city") != std::string::npos)
        {
            cities.append(QString::fromStdString(it->second));
        }
        else
        {
            if(it->second.find("true")!=std::string::npos)
            {
                transports.append(QString::fromStdString(it->first));
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
    foreach(auto item,cityPairs)
    {
        QJsonArray ticketArray=Topology::instance().dijkstra(item.first,item.second,transports,actualTime);
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

api::Header api::requestProcessing(std::string msg, dbcontroller *dbCtrl)
{
    auto handler=requestparser::getHandler(msg);
    auto params=requestparser::getParams(msg);
    int Handler=handlerToInt(handler);
    Header header;
    bool res=requestParamsCheck(Handler,params);
    if(!res)
    {
        header.errorCode=400;
        header.response="{\"error\":\"Invalid options\"}";
        header.contentLength= header.response.length();
        header.status="Invalid options";
        return header;
    }
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
//        header.errorCode=200;
//        header.response=execPython().toStdString();
//        QByteArray byteArray = QByteArray::fromPercentEncoding(QString::fromStdString(header.response).toUtf8());
//        header.contentLength=byteArray.size();
//        header.status="OK";
//        return header;
    }
    case GET_ROUTES:
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
    if(handler == "getRoutes")
    {
        return GET_ROUTES;
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

QJsonObject api::getLowDurationTicket(QJsonObject obj, QStringList transports)
{
    QJsonArray segments=obj["segments"].toArray();
    QJsonObject minDurationItem;
    int minDuration=1000000000;
    foreach(auto item,segments)
    {
        QJsonObject value=item.toObject();
        QString transport=value["from"].toObject()["transport_type"].toString();
        if(value["duration"].toInt()<minDuration &&
                transports.contains(transport))
        {
            minDuration=value["duration"].toInt();
            minDurationItem=value;
        }
    }
    return minDurationItem;
}

QString api::formOutputJson(QJsonObject obj)
{
    QString fromData=obj["from"].toObject()["title"].toString();
    QByteArray byteArray = QByteArray::fromStdString(fromData.toStdString());
        QString from = QString::fromUtf8(byteArray);
    QJsonObject resObj;
    //qDebug() << "Restored string: " << restoredString;
    resObj["from"]=  from;//QString::fromUtf8(obj["from"].toObject()["title"].toString().toStdString().c_str());
    resObj["to"]=obj["to"].toObject()["title"].toString();
    resObj["arrival"]=obj["arrival"].toString();
    resObj["departure"]=obj["departure"].toString();
    resObj["transport"]=obj["from"].toObject()["transport_type"].toString();
    resObj["duration"]=obj["duration"].toInt();//cost
    QJsonDocument document(resObj);
    QString strJson(document.toJson(QJsonDocument::Compact));
    return strJson;
}

QString api::test()
{
    QMap<QString,QString> city={{"Москва","c213"},
                                            {"Калининград","c22"},
                                            {"Санкт-Петербург","c2"},
                                            {"Сочи","c239"},
                                             {"Владимир","c192"}};

    QList<QPair<QString,QString>> pair={{"Москва","Санкт-Петербург"},{"Санкт-Петербург","Калининград"},
                                       {"Калининград","Сочи"},{"Сочи","Москва"}};
    QJsonArray arr;
    foreach(auto item,pair)
    {
        auto data=execPython(city[item.first],city[item.second],"2023-12-6");
        QJsonDocument doc = QJsonDocument::fromJson(data.toUtf8());
        auto obj = doc.object();
        auto value=getLowDurationTicket(obj,{"train","plane"});
        auto js=formOutputJson(value);
        QJsonDocument js_doc = QJsonDocument::fromJson(js.toUtf8());
        arr.append(js_doc.object());
    }
    QJsonDocument jsonDocument(arr);
    QString strJson = jsonDocument.toJson(QJsonDocument::Compact);
    return strJson;
}
