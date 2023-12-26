#include "ticket.h"

Ticket::Ticket(QString arrival, QString departure, int distance, int cost,QString transport)
{
    this->arrival=arrival;
    this->departure=departure;
    this->distance=distance;
    this->cost=cost;
    this->transport=transport;
}

void Ticket::generateRandomTimes()
{
    QDateTime currentDate = QDateTime::currentDateTime();
    int randomDays = QRandomGenerator::global()->bounded(0, 1); // Генерируем случайное число от 0 до 1

    arrivalTime = currentDate.addDays(randomDays);
    arrivalTime.setMSecsSinceEpoch(arrivalTime.toMSecsSinceEpoch() + static_cast<qint64>(QRandomGenerator::global()->bounded(0, 86400000)) + 3600000);

    randomDays = QRandomGenerator::global()->bounded(0, 1);
    departureTime = arrivalTime.addDays(randomDays);
    departureTime.setMSecsSinceEpoch(departureTime.toMSecsSinceEpoch() + static_cast<qint64>(QRandomGenerator::global()->bounded(0, 86400000)) + 7200000);

    travelTime = departureTime.toMSecsSinceEpoch() - arrivalTime.toMSecsSinceEpoch();
}

QString Ticket::getArrival()
{
    return arrival;
}

QString Ticket::getDeparture()
{
    return departure;
}

int Ticket::getDistance()
{
    return distance;
}

int Ticket::getCost()
{
    return cost;
}

QDateTime Ticket::getArrivalTime()
{
    return arrivalTime;
}

QDateTime Ticket::getDepartureTime()
{
    return departureTime;
}

qint64 Ticket::getTravelTime()
{
    return travelTime;
}

void Ticket::setArrivalTime(QDateTime arrivalTime)
{
    this->arrivalTime=arrivalTime;
}

void Ticket::setDepartureTime(QDateTime departureTime)
{
    this->departureTime=departureTime;
}

void Ticket::setTravelTime(qint64 travelTime)
{
    this->travelTime=travelTime;
}

QJsonObject Ticket::toJson()
{
    QJsonObject obj;
    obj["from"]=arrival;
    obj["to"]=departure;
    obj["arrival"]=arrivalTime.toString();
    obj["departure"]=departureTime.toString();
    obj["transport"]="train";
    obj["duration"]=travelTime/1000;
    obj["cost"]=cost;
    return obj;
}

