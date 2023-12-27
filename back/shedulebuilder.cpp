#include "shedulebuilder.h"
 //построение расписания самолетов для двух городов
QList<Ticket *> sheduleBuilder::generatePlaneSchedule(QString source, QString destination, int distance)
{
    int Days = 2;
    QList<Ticket *> schedule;
    qint64 timeInterval=QRandomGenerator::global()->bounded(7200, 10800);
    qint64 startTime=QRandomGenerator::global()->bounded(0, 7200);
    qint64 endTime=24*60*60;
    qint64 travelTime=distance>100000?QRandomGenerator::global()->bounded(7200, 10800):QRandomGenerator::global()->bounded(3600, 7200);
    int cost=distance>100000?QRandomGenerator::global()->bounded(5000, 7500):QRandomGenerator::global()->bounded(2500, 5000);
    QDate currentDate = QDate::currentDate();
    for(int day=0;day<Days;day++)
    {
        for(int tm=startTime;tm<endTime && tm+travelTime<endTime;tm+=timeInterval)
        {
            Ticket *ticket=new Ticket(source,destination,distance,cost,"plane");
            QDateTime arrival=QDateTime(currentDate, QTime(0, 0));
            arrival=arrival.addDays(day);
            arrival.setMSecsSinceEpoch(arrival.toMSecsSinceEpoch()+tm*1000);
            QDateTime departure=arrival;
            departure.setMSecsSinceEpoch(departure.toMSecsSinceEpoch()+travelTime*1000);
            ticket->setArrivalTime(arrival);
            ticket->setDepartureTime(departure);
            ticket->setTravelTime(travelTime);
            schedule.append(ticket);
        }
    }
    return schedule;
}
 //построение расписания автобусов для двух городов
QList<Ticket *> sheduleBuilder::generateBusSchedule(QString source, QString destination, int distance)
{
    int Days = 2;
    QList<Ticket *> schedule;
    qint64 timeInterval=QRandomGenerator::global()->bounded(3600, 7200);
    qint64 startTime=QRandomGenerator::global()->bounded(0, 7200);
    qint64 endTime=24*60*60;
    qint64 travelTime=distance>100000?QRandomGenerator::global()->bounded(7200, 10800):QRandomGenerator::global()->bounded(1800, 7200);
    int cost=distance>100000?QRandomGenerator::global()->bounded(1000, 2500):QRandomGenerator::global()->bounded(150, 400);
    QDate currentDate = QDate::currentDate();
    for(int day=0;day<Days;day++)
    {
        for(int tm=startTime;tm<endTime && tm+travelTime<endTime;tm+=timeInterval)
        {
            Ticket *ticket=new Ticket(source,destination,distance,cost,"bus");
            QDateTime arrival=QDateTime(currentDate, QTime(0, 0));
            arrival=arrival.addDays(day);
            arrival.setMSecsSinceEpoch(arrival.toMSecsSinceEpoch()+tm*1000);
            QDateTime departure=arrival;
            departure.setMSecsSinceEpoch(departure.toMSecsSinceEpoch()+travelTime*1000);
            ticket->setArrivalTime(arrival);
            ticket->setDepartureTime(departure);
            ticket->setTravelTime(travelTime);
            schedule.append(ticket);
        }
    }
    return schedule;
}
 //построение расписания поездов для двух городов
QList<Ticket *> sheduleBuilder::generateTrainSchedule(QString source, QString destination, int distance)
{
    int Days = 2;
    QList<Ticket *> schedule;
    qint64 timeInterval=QRandomGenerator::global()->bounded(1800, 3600);
    qint64 startTime=QRandomGenerator::global()->bounded(0, 7200);
    qint64 endTime=24*60*60;
    qint64 travelTime=distance>100000?QRandomGenerator::global()->bounded(7200, 10800):QRandomGenerator::global()->bounded(3600, 7200);
    int cost=distance>100000?QRandomGenerator::global()->bounded(1000, 1500):QRandomGenerator::global()->bounded(300, 1000);
    QDate currentDate = QDate::currentDate();
    for(int day=0;day<Days;day++)
    {
        for(int tm=startTime;tm<endTime && tm+travelTime<endTime;tm+=timeInterval)
        {
            Ticket *ticket=new Ticket(source,destination,distance,cost,"train");
            QDateTime arrival=QDateTime(currentDate, QTime(0, 0));
            arrival=arrival.addDays(day);
            arrival.setMSecsSinceEpoch(arrival.toMSecsSinceEpoch()+tm*1000);
            QDateTime departure=arrival;
            departure.setMSecsSinceEpoch(departure.toMSecsSinceEpoch()+travelTime*1000);
            ticket->setArrivalTime(arrival);
            ticket->setDepartureTime(departure);
            ticket->setTravelTime(travelTime);
            schedule.append(ticket);
        }
    }
    return schedule;
}
