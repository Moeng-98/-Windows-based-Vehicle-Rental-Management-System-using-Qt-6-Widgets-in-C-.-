#include "car.h"

Car::Car(const QString &id, const QString &brand, const QString &model,
         double price, bool rented, int doors, int seats)
    : Vehicle(id, brand, model, price, rented),
    doors(doors), seats(seats)
{
}

QString Car::typeName() const
{
    return "Car";
}

QString Car::displayInfo() const
{
    return typeName() + " | " + Vehicle::displayInfo() +
           " | Doors: " + QString::number(doors) +
           " | Seats: " + QString::number(seats);
}

int Car::getDoors() const
{
    return doors;
}

int Car::getSeats() const
{
    return seats;
}