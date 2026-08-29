#include "vehicle.h"

Vehicle::Vehicle()
    : id(""), brand(""), model(""), pricePerDay(0.0), isRented(false)
{
}

Vehicle::Vehicle(const QString &id, const QString &brand, const QString &model,
                 double price, bool rented)
    : id(id), brand(brand), model(model),
    pricePerDay(price), isRented(rented)
{
}

QString Vehicle::getId() const
{
    return id;
}

QString Vehicle::getBrand() const
{
    return brand;
}

QString Vehicle::getModel() const
{
    return model;
}

double Vehicle::getPricePerDay() const
{
    return pricePerDay;
}

bool Vehicle::getIsRented() const
{
    return isRented;
}

void Vehicle::setIsRented(bool rented)
{
    isRented = rented;
}

QString Vehicle::displayInfo() const
{
    return id + " | " + brand + " | " + model;
}