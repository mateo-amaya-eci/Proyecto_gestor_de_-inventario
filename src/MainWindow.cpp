#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QHeaderView>
#include <QMessageBox>
#include <QDate>

MainWindow::MainWindow(QWidget *parent) : 
    QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    setupUI();
    refreshTable();
    showLowStockAlert();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::setupUI() {
    // Configurar ventana principal
    setWindowTitle("Gestor de Inventario - Hogar/Laboratorio");
    setFixedSize(800, 600);
    
    // Crear tabla
    tableWidget = new QTableWidget(this);
    tableWidget->setGeometry(10, 10, 780, 300);
    tableWidget->setColumnCount(6);
    tableWidget->setHorizontalHeaderLabels(
        {"ID", "Nombre", "Tipo", "Cantidad", "Ubicación", "Fecha Compra"});
    tableWidget->horizontalHeader()->setStretchLastSection(true);
    
    // Campos del formulario
    QLabel *nameLabel = new QLabel("Nombre:", this);
    nameLabel->setGeometry(10, 320, 80, 25);
    nameEdit = new QLineEdit(this);
    nameEdit->setGeometry(100, 320, 200, 25);
    
    QLabel *typeLabel = new QLabel("Tipo:", this);
    typeLabel->setGeometry(10, 350, 80, 25);
    typeEdit = new QLineEdit(this);
    typeEdit->setGeometry(100, 350, 200, 25);
    
    QLabel *quantityLabel = new QLabel("Cantidad:", this);
    quantityLabel->setGeometry(10, 380, 80, 25);
    quantitySpin = new QSpinBox(this);
    quantitySpin->setGeometry(100, 380, 100, 25);
    quantitySpin->setMinimum(0);
    quantitySpin->setMaximum(9999);
    
    QLabel *locationLabel = new QLabel("Ubicación:", this);
    locationLabel->setGeometry(10, 410, 80, 25);
    locationEdit = new QLineEdit(this);
    locationEdit->setGeometry(100, 410, 200, 25);
    
    QLabel *dateLabel = new QLabel("Fecha:", this);
    dateLabel->setGeometry(10, 440, 80, 25);
    dateEdit = new QDateEdit(this);
    dateEdit->setGeometry(100, 440, 150, 25);
    dateEdit->setDate(QDate::currentDate());
    
    // Botones
    addButton = new QPushButton("Agregar", this);
    addButton->setGeometry(320, 320, 80, 30);
    connect(addButton, &QPushButton::clicked, this, &MainWindow::onAddButtonClicked);
    
    deleteButton = new QPushButton("Eliminar", this);
    deleteButton->setGeometry(320, 360, 80, 30);
    connect(deleteButton, &QPushButton::clicked, this, &MainWindow::onDeleteButtonClicked);
    
    // Búsqueda
    QLabel *searchLabel = new QLabel("Buscar:", this);
    searchLabel->setGeometry(450, 320, 80, 25);
    searchEdit = new QLineEdit(this);
    searchEdit->setGeometry(520, 320, 200, 25);
    connect(searchEdit, &QLineEdit::textChanged, this, &MainWindow::onSearchTextChanged);
    
    // Reportes
    reportButton = new QPushButton("Generar Reporte", this);
    reportButton->setGeometry(450, 360, 120, 30);
    connect(reportButton, &QPushButton::clicked, this, &MainWindow::onGenerateReportClicked);
}

void MainWindow::refreshTable() {
    tableWidget->setRowCount(0);
    auto components = inventoryManager.getAllComponents();
    
    for (const auto& comp : components) {
        int row = tableWidget->rowCount();
        tableWidget->insertRow(row);
        
        tableWidget->setItem(row, 0, new QTableWidgetItem(QString::number(comp.getId())));
        tableWidget->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(comp.getName())));
        tableWidget->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(comp.getType())));
        tableWidget->setItem(row, 3, new QTableWidgetItem(QString::number(comp.getQuantity())));
        tableWidget->setItem(row, 4, new QTableWidgetItem(QString::fromStdString(comp.getLocation())));
        tableWidget->setItem(row, 5, new QTableWidgetItem(QString::fromStdString(comp.getPurchaseDate())));
    }
}

void MainWindow::onAddButtonClicked() {
    std::string name = nameEdit->text().toStdString();
    std::string type = typeEdit->text().toStdString();
    int quantity = quantitySpin->value();
    std::string location = locationEdit->text().toStdString();
    std::string date = dateEdit->date().toString("yyyy-MM-dd").toStdString();
    
    if (name.empty() || type.empty() || location.empty()) {
        QMessageBox::warning(this, "Error", "Por favor complete todos los campos");
        return;
    }
    
    Component newComp(name, type, quantity, location, date);
    if (inventoryManager.addComponent(newComp)) {
        refreshTable();
        clearForm();
        showLowStockAlert();
        QMessageBox::information(this, "Éxito", "Componente agregado correctamente");
    } else {
        QMessageBox::critical(this, "Error", "Error al agregar componente");
    }
}

void MainWindow::showLowStockAlert() {
    auto lowStock = inventoryManager.getLowStockComponents();
    if (!lowStock.empty()) {
        QString message = "Componentes con stock bajo:\n";
        for (const auto& comp : lowStock) {
            message += QString("- %1 (%2): %3 unidades\n")
                      .arg(QString::fromStdString(comp.getName()))
                      .arg(QString::fromStdString(comp.getType()))
                      .arg(comp.getQuantity());
        }
        QMessageBox::warning(this, "Alerta de Stock Bajo", message);
    }
}