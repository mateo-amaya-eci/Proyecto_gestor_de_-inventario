#include "MainWindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QHeaderView>
#include <QFileDialog>
#include <ctime>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), selectedId(-1)
{
    // Inicializar managers
    dbManager = new DatabaseManager();
    inventoryManager = new InventoryManager(dbManager);
    
    setupUI();
    loadComponents();
    
    // Conectar señales y slots
    connect(tableWidget, &QTableWidget::itemSelectionChanged, 
            this, &MainWindow::onTableSelectionChanged);
}

MainWindow::~MainWindow()
{
    delete inventoryManager;
    delete dbManager;
}

void MainWindow::setupUI()
{
    // Crear widget central y layout principal
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    
    // Crear tabla
    tableWidget = new QTableWidget(this);
    tableWidget->setColumnCount(6);
    QStringList headers = {"ID", "Nombre", "Tipo", "Cantidad", "Ubicación", "Fecha Compra"};
    tableWidget->setHorizontalHeaderLabels(headers);
    tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    
    // Formulario
    QGroupBox *formGroup = new QGroupBox("Componente", this);
    QFormLayout *formLayout = new QFormLayout(formGroup);
    
    nameEdit = new QLineEdit(this);
    typeCombo = new QComboBox(this);
    typeCombo->addItems({"Resistor", "Capacitor", "Transistor", "LED", "Microcontrolador", "Sensor", "Cable", "Otro"});
    quantitySpin = new QSpinBox(this);
    quantitySpin->setMinimum(0);
    quantitySpin->setMaximum(10000);
    locationEdit = new QLineEdit(this);
    dateEdit = new QDateEdit(this);
    dateEdit->setDate(QDate::currentDate());
    dateEdit->setCalendarPopup(true);
    
    formLayout->addRow("Nombre:", nameEdit);
    formLayout->addRow("Tipo:", typeCombo);
    formLayout->addRow("Cantidad:", quantitySpin);
    formLayout->addRow("Ubicación:", locationEdit);
    formLayout->addRow("Fecha Compra:", dateEdit);
    
    // Botones
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    
    addButton = new QPushButton("Agregar", this);
    updateButton = new QPushButton("Actualizar", this);
    deleteButton = new QPushButton("Eliminar", this);
    reportButton = new QPushButton("Generar Reporte", this);
    
    updateButton->setEnabled(false);
    deleteButton->setEnabled(false);
    
    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(updateButton);
    buttonLayout->addWidget(deleteButton);
    buttonLayout->addWidget(reportButton);
    buttonLayout->addStretch();
    
    // Búsqueda
    QHBoxLayout *searchLayout = new QHBoxLayout();
    searchEdit = new QLineEdit(this);
    searchEdit->setPlaceholderText("Buscar por nombre...");
    searchButton = new QPushButton("Buscar", this);
    
    searchLayout->addWidget(new QLabel("Buscar:", this));
    searchLayout->addWidget(searchEdit);
    searchLayout->addWidget(searchButton);
    
    // Estado
    statusLabel = new QLabel("Listo", this);
    
    // Conectar botones
    connect(addButton, &QPushButton::clicked, this, &MainWindow::addComponent);
    connect(updateButton, &QPushButton::clicked, this, &MainWindow::updateComponent);
    connect(deleteButton, &QPushButton::clicked, this, &MainWindow::deleteComponent);
    connect(searchButton, &QPushButton::clicked, this, &MainWindow::searchComponents);
    connect(reportButton, &QPushButton::clicked, this, &MainWindow::generateReport);
    
    // Agregar widgets al layout principal
    mainLayout->addWidget(tableWidget);
    mainLayout->addWidget(formGroup);
    mainLayout->addLayout(buttonLayout);
    mainLayout->addLayout(searchLayout);
    mainLayout->addWidget(statusLabel);
    
    setCentralWidget(centralWidget);
    setWindowTitle("Gestor de Inventario - Hogar/Laboratorio");
    resize(800, 600);
}

void MainWindow::loadComponents()
{
    // Implementar carga de componentes desde InventoryManager
}

void MainWindow::addComponent()
{
    // Implementar agregar componente
}

void MainWindow::updateComponent()
{
    // Implementar actualizar componente
}

void MainWindow::deleteComponent()
{
    // Implementar eliminar componente
}

void MainWindow::searchComponents()
{
    // Implementar búsqueda
}

void MainWindow::onTableSelectionChanged()
{
    // Implementar manejo de selección
}

void MainWindow::generateReport()
{
    // Implementar generación de reportes
}

void MainWindow::checkLowStock()
{
    // Implementar verificación de stock bajo
}

void MainWindow::clearForm()
{
    nameEdit->clear();
    typeCombo->setCurrentIndex(0);
    quantitySpin->setValue(0);
    locationEdit->clear();
    dateEdit->setDate(QDate::currentDate());
    selectedId = -1;
    updateButton->setEnabled(false);
    deleteButton->setEnabled(false);
}

void MainWindow::populateForm(const Component& component)
{
    nameEdit->setText(QString::fromStdString(component.getName()));
    
    // Buscar el tipo en el combo
    int index = typeCombo->findText(QString::fromStdString(component.getType()));
    if (index != -1) typeCombo->setCurrentIndex(index);
    
    quantitySpin->setValue(component.getQuantity());
    locationEdit->setText(QString::fromStdString(component.getLocation()));
    
    // Convertir time_t a QDate
    std::time_t purchaseTime = component.getPurchaseDate();
    std::tm* timeInfo = std::localtime(&purchaseTime);
    QDate date(timeInfo->tm_year + 1900, timeInfo->tm_mon + 1, timeInfo->tm_mday);
    dateEdit->setDate(date);
    
    selectedId = component.getId();
    updateButton->setEnabled(true);
    deleteButton->setEnabled(true);
}

Component MainWindow::getFormData() const
{
    std::string name = nameEdit->text().toStdString();
    std::string type = typeCombo->currentText().toStdString();
    int quantity = quantitySpin->value();
    std::string location = locationEdit->text().toStdString();
    
    // Convertir QDate a time_t
    QDate qDate = dateEdit->date();
    std::tm timeInfo = {};
    timeInfo.tm_year = qDate.year() - 1900;
    timeInfo.tm_mon = qDate.month() - 1;
    timeInfo.tm_mday = qDate.day();
    std::time_t purchaseDate = std::mktime(&timeInfo);
    
    return Component(selectedId, name, type, quantity, location, purchaseDate);
}
