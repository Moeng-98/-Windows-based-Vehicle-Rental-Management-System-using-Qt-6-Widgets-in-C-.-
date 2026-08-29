#pragma once

#include <QWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QTableWidget>
#include <QLabel>
#include <QPushButton>

#include "manager.h"

class MainWindow : public QWidget
{
    Q_OBJECT

public:

    explicit MainWindow(QWidget *parent = nullptr);

private:
    Manager manager;

    QLineEdit *idInput;
    QLineEdit *brandInput;
    QLineEdit *modelInput;
    QLineEdit *priceInput;
    QLineEdit *customerInput;
    QLineEdit *daysInput;

    QComboBox *typeCombo;

    QTableWidget *table;
    QTableWidget *historyTable;
    QLabel *selectedLabel;

    QPushButton *addBtn;
    QPushButton *rentBtn;
    QPushButton *returnBtn;
    QPushButton *removeBtn;
    QPushButton *refreshBtn;
    QPushButton *historyBtn;
    QLabel *historyLabel;

    double totalRevenue;

    void setupUI();
    void loadTable();
    void loadHistory();

private slots:
    void addVehicle();
    void rentVehicle();
    void returnVehicle();
    void removeVehicle();
    void onTableRowSelected(int row, int column);
};