#include "motorcycle.h"

Motorcycle::Motorcycle(const QString &id, const QString &brand, const QString &model,
                       double price, bool rented, int engine)
    : Vehicle(id, brand, model, price, rented), engine(engine)
{
}

QString Motorcycle::typeName() const
{
    return "Motorcycle";
}

QString Motorcycle::displayInfo() const
{
    return QString("Engine: %1cc").arg(engine);
}

int Motorcycle::getEngine() const
{
    return engine;
}