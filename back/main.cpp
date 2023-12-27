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
    //чтение конфига (.ini)
    bool check=config::instance().readConfig();
    if(!check)
    {
        qDebug() << "Config read fail";
        return 1;
    }
    config::params par=config::instance().getParams();
    //генерация топологии и билетов
    Topology::instance();
    //server init
    WebServer webServer((par.ServerHost).toStdString().c_str(), par.ServerPort);
    if (webServer.init() != 0)
        return -1;
    //подключение к базе данных
    webServer.connectToDB();
    webServer.run();

    system("pause");
    return 0;
}
