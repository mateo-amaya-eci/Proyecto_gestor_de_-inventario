#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QSpinBox>
#include <QDateEdit>
#include <QLabel>
#include <QComboBox>
#include <QMessageBox>
#include <QGroupBox>
#include <QTimer>       // ¡Agregado!
#include "Component.h"
#include "DatabaseManager.h"
#include "InventoryManager.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void addComponent();
    void updateComponent();
    void deleteComponent();
    void searchComponents();
    void onTableSelectionChanged();
    void generateReport();
    void checkLowStock();

private:
    void setupUI();
    void loadComponents();
    void clearForm();
    void populateForm(const Component& component);
    Component getFormData() const;

    // Widgets
    QTableWidget *tableWidget;
    QLineEdit *nameEdit;
    QComboBox *typeCombo;
    QSpinBox *quantitySpin;
    QLineEdit *locationEdit;
    QDateEdit *dateEdit;
    
    QPushButton *addButton;
    QPushButton *updateButton;
    QPushButton *deleteButton;
    QPushButton *searchButton;
    QPushButton *reportButton;
    QLineEdit *searchEdit;
    
    QLabel *statusLabel;

    // Managers
    DatabaseManager *dbManager;
    InventoryManager *inventoryManager;
    int selectedId;
};

#endif // MAINWINDOW_H