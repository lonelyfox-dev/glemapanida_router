#ifndef TICKET_H
#define TICKET_H

#include <QObject>
#include <QDateTime>
#include <QRandomGenerator>
#include <QJsonObject>

class Ticket
{
public:
    Ticket(QString arrival,QString departure,int distance,int cost,QString transport);
    Ticket(){};
    void generateRandomTimes();

    QString getArrival();
    QString getDeparture();
    int getDistance();
    int getCost();
    QDateTime getArrivalTime();
    QDateTime getDepartureTime();
    qint64 getTravelTime();

    void setArrivalTime(QDateTime arrivalTime);
    void setDepartureTime(QDateTime departureTime);
    void setTravelTime(qint64 travelTime);


    QJsonObject toJson();
private:
    QString arrival;
    QString departure;
    int distance;
    int cost;
    qint64 travelTime;
    QDateTime arrivalTime;
    QDateTime departureTime;
    QString transport="";
//    QRandomGenerator generator;
};

#endif // TICKET_H
