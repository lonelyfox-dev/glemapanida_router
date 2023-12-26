#ifndef SHEDULEBUILDER_H
#define SHEDULEBUILDER_H
#include "ticket.h"

class sheduleBuilder
{
public:
    static QList<Ticket*> generatePlaneSchedule(QString source,QString destination, int distance);
    static QList<Ticket*> generateBusSchedule(QString source,QString destination, int distance);
    static QList<Ticket*> generateTrainSchedule(QString source,QString destination, int distance);
private:
    sheduleBuilder(){};
};

#endif // SHEDULEBUILDER_H
