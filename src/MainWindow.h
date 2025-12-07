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
#include <QTimer>
#include "Component.h"
#include "DatabaseManager.h"
#include "InventoryManager.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT  // Macro necesaria para señales y slots de Qt

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:  // Métodos que responden a eventos (señales)
    void addComponent();
    void updateComponent();
    void deleteComponent();
    void searchComponents();
    void onTableSelectionChanged();
    void generateReport();
    void checkLowStock();

private:  // Métodos internos (implementación)
    void setupUI();
    void loadComponents();
    void clearForm();
    void populateForm(const Component& component);
    Component getFormData() const;

private:  // Variables miembro (estado interno)
    // Widgets de la interfaz
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

    // Managers para lógica de negocio
    DatabaseManager *dbManager;
    InventoryManager *inventoryManager;
    int selectedId;
};

#endif // MAINWINDOW_H