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
    //чтение .ini файла
    bool readConfig();
    //обращение к объекту для вызова функций и изменения состояний
    static config &instance() {
        static config l_config;
        return l_config;
    }
    //удаление конструктора-копирования
    config( const config& ) = delete;
    //структура, хранящая в себе все параметры .ini файла
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
    //проверка присутствия ключа в файле
    static bool checkKey(QSettings *settings, QString key);
};


#endif // CONFIG_H
