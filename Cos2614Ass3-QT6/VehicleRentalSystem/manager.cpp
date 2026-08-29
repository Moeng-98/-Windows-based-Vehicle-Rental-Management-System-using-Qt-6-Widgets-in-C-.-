#include "manager.h"
#include "vehicle.h"
#include "car.h"
#include "motorcycle.h"

#include <QTextStream>
#include <QFile>
#include <QStringList>

Manager::~Manager()
{
    saveToFile();

    for (Vehicle* v : vehicles)
        delete v;
}

void Manager::addVehicle(Vehicle* v)
{
    vehicles.append(v);
}

Vehicle* Manager::findVehicle(const QString &id)
{
    for (Vehicle* v : vehicles)
        if (v->getId().compare(id, Qt::CaseInsensitive) == 0)
            return v;

    return nullptr;
}

void Manager::loadFromFile()
{
    QFile file("vehicles.txt");

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        // fallback default vehicles
        vehicles.append(new Car("C1", "Toyota", "Corolla", 300, false, 4, 5));
        vehicles.append(new Car("C2", "BMW", "M3", 800, false, 4, 5));
        vehicles.append(new Motorcycle("M1", "Yamaha", "R1", 500, false, 1000));
        return;
    }

    QTextStream in(&file);

    while (!in.atEnd())
    {
        QString line = in.readLine();
        QStringList parts = line.split("|");

        if (parts.size() < 6)
            continue;

        QString type = parts[0];
        QString id = parts[1];
        QString brand = parts[2];
        QString model = parts[3];
        double price = parts[4].toDouble();
        bool rented = parts[5].toInt();

        if (type == "Car")
        {
            int doors = parts.size() > 6 ? parts[6].toInt() : 4;
            int seats = parts.size() > 7 ? parts[7].toInt() : 5;

            Car* c = new Car(id, brand, model, price, rented, doors, seats);

            if (parts.size() >= 10)
                c->setRentalInfo(parts[8], parts[9].toInt());

            vehicles.append(c);
        }

        else if (type == "Motorcycle")
        {
            int engine = parts.size() > 6 ? parts[6].toInt() : 500;

            Motorcycle* m = new Motorcycle(id, brand, model, price, rented, engine);

            if (parts.size() >= 9)
                m->setRentalInfo(parts[7], parts[8].toInt());

            vehicles.append(m);
        }
    }

    file.close();

    if (vehicles.isEmpty())
    {
        vehicles.append(new Car("C1", "Toyota", "Corolla", 300, false, 4, 5));
        vehicles.append(new Motorcycle("M1", "Yamaha", "R1", 500, false, 1000));
    }
}

void Manager::saveToFile()
{
    QFile file("vehicles.txt");

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&file);

    for (Vehicle* v : vehicles)
    {
        if (Car* c = dynamic_cast<Car*>(v))
        {
            out << "Car|"
                << c->getId() << "|"
                << c->getBrand() << "|"
                << c->getModel() << "|"
                << c->getPricePerDay() << "|"
                << c->getIsRented() << "|"
                << c->getDoors() << "|"
                << c->getSeats() << "|"
                << c->getCustomer() << "|"
                << c->getDays()
                << "\n";
        }
        else if (Motorcycle* m = dynamic_cast<Motorcycle*>(v))
        {
            out << "Motorcycle|"
                << m->getId() << "|"
                << m->getBrand() << "|"
                << m->getModel() << "|"
                << m->getPricePerDay() << "|"
                << m->getIsRented() << "|"
                << m->getEngine() << "|"
                << m->getCustomer() << "|"
                << m->getDays()
                << "\n";
        }
    }

    file.close();
}

QVector<Vehicle*>& Manager::getVehicles()
{
    return vehicles;
}