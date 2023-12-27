#ifndef TOPOLOGY_H
#define TOPOLOGY_H

#include <QObject>
#include <QSqlQuery>
#include <QTime>
#include <QtGlobal>
#include <QVector>
#include <QJsonArray>
#include <queue>

#include "dbcontroller.h"
#include "config.h"
#include "ticket.h"
#include "shedulebuilder.h"

class Topology : public QObject
{
    Q_OBJECT
public:
    static Topology &instance() {
        static Topology l_Topology;
        return l_Topology;
    }
    //создание сулучайной топологии и билетов
    void generateRandomTopology();

    QJsonArray LowCostRoute(QString startVertex,QString endVertex,QStringList transports,QDateTime startTime);
    QJsonArray LowTravelTimeRoute(QString startVertex,QString endVertex,QStringList transports,QDateTime startTime);

private:
    Topology();

    dbcontroller *dbCtrl;
    QStringList cities;
    const int INF = 1e+9;
    std::map<int,std::vector< std::pair<int, int> >> g;
    std::map<int,std::vector< std::pair<std::vector<int>, int> >> lowCosts;//город к соседям + стоимости билетов
    std::map<int,std::vector< std::pair<std::vector<qint64>, int> >> lowTravelTime;//город к соседям + время в пути билетов
    QMap<QPair<int,int>,QMap<QString,QList<Ticket*>>> scheduleTickets;

    void generateSingleSchedule(QString source,QString destination, int distance);

};

#endif // TOPOLOGY_H
