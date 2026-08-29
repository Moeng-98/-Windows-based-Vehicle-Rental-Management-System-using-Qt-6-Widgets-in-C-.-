#ifndef VEHICLE_H
#define VEHICLE_H

#include <QString>

class Vehicle
{
protected:
    QString id;
    QString brand;
    QString model;
    double pricePerDay;
    bool isRented;

    QString rentedBy;
    int rentedDays = 0;

public:
    Vehicle();
    Vehicle(const QString &id, const QString &brand, const QString &model,
            double price, bool rented);

    virtual ~Vehicle() = default;

    QString getId() const;
    QString getBrand() const;
    QString getModel() const;
    double getPricePerDay() const;

    bool getIsRented() const;
    void setIsRented(bool rented);

    void setRentalInfo(const QString& customer, int days)
    {
        rentedBy = customer;
        rentedDays = days;
    }

    QString getCustomer() const { return rentedBy; }
    int getDays() const { return rentedDays; }

    virtual QString typeName() const = 0;
    virtual QString displayInfo() const;
};

#endif