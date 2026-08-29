#ifndef MANAGER_H
#define MANAGER_H

#include <QVector>
#include <QString>
#include "vehicle.h"

class Manager
{
private:
    QVector<Vehicle*> vehicles;

public:
    ~Manager();

    void addVehicle(Vehicle* v);
    Vehicle* findVehicle(const QString &id);

    void displayAll();
    void displayAvailable();

    void searchVehicle(const QString &id);

    void rentVehicle(const QString &id);
    void returnVehicle(const QString &id);

    void loadFromFile();
    void saveToFile();

    QVector<Vehicle*>& getVehicles();
};

#endif