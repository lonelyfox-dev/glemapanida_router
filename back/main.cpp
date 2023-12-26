#include <QSqlDatabase>
#include<QSqlQuery>
#include<QJsonArray>
#include<QJsonDocument>
#include <QJsonObject>
#include <QSqlQuery>
#include <QProcess>

#include "topology.h"
#include "config.h"
#include "webserver.h"

int main(int argc, char *argv[])
{

    QProcess process;

       // Указание пути к интерпретатору Python
       process.setProgram("C:/Program Files (x86)/Microsoft Visual Studio/Shared/Python39_64/python.exe");

       // Передача скрипта на Python с параметрами
       QStringList arguments;
       arguments << "get_routes.py" << "c213" << "c239"<<"2023-12-4";
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
        QJsonObject resObj;
        resObj["from"]="Москва";//obj["search"].toObject()["from"].toObject()["title"].toString();
        resObj["to"]="Санкт-Петербург";//obj["search"].toObject()["to"].toObject()["title"].toString();
        resObj["arrival"]=obj["segments"][0]["arrival"].toString();
        resObj["departure"]=obj["segments"][0]["departure"].toString();
        resObj["transport"]=obj["segments"][0]["from"].toObject()["transport_type"].toString();
        QJsonDocument document(resObj);
        QJsonObject o=document.object();
        QString strJson(document.toJson(QJsonDocument::Compact));

    bool check=config::instance().readConfig();
    if(!check)
    {
        qDebug() << "Config read fail";
        return 1;
    }
    config::params par=config::instance().getParams();

//    Topology *tp=new Topology();
    Topology::instance();
    WebServer webServer((par.ServerHost).toStdString().c_str(), par.ServerPort);
    if (webServer.init() != 0)
        return -1;
    webServer.connectToDB();
    webServer.run();

    system("pause");
    return 0;
}
