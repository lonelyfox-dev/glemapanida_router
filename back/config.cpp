#include "config.h"
#include <QDir>
config::config()
{

}

config::params config::getParams()
{
    return par;
}
bool config::checkKey(QSettings *settings, QString key) {
    if (!settings->contains(key)) { qDebug() << key << "does not exist"; return false; }
    return true;
}
bool config::readConfig()
{
    QString appPath = QDir::currentPath()+"/SERVER2.ini";
    QFileInfo check_file(appPath);

    if (!check_file.exists()) {
        qDebug() << ": file not exist - " << appPath;
        return 0;
    }
    std::unique_ptr<QSettings> settings = std::unique_ptr<QSettings>
            (new QSettings(appPath,QSettings::IniFormat));
    if (!checkKey(settings.get(), "ServerHost")) return false;
    par.ServerHost=settings->value("ServerHost").toString();

    if (!checkKey(settings.get(), "ServerPort")) return false;
    par.ServerPort=settings->value("ServerPort").toInt();

    if (!checkKey(settings.get(), "ip")) return false;
    par.ip=settings->value("ip").toString();

    if (!checkKey(settings.get(), "port")) return false;
    par.port=settings->value("port").toInt();

    if (!checkKey(settings.get(), "dbname")) return false;
    par.dbname=settings->value("dbname").toString();

    if (!checkKey(settings.get(), "schema")) return false;
    par.schema=settings->value("schema").toString();

    if (!checkKey(settings.get(), "user")) return false;
    par.user=settings->value("user").toString();

    if (!checkKey(settings.get(), "password")) return false;
    par.password=settings->value("password").toString();

    qDebug() << "Config read successfully";
    return true;
}
