#ifndef CONFIG_H
#define CONFIG_H
#include <QCoreApplication>
#include <QObject>
#include <QSettings>
#include <QFileInfo>
#include <QDebug>

class config
{
public:
    bool readConfig();
    static config &instance() {
        static config l_config;
        return l_config;
    }
    config( const config& ) = delete;
    struct params{
        QString ServerHost;
        int ServerPort;

        QString ip;
        int port;
        QString dbname;
        QString schema;
        QString user;
        QString password;
    };
    params par;
    params getParams();
private:
    config();
    static bool checkKey(QSettings *settings, QString key);
};


#endif // CONFIG_H
