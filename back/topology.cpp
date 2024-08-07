#include "topology.h"

Topology::Topology()
{
    config::params par=config::instance().getParams();
    dbCtrl = new dbcontroller(par.ip,par.port,par.dbname,par.user,par.password,par.schema);

    QSqlQuery query(QString("select name from %1.city;").arg(dbCtrl->getSchema()),(dbCtrl->getDB()));
    while (query.next()) {
        QString city;
        city=query.value(0).toString();
        cities.append(city);
    }
    generateRandomTopology();
}

void Topology::generateSingleSchedule(QString source, QString destination, int distance)
{

}

void Topology::generateRandomTopology()
{
    srand(QTime::currentTime().msec()); // Начальная инициализация генератора случайных чисел

    int numCities = cities.size();
    QList<QPair<QString,QString>> res;
    for (int i = 0; i < numCities; ++i) {
        QList<QString> allCities=cities;
        // Генерируем случайное количество связей у текущего города (от 1 до numCities - 1)
        int numConnections = rand() % 100+1;/*(numCities - 1) + 1;*/

        for (int conn = 0; conn < numConnections; ++conn) {
            int randomDest = rand() % allCities.size();
            if (cities[i] != allCities[randomDest]) {
                QString source = cities[i];
                QString destination = allCities[randomDest];
                //                    QString transport = transportTypes[qrand() % transportTypes.size()]; // Случайно выбираем вид транспорта

                //                    tickets.append(TravelTicket(source, destination, transport));
                int distance=rand() % (200000 - 50000 + 1) + 50000;//метры

                g[i].push_back(std::make_pair(distance,cities.indexOf(destination)));

                res.append(qMakePair(source,destination));
                scheduleTickets[qMakePair(i,cities.indexOf(destination))]["plane"]=sheduleBuilder::generatePlaneSchedule(source,destination,distance);
                scheduleTickets[qMakePair(i,cities.indexOf(destination))]["train"]=sheduleBuilder::generateTrainSchedule(source,destination,distance);
                scheduleTickets[qMakePair(i,cities.indexOf(destination))]["bus"]=sheduleBuilder::generateBusSchedule(source,destination,distance);
                std::vector<int> costs{scheduleTickets[qMakePair(i,cities.indexOf(destination))]["plane"][0]->getCost(),
                            scheduleTickets[qMakePair(i,cities.indexOf(destination))]["bus"][0]->getCost(),scheduleTickets[qMakePair(i,cities.indexOf(destination))]["train"][0]->getCost()};
                lowCosts[i].push_back(std::make_pair(costs,cities.indexOf(destination)));
                std::vector<qint64> trTimes{scheduleTickets[qMakePair(i,cities.indexOf(destination))]["plane"][0]->getTravelTime(),
                            scheduleTickets[qMakePair(i,cities.indexOf(destination))]["bus"][0]->getTravelTime(),scheduleTickets[qMakePair(i,cities.indexOf(destination))]["train"][0]->getTravelTime()};

                lowTravelTime[i].push_back(std::make_pair(trTimes,cities.indexOf(destination)));
                allCities.erase(allCities.begin()+randomDest);
            }
        }
    }
}

QJsonArray Topology::LowCostRoute(QString startVertex, QString endVertex, QStringList transports, QDateTime startTime)
{
    int u=cities.indexOf(startVertex);
    QVector<int> cost(cities.size(),INF);
    QVector<int> p(cities.size());
    QVector<quint64> tm(cities.size());
    cost[u] = 0;
    p[u] = u;
    tm[u]=QDateTime::currentDateTime().toMSecsSinceEpoch();
    std::priority_queue< std::pair<int, int>, std::vector< std::pair<int, int> >, std::greater< std::pair<int, int> > > q;
    q.push(std::make_pair(0, u));
    while (!q.empty()) {
        std::pair<int, int> u = q.top();
        q.pop();
        //если стоимость поездки между городом i и j больше,
        //чем уже найденная стоимость между этими городами, то пропускаем такую ситуацию
            if ( u.first > cost[u.second]) continue;
            for (int i = 0; i < (int) lowCosts[u.second].size(); i++) {
                int v = lowCosts[u.second][i].second;
                for(int j=0;j<lowCosts[u.second][i].first.size();j++)
                {
                int lCost= lowCosts[u.second][i].first[j];
                auto shedule=scheduleTickets[qMakePair(u.second,v)];
                Ticket *ticket=new Ticket();
                for(auto it=shedule.begin();it!=shedule.end();it++)
                {
                    if(!transports.contains(it.key())) continue;
                    for(int i=0;i<it.value().size();i++)
                    {
                        if(it.value()[i]->getArrivalTime().toMSecsSinceEpoch()>tm[u.second]&&it.value()[i]->getArrivalTime().toMSecsSinceEpoch()>startTime.toMSecsSinceEpoch())
                        {
                            ticket=it.value()[i];
                            break;
                        }
                    }
                }
                //нужно, чтобы билеты брались в правльной последовательности (прибытие нового по времени позже чем отправление предыдущего)
                if(ticket->getArrivalTime().toMSecsSinceEpoch()>tm[u.second])
                    if (cost[v] > cost[u.second] + lCost) {
                        p[v] = u.second;
                        cost[v] = cost[u.second] + lCost;
                        tm[v]=ticket->getDepartureTime().toMSecsSinceEpoch();
                        q.push(std::make_pair(cost[v], v));
                    }
                }
            }

    }
    //восстановление последовательности билетов
    int node=cities.indexOf(endVertex);
    QString start=cities[u];
    QString end=cities[node];
    QList<Ticket*> tick;
    QStringList citiesPath;
    qint64 prevDepartureTime=0;
    citiesPath.append(end);
    if(p[node]!=0)
        while(p[node] != u)
        {
            auto shedule=scheduleTickets[qMakePair(p[node],node)];
            Ticket *ticket=new Ticket();
            for(auto it=shedule.begin();it!=shedule.end();it++)
            {
                if(!transports.contains(it.key())) continue;
                for(int i=0;i<it.value().size();i++)
                {
                    if(it.value()[i]->getArrivalTime().toMSecsSinceEpoch()>prevDepartureTime&&it.value()[i]->getArrivalTime().toMSecsSinceEpoch()>startTime.toMSecsSinceEpoch())
                    {
                        ticket=it.value()[i];
                        prevDepartureTime=ticket->getDepartureTime().toMSecsSinceEpoch();
                        break;
                    }
                }
            }
            citiesPath.append(cities[p[node]]);
            tick.append(ticket);
            node=p[node];
            qDebug()<<node;
        }
    if(tick.size()!=0)
    {
        auto shedule=scheduleTickets[qMakePair(p[node],node)];
        Ticket *ticket=new Ticket();
        for(auto it=shedule.begin();it!=shedule.end();it++)
        {
            if(!transports.contains(it.key())) continue;
            for(int i=0;i<it.value().size();i++)
            {
                if(it.value()[i]->getArrivalTime().toMSecsSinceEpoch()>prevDepartureTime&&it.value()[i]->getArrivalTime().toMSecsSinceEpoch()>startTime.toMSecsSinceEpoch())
                {
                    ticket=it.value()[i];
                    prevDepartureTime=ticket->getDepartureTime().toMSecsSinceEpoch();
                    break;
                }
            }
        }
        tick.append(ticket);
        citiesPath.append(cities[p[node]]);
    }
    if(tick.size()==0 && p[node] == u)
    {
        auto shedule=scheduleTickets[qMakePair(p[node],node)];
        Ticket *ticket=new Ticket();
        for(auto it=shedule.begin();it!=shedule.end();it++)
        {
            if(!transports.contains(it.key())) continue;
            for(int i=0;i<it.value().size();i++)
            {
                if(it.value()[i]->getArrivalTime().toMSecsSinceEpoch()>startTime.toMSecsSinceEpoch())
                {
                    ticket=it.value()[i];
                    break;
                }
            }
        }
        tick.append(ticket);
        citiesPath.append(cities[node]);
        citiesPath.append(cities[p[node]]);
    }
    prevDepartureTime=0;
    tick.clear();
    for(int i=citiesPath.size()-2;i>=0;i--)
    {
        bool found=false;
        int s=cities.indexOf(citiesPath[i+1]);
        int e=cities.indexOf(citiesPath[i]);
        auto schedule=scheduleTickets[qMakePair(s,e)];
        Ticket* ticket=new Ticket();
        for(auto it=schedule.begin();it!=schedule.end();it++)
        {
            if(found) break;
            if(!transports.contains(it.key())) continue;
            for(int j=0;j<it.value().size();j++)
                if(it.value()[j]->getArrivalTime().toMSecsSinceEpoch()>prevDepartureTime&&it.value()[j]->getArrivalTime().toMSecsSinceEpoch()>startTime.toMSecsSinceEpoch())
                {
                    if(found) break;
                    ticket=it.value()[j];
                    prevDepartureTime=ticket->getDepartureTime().toMSecsSinceEpoch();
                    tick.append(ticket);
                    found=true;
                    break;
                }
        }
    }
    QJsonArray arr;
    for(int i=0;i<tick.size();i++)
    {
        arr.append(tick[i]->toJson());
    }
    return arr;
}

QJsonArray Topology::LowTravelTimeRoute(QString startVertex, QString endVertex, QStringList transports, QDateTime startTime)
{
    int u=cities.indexOf(startVertex);
    QVector<int> times(cities.size(),INF);
    QVector<int> p(cities.size());
    QVector<quint64> tm(cities.size());
    times[u] = 0;
    p[u] = u;
    tm[u]=QDateTime::currentDateTime().toMSecsSinceEpoch();
    std::priority_queue< std::pair<int, int>, std::vector< std::pair<int, int> >, std::greater< std::pair<int, int> > > q;
    q.push(std::make_pair(0, u));
    while (!q.empty()) {
        std::pair<int, int> u = q.top();
        q.pop();
        //если время поездки между городом i и j больше,
        //чем уже найденное время между этими городами, то пропускаем такую ситуацию
            if ( u.first > times[u.second]) continue;
            for (int i = 0; i < (int) lowTravelTime[u.second].size(); i++) {
                int v = lowTravelTime[u.second][i].second;
                for(int j=0;j<lowTravelTime[u.second][i].first.size();j++)
                {
                int lTime= lowTravelTime[u.second][i].first[j];
                auto shedule=scheduleTickets[qMakePair(u.second,v)];
                Ticket *ticket=new Ticket();
                for(auto it=shedule.begin();it!=shedule.end();it++)
                {
                    if(!transports.contains(it.key())) continue;
                    for(int i=0;i<it.value().size();i++)
                    {
                        if(it.value()[i]->getArrivalTime().toMSecsSinceEpoch()>tm[u.second]&&it.value()[i]->getArrivalTime().toMSecsSinceEpoch()>startTime.toMSecsSinceEpoch())
                        {
                            ticket=it.value()[i];
                            break;
                        }
                    }
                }
                if(ticket->getArrivalTime().toMSecsSinceEpoch()>tm[u.second])
                    if (times[v] > times[u.second] + lTime) {
                        p[v] = u.second;
                        times[v] = times[u.second] + lTime;
                        tm[v]=ticket->getDepartureTime().toMSecsSinceEpoch();
                        q.push(std::make_pair(times[v], v));
                    }
                }
            }

    }
    //восстановление последовательности билетов
    int node=cities.indexOf(endVertex);
    QString start=cities[u];
    QString end=cities[node];
    QList<Ticket*> tick;
    QStringList citiesPath;
    qint64 prevDepartureTime=0;
    citiesPath.append(end);
    if(p[node]!=0)
        while(p[node] != u)
        {
            auto shedule=scheduleTickets[qMakePair(p[node],node)];
            Ticket *ticket=new Ticket();
            for(auto it=shedule.begin();it!=shedule.end();it++)
            {
                if(!transports.contains(it.key())) continue;
                for(int i=0;i<it.value().size();i++)
                {
                    if(it.value()[i]->getArrivalTime().toMSecsSinceEpoch()>prevDepartureTime&&it.value()[i]->getArrivalTime().toMSecsSinceEpoch()>startTime.toMSecsSinceEpoch())
                    {
                        ticket=it.value()[i];
                        prevDepartureTime=ticket->getDepartureTime().toMSecsSinceEpoch();
                        break;
                    }
                }
            }
            citiesPath.append(cities[p[node]]);
            tick.append(ticket);
            node=p[node];
            qDebug()<<node;
        }
    if(tick.size()!=0)
    {
        auto shedule=scheduleTickets[qMakePair(p[node],node)];
        Ticket *ticket=new Ticket();
        for(auto it=shedule.begin();it!=shedule.end();it++)
        {
            if(!transports.contains(it.key())) continue;
            for(int i=0;i<it.value().size();i++)
            {
                if(it.value()[i]->getArrivalTime().toMSecsSinceEpoch()>prevDepartureTime&&it.value()[i]->getArrivalTime().toMSecsSinceEpoch()>startTime.toMSecsSinceEpoch())
                {
                    ticket=it.value()[i];
                    prevDepartureTime=ticket->getDepartureTime().toMSecsSinceEpoch();
                    break;
                }
            }
        }
        tick.append(ticket);
        citiesPath.append(cities[p[node]]);
    }
    if(tick.size()==0 && p[node] == u)
    {
        auto shedule=scheduleTickets[qMakePair(p[node],node)];
        Ticket *ticket=new Ticket();
        for(auto it=shedule.begin();it!=shedule.end();it++)
        {
            if(!transports.contains(it.key())) continue;
            for(int i=0;i<it.value().size();i++)
            {
                if(it.value()[i]->getArrivalTime().toMSecsSinceEpoch()>startTime.toMSecsSinceEpoch())
                {
                    ticket=it.value()[i];
                    break;
                }
            }
        }
        tick.append(ticket);
        citiesPath.append(cities[node]);
        citiesPath.append(cities[p[node]]);
    }
    prevDepartureTime=0;
    tick.clear();
    for(int i=citiesPath.size()-2;i>=0;i--)
    {
        bool found=false;
        int s=cities.indexOf(citiesPath[i+1]);
        int e=cities.indexOf(citiesPath[i]);
        auto schedule=scheduleTickets[qMakePair(s,e)];
        Ticket* ticket=new Ticket();
        for(auto it=schedule.begin();it!=schedule.end();it++)
        {
            if(found) break;
            if(!transports.contains(it.key())) continue;
            for(int j=0;j<it.value().size();j++)
                //нужно, чтобы билеты брались в правльной последовательности (прибытие нового по времени позже чем отправление предыдущего)

                if(it.value()[j]->getArrivalTime().toMSecsSinceEpoch()>prevDepartureTime&&it.value()[j]->getArrivalTime().toMSecsSinceEpoch()>startTime.toMSecsSinceEpoch())
                {
                    if(found) break;
                    ticket=it.value()[j];
                    prevDepartureTime=ticket->getDepartureTime().toMSecsSinceEpoch();
                    tick.append(ticket);
                    found=true;
                    break;
                }
        }
    }
    QJsonArray arr;
    //    for(int i=tick.size()-1;i>=0;i--)
    //    {
    //        arr.append(tick[i]->toJson());
    //    }
    for(int i=0;i<tick.size();i++)
    {
        arr.append(tick[i]->toJson());
    }
    return arr;
}
