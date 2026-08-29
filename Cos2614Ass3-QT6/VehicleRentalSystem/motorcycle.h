#ifndef MOTORCYCLE_H
#define MOTORCYCLE_H

#include "vehicle.h"

class Motorcycle : public Vehicle
{
private:
    int engine;

public:
    Motorcycle(const QString &id, const QString &brand, const QString &model,
               double price, bool rented, int engine);

    QString typeName() const override;
    QString displayInfo() const override;

    int getEngine() const;
};

#endif