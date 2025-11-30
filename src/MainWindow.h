#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QLineEdit>
#include <QSpinBox>
#include <QDateEdit>
#include <QPushButton>
#include <QLabel>
#include "InventoryManager.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onAddButtonClicked();
    void onDeleteButtonClicked();
    void onSearchTextChanged();
    void onGenerateReportClicked();
    void onTableSelectionChanged();

private:
    Ui::MainWindow *ui;
    InventoryManager inventoryManager;
    
    void setupUI();
    void refreshTable();
    void clearForm();
    void showLowStockAlert();
    
    // Widgets
    QTableWidget *tableWidget;
    QLineEdit *nameEdit;
    QLineEdit *typeEdit;
    QSpinBox *quantitySpin;
    QLineEdit *locationEdit;
    QDateEdit *dateEdit;
    QLineEdit *searchEdit;
    QPushButton *addButton;
    QPushButton *deleteButton;
    QPushButton *reportButton;
};

#endif