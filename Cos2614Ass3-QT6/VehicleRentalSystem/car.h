#ifndef CAR_H
#define CAR_H

#include "vehicle.h"

class Car : public Vehicle
{
private:
    int doors;
    int seats;

public:
    Car(const QString &id, const QString &brand, const QString &model,
        double price, bool rented, int doors, int seats);

    QString typeName() const override;
    QString displayInfo() const override;

    int getDoors() const;
    int getSeats() const;
};

#endif