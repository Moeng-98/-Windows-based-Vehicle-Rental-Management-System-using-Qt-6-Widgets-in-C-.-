#include "mainwindow.h"
#include "utils.h"
#include "car.h"
#include "motorcycle.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QTableWidgetItem>
#include <QMessageBox>
#include <QHeaderView>

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    totalRevenue = 0;

    setupUI();
    manager.loadFromFile();
    loadTable();
    loadHistory();
}
void MainWindow::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    idInput = new QLineEdit;
    brandInput = new QLineEdit;
    modelInput = new QLineEdit;
    priceInput = new QLineEdit;

    idInput->setReadOnly(true);
    brandInput->setReadOnly(true);
    modelInput->setReadOnly(true);
    priceInput->setReadOnly(true);

    mainLayout->addWidget(new QLabel("Customer Name"));
    customerInput = new QLineEdit;
    mainLayout->addWidget(customerInput);

    mainLayout->addWidget(new QLabel("Number of Days"));
    daysInput = new QLineEdit;
    mainLayout->addWidget(daysInput);

    typeCombo = new QComboBox;
    typeCombo->addItems({"Car", "Motorcycle"});

    addBtn = new QPushButton("Add Vehicle");
    rentBtn = new QPushButton("Rent Vehicle");
    returnBtn = new QPushButton("Return Vehicle");
    removeBtn = new QPushButton("Remove Vehicle");
    refreshBtn = new QPushButton("Refresh");
    historyBtn = new QPushButton("View History");

    selectedLabel = new QLabel("No vehicle selected");
    selectedLabel->setStyleSheet("padding: 8px; border: 1px solid gray;");
    selectedLabel->setWordWrap(true);

    table = new QTableWidget;
    table->setColumnCount(6);
    table->setHorizontalHeaderLabels({"ID","Brand","Model","Price","Type","Status"});
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    historyTable = new QTableWidget;
    historyTable->setColumnCount(5);
    historyTable->setHorizontalHeaderLabels({"Vehicle ID", "Customer", "Days", "Total Price", "Status"});
    historyTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    historyTable->hide();

    historyLabel = new QLabel("Rental History");
    historyLabel->hide();

    mainLayout->addWidget(new QLabel("ID"));
    mainLayout->addWidget(idInput);
    mainLayout->addWidget(new QLabel("Brand"));
    mainLayout->addWidget(brandInput);
    mainLayout->addWidget(new QLabel("Model"));
    mainLayout->addWidget(modelInput);
    mainLayout->addWidget(new QLabel("Price"));
    mainLayout->addWidget(priceInput);
    mainLayout->addWidget(new QLabel("Type"));
    mainLayout->addWidget(typeCombo);

    mainLayout->addWidget(addBtn);
    mainLayout->addWidget(selectedLabel);
    mainLayout->addWidget(table);

    mainLayout->addWidget(historyBtn);
    mainLayout->addWidget(historyLabel);
    mainLayout->addWidget(historyTable);

    QHBoxLayout *btnLayout = new QHBoxLayout;
    btnLayout->addWidget(rentBtn);
    btnLayout->addWidget(returnBtn);
    btnLayout->addWidget(removeBtn);
    btnLayout->addWidget(refreshBtn);

    mainLayout->addLayout(btnLayout);

    connect(addBtn, &QPushButton::clicked, this, &MainWindow::addVehicle);
    connect(rentBtn, &QPushButton::clicked, this, &MainWindow::rentVehicle);
    connect(returnBtn, &QPushButton::clicked, this, &MainWindow::returnVehicle);
    connect(removeBtn, &QPushButton::clicked, this, &MainWindow::removeVehicle);
    connect(refreshBtn, &QPushButton::clicked, this, &MainWindow::loadTable);

    connect(historyBtn, &QPushButton::clicked, this, [=]() {
        bool visible = historyTable->isVisible();
        historyTable->setVisible(!visible);
        historyLabel->setVisible(!visible);
        historyBtn->setText(visible ? "View History" : "Hide History");
    });

    connect(table, &QTableWidget::itemSelectionChanged, this, [=]() {
        int row = table->currentRow();
        onTableRowSelected(row, 0);
    });
}
void MainWindow::loadTable()
{
    table->setRowCount(0);

    int row = 0;

    for (Vehicle* v : manager.getVehicles())
    {
        table->insertRow(row);

        table->setItem(row, 0, new QTableWidgetItem(v->getId()));
        table->setItem(row, 1, new QTableWidgetItem(v->getBrand()));
        table->setItem(row, 2, new QTableWidgetItem(v->getModel()));
        table->setItem(row, 3, new QTableWidgetItem(QString("R%1").arg(v->getPricePerDay())));
        table->setItem(row, 4, new QTableWidgetItem(v->typeName()));

        QTableWidgetItem *statusItem = new QTableWidgetItem(
            v->getIsRented() ? "Rented" : "Available"
            );

        if (v->getIsRented()) {
            statusItem->setBackground(Qt::red);
            statusItem->setForeground(Qt::white);
        } else {
            statusItem->setBackground(Qt::darkGreen);
            statusItem->setForeground(Qt::white);
        }

        statusItem->setTextAlignment(Qt::AlignCenter);
        table->setItem(row, 5, statusItem);

        row++;
    }

    if (table->rowCount() > 0)
    {
        table->selectRow(0);
        onTableRowSelected(0, 0);
    }

    loadHistory();
}
void MainWindow::loadHistory()
{
    historyTable->setRowCount(0);

    int row = 0;

    for (Vehicle* v : manager.getVehicles())
    {
        if (v->getCustomer().isEmpty())
            continue;

        historyTable->insertRow(row);

        historyTable->setItem(row, 0, new QTableWidgetItem(v->getId()));
        historyTable->setItem(row, 1, new QTableWidgetItem(v->getCustomer()));
        historyTable->setItem(row, 2, new QTableWidgetItem(QString::number(v->getDays())));

        double total = v->getPricePerDay() * v->getDays();
        historyTable->setItem(row, 3, new QTableWidgetItem("R" + QString::number(total)));

        QTableWidgetItem *statusItem = new QTableWidgetItem(
            v->getIsRented() ? "Active" : "Completed"
            );

        if (v->getIsRented()) {
            statusItem->setBackground(Qt::red);
            statusItem->setForeground(Qt::white);
        } else {
            statusItem->setBackground(Qt::darkGreen);
            statusItem->setForeground(Qt::white);
        }

        statusItem->setTextAlignment(Qt::AlignCenter);
        historyTable->setItem(row, 4, statusItem);

        row++;
    }
}
void MainWindow::rentVehicle()
{
    int row = table->currentRow();

    if (row < 0)
    {
        QMessageBox::warning(this, "Error", "Please select a vehicle");
        return;
    }

    QString customer = customerInput->text().trimmed();
    int days = daysInput->text().toInt();

    if (customer.isEmpty() || days <= 0)
    {
        QMessageBox::warning(this, "Invalid", "Please Enter your name, initials and number of days you will be renting the vehicle");
        return;
    }

    QString id = table->item(row, 0)->text();
    Vehicle* v = manager.findVehicle(id);

    if (!v) return;

    if (v->getIsRented())
    {
        QMessageBox::warning(this, "Error", "Vehicle already rented");
        return;
    }

    double total = v->getPricePerDay() * days;

    auto reply = QMessageBox::question(
        this,
        "Confirm Rental",
        "Customer: " + customer +
            "\nDays: " + QString::number(days) +
            "\nTotal: R" + QString::number(total)
        );

    if (reply != QMessageBox::Yes)
        return;

    v->setIsRented(true);
    v->setRentalInfo(customer, days);

    totalRevenue += total;

    QMessageBox::information(this, "Success",
                             "Rental completed!\nTotal Revenue: R" + QString::number(totalRevenue));

    manager.saveToFile();
    loadTable();

    customerInput->clear();
    daysInput->clear();
}

void MainWindow::addVehicle()
{
    QString id = idInput->text().trimmed();
    QString brand = brandInput->text().trimmed();
    QString model = modelInput->text().trimmed();
    double price = priceInput->text().toDouble();

    if (id.isEmpty() || brand.isEmpty() || model.isEmpty() || price <= 0) return;
    if (manager.findVehicle(id)) return;

    if (typeCombo->currentText() == "Car")
        manager.addVehicle(new Car(id, brand, model, price, false, 4, 5));
    else
        manager.addVehicle(new Motorcycle(id, brand, model, price, false, 600));

    manager.saveToFile();
    loadTable();
}

void MainWindow::returnVehicle()
{
    int row = table->currentRow();
    if (row < 0) return;

    QString id = table->item(row, 0)->text();
    Vehicle* v = manager.findVehicle(id);

    if (!v || !v->getIsRented()) return;

    v->setIsRented(false);
    v->setRentalInfo("", 0);

    manager.saveToFile();
    loadTable();
}

void MainWindow::removeVehicle()
{
    int row = table->currentRow();
    if (row < 0) return;

    QString id = table->item(row, 0)->text();
    QVector<Vehicle*>& list = manager.getVehicles();

    for (int i = 0; i < list.size(); ++i)
    {
        if (list[i]->getId() == id)
        {
            delete list[i];
            list.remove(i);
            break;
        }
    }

    manager.saveToFile();
    loadTable();
}

void MainWindow::onTableRowSelected(int row, int)
{
    if (row < 0 || !table->item(row, 0))
    {
        rentBtn->setEnabled(false);
        return;
    }

    QString id = table->item(row, 0)->text();
    QString brand = table->item(row, 1)->text();
    QString model = table->item(row, 2)->text();
    QString price = table->item(row, 3)->text();

    Vehicle* v = manager.findVehicle(id);

    idInput->setText(id);
    brandInput->setText(brand);
    modelInput->setText(model);
    priceInput->setText(price);

    rentBtn->setEnabled(!(v && v->getIsRented()));

    selectedLabel->setText(
        "Selected Vehicle:\n"
        "ID: " + id + "\n" +
        "Brand: " + brand + "\n" +
        "Model: " + model + "\n" +
        "Price: " + price + "\n" +
        "Type: " + table->item(row, 4)->text() + "\n" +
        "Status: " + table->item(row, 5)->text() + "\n" +
        "Customer: " + (v ? v->getCustomer() : "") + "\n" +
        "Days: " + (v ? QString::number(v->getDays()) : "0")
        );
}