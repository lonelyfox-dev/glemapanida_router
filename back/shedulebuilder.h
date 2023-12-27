#ifndef SHEDULEBUILDER_H
#define SHEDULEBUILDER_H
#include "ticket.h"

class sheduleBuilder
{
public:
    //построение расписания самолетов для двух городов
    static QList<Ticket*> generatePlaneSchedule(QString source,QString destination, int distance);
    //построение расписания автобусов для двух городов
    static QList<Ticket*> generateBusSchedule(QString source,QString destination, int distance);
    //построение расписания поездов для двух городов
    static QList<Ticket*> generateTrainSchedule(QString source,QString destination, int distance);
private:
    sheduleBuilder(){};
};

#endif // SHEDULEBUILDER_H
