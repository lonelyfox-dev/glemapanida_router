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
    void generateRandomTopology();
    QJsonArray dijkstra(QString startVertex,QString endVertex,QStringList transports,QDateTime startTime);
private:
    Topology();

    dbcontroller *dbCtrl;
    QStringList cities;
    const int INF = 1e+9;
    std::map<int,std::vector< std::pair<int, int> >> g;
    QMap<QPair<int,int>,QMap<QString,QList<Ticket*>>> scheduleTickets;

    void generateSingleSchedule(QString source,QString destination, int distance);

};

#endif // TOPOLOGY_H
