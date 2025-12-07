#include "MainWindow.h"
#include <iostream>      // ¡Agregado para std::cout!
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QHeaderView>
#include <QFileDialog>
#include <QGroupBox>
#include <QTextStream>
#include <QDir>
#include <QTimer>
#include <QFileInfo>
#include <ctime>
#include <algorithm>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), selectedId(-1), dbManager(nullptr), inventoryManager(nullptr)
{
    // Inicializar managers
    dbManager = new DatabaseManager();
    if (!dbManager->connect()) {
        QMessageBox::critical(this, "Error", "No se pudo conectar a la base de datos");
    }
    
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
    tableWidget->horizontalHeader()->setStretchLastSection(true);
    
    // Formulario
    QGroupBox *formGroup = new QGroupBox("Componente", this);
    QFormLayout *formLayout = new QFormLayout(formGroup);
    
    nameEdit = new QLineEdit(this);
    nameEdit->setPlaceholderText("Ej: Resistor 1kΩ");
    
    typeCombo = new QComboBox(this);
    // Agregar items de forma más simple
    typeCombo->addItem("Resistor");
    typeCombo->addItem("Capacitor");
    typeCombo->addItem("Transistor");
    typeCombo->addItem("LED");
    typeCombo->addItem("Microcontrolador");
    typeCombo->addItem("Sensor");
    typeCombo->addItem("Cable");
    typeCombo->addItem("Otro");
    
    quantitySpin = new QSpinBox(this);
    quantitySpin->setMinimum(0);
    quantitySpin->setMaximum(10000);
    quantitySpin->setValue(1);
    
    locationEdit = new QLineEdit(this);
    locationEdit->setPlaceholderText("Ej: Cajón A, Estante 2");
    
    dateEdit = new QDateEdit(this);
    dateEdit->setDate(QDate::currentDate());
    dateEdit->setCalendarPopup(true);
    dateEdit->setDisplayFormat("dd/MM/yyyy");
    
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
    QPushButton *clearButton = new QPushButton("Limpiar", this);
    
    updateButton->setEnabled(false);
    deleteButton->setEnabled(false);
    
    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(updateButton);
    buttonLayout->addWidget(deleteButton);
    buttonLayout->addWidget(reportButton);
    buttonLayout->addWidget(clearButton);
    buttonLayout->addStretch();
    
    // Búsqueda
    QHBoxLayout *searchLayout = new QHBoxLayout();
    searchEdit = new QLineEdit(this);
    searchEdit->setPlaceholderText("Buscar por nombre, tipo o ubicación...");
    searchButton = new QPushButton("Buscar", this);
    QPushButton *showAllButton = new QPushButton("Mostrar Todos", this);
    
    searchLayout->addWidget(new QLabel("Buscar:", this));
    searchLayout->addWidget(searchEdit);
    searchLayout->addWidget(searchButton);
    searchLayout->addWidget(showAllButton);
    
    // Estado
    statusLabel = new QLabel("Listo", this);
    statusLabel->setStyleSheet("padding: 5px; background-color: #f0f0f0; border: 1px solid #ccc;");
    
    // Conectar botones
    connect(addButton, &QPushButton::clicked, this, &MainWindow::addComponent);
    connect(updateButton, &QPushButton::clicked, this, &MainWindow::updateComponent);
    connect(deleteButton, &QPushButton::clicked, this, &MainWindow::deleteComponent);
    connect(searchButton, &QPushButton::clicked, this, &MainWindow::searchComponents);
    connect(showAllButton, &QPushButton::clicked, this, [this]() { 
        searchEdit->clear(); 
        loadComponents(); 
    });
    connect(reportButton, &QPushButton::clicked, this, &MainWindow::generateReport);
    connect(clearButton, &QPushButton::clicked, this, &MainWindow::clearForm);
    
    // Agregar widgets al layout principal
    mainLayout->addWidget(tableWidget);
    mainLayout->addWidget(formGroup);
    mainLayout->addLayout(buttonLayout);
    mainLayout->addLayout(searchLayout);
    mainLayout->addWidget(statusLabel);
    
    setCentralWidget(centralWidget);
    setWindowTitle("Gestor de Inventario - Hogar/Laboratorio");
    resize(900, 650);
    
    // Verificar stock bajo periódicamente
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::checkLowStock);
    timer->start(30000); // Cada 30 segundos
}

void MainWindow::loadComponents()
{
    tableWidget->setRowCount(0);
    
    auto components = inventoryManager->getAllComponents();
    std::cout << "DEBUG - loadComponents: " << components.size() << " componentes" << std::endl;
    
    for (const auto& component : components) {
        int row = tableWidget->rowCount();
        tableWidget->insertRow(row);
        
        std::cout << "DEBUG - Component " << row << ":" << std::endl;
        std::cout << "  ID: " << component.getId() << std::endl;
        std::cout << "  Nombre: " << component.getName() << std::endl;
        std::cout << "  Tipo: " << component.getType() << std::endl;
        std::cout << "  Cantidad: " << component.getQuantity() << std::endl;
        std::cout << "  Ubicación: " << component.getLocation() << std::endl;
        
        // Mostrar datos en la tabla SIN comillas adicionales
        tableWidget->setItem(row, 0, new QTableWidgetItem(QString::number(component.getId())));
        tableWidget->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(component.getName())));
        tableWidget->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(component.getType())));
        
        QTableWidgetItem *quantityItem = new QTableWidgetItem(QString::number(component.getQuantity()));
        if (component.isLowStock()) {
            quantityItem->setBackground(QColor(255, 200, 200));
        }
        tableWidget->setItem(row, 3, quantityItem);
        
        tableWidget->setItem(row, 4, new QTableWidgetItem(QString::fromStdString(component.getLocation())));
        tableWidget->setItem(row, 5, new QTableWidgetItem(QString::fromStdString(component.getPurchaseDateString())));
    }
    
    statusLabel->setText(QString("Cargados %1 componentes").arg(components.size()));
    checkLowStock();
}

void MainWindow::addComponent()
{
    Component component = getFormData();
    
    // DEPURACIÓN: Mostrar los datos capturados
    std::cout << "DEBUG - Datos capturados:" << std::endl;
    std::cout << "  Nombre: " << component.getName() << std::endl;
    std::cout << "  Tipo: " << component.getType() << std::endl;
    std::cout << "  Cantidad: " << component.getQuantity() << std::endl;
    std::cout << "  Ubicación: " << component.getLocation() << std::endl;
    std::cout << "  Fecha: " << component.getPurchaseDateString() << std::endl;
    
    if (component.getName().empty()) {
        QMessageBox::warning(this, "Advertencia", "El nombre es requerido");
        return;
    }
    
    if (component.getQuantity() < 0) {
        QMessageBox::warning(this, "Advertencia", "La cantidad no puede ser negativa");
        return;
    }
    
    if (inventoryManager->addComponent(component)) {
        clearForm();
        loadComponents();
        statusLabel->setText("Componente agregado exitosamente");
        statusLabel->setStyleSheet("padding: 5px; background-color: #d4edda; border: 1px solid #c3e6cb; color: #155724;");
    } else {
        QMessageBox::critical(this, "Error", "No se pudo agregar el componente");
        statusLabel->setText("Error al agregar componente");
        statusLabel->setStyleSheet("padding: 5px; background-color: #f8d7da; border: 1px solid #f5c6cb; color: #721c24;");
    }
}

void MainWindow::updateComponent()
{
    if (selectedId == -1) return;
    
    Component component = getFormData();
    component.setId(selectedId);
    
    if (component.getName().empty()) {
        QMessageBox::warning(this, "Advertencia", "El nombre es requerido");
        return;
    }
    
    if (component.getQuantity() < 0) {
        QMessageBox::warning(this, "Advertencia", "La cantidad no puede ser negativa");
        return;
    }
    
    if (inventoryManager->updateComponent(component)) {
        loadComponents();
        statusLabel->setText("Componente actualizado exitosamente");
        statusLabel->setStyleSheet("padding: 5px; background-color: #d4edda; border: 1px solid #c3e6cb; color: #155724;");
    } else {
        QMessageBox::critical(this, "Error", "No se pudo actualizar el componente");
        statusLabel->setText("Error al actualizar componente");
        statusLabel->setStyleSheet("padding: 5px; background-color: #f8d7da; border: 1px solid #f5c6cb; color: #721c24;");
    }
}

void MainWindow::deleteComponent()
{
    if (selectedId == -1) return;
    
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirmar", 
                                  "¿Está seguro de eliminar este componente?",
                                  QMessageBox::Yes | QMessageBox::No);
    
    if (reply == QMessageBox::Yes) {
        if (inventoryManager->deleteComponent(selectedId)) {
            clearForm();
            loadComponents();
            statusLabel->setText("Componente eliminado exitosamente");
            statusLabel->setStyleSheet("padding: 5px; background-color: #d4edda; border: 1px solid #c3e6cb; color: #155724;");
        } else {
            QMessageBox::critical(this, "Error", "No se pudo eliminar el componente");
            statusLabel->setText("Error al eliminar componente");
            statusLabel->setStyleSheet("padding: 5px; background-color: #f8d7da; border: 1px solid #f5c6cb; color: #721c24;");
        }
    }
}

void MainWindow::searchComponents()
{
    std::string keyword = searchEdit->text().toStdString();
    if (keyword.empty()) {
        loadComponents();
        return;
    }
    
    tableWidget->setRowCount(0);
    auto components = inventoryManager->searchComponents(keyword);
    
    for (const auto& component : components) {
        int row = tableWidget->rowCount();
        tableWidget->insertRow(row);
        
        tableWidget->setItem(row, 0, new QTableWidgetItem(QString::number(component.getId())));
        tableWidget->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(component.getName())));
        tableWidget->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(component.getType())));
        
        QTableWidgetItem *quantityItem = new QTableWidgetItem(QString::number(component.getQuantity()));
        if (component.isLowStock()) {
            quantityItem->setBackground(QColor(255, 200, 200));
        }
        tableWidget->setItem(row, 3, quantityItem);
        
        tableWidget->setItem(row, 4, new QTableWidgetItem(QString::fromStdString(component.getLocation())));
        tableWidget->setItem(row, 5, new QTableWidgetItem(QString::fromStdString(component.getPurchaseDateString())));
    }
    
    statusLabel->setText(QString("Encontrados %1 componentes").arg(components.size()));
    statusLabel->setStyleSheet("padding: 5px; background-color: #d1ecf1; border: 1px solid #bee5eb; color: #0c5460;");
}

void MainWindow::onTableSelectionChanged()
{
    QList<QTableWidgetItem*> selectedItems = tableWidget->selectedItems();
    if (selectedItems.isEmpty()) {
        clearForm();
        return;
    }
    
    int row = selectedItems.first()->row();
    int id = tableWidget->item(row, 0)->text().toInt();
    
    // Buscar el componente por ID
    auto components = inventoryManager->getAllComponents();
    auto it = std::find_if(components.begin(), components.end(),
                          [id](const Component& c) { return c.getId() == id; });
    
    if (it != components.end()) {
        populateForm(*it);
    }
}

void MainWindow::generateReport()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    "Guardar Reporte",
                                                    QDir::homePath() + "/reporte_inventario.csv",
                                                    "CSV Files (*.csv);;Text Files (*.txt);;All Files (*)");
    
    if (fileName.isEmpty()) return;
    
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Error", "No se pudo abrir el archivo para escritura");
        return;
    }
    
    QTextStream stream(&file);
    stream.setCodec("UTF-8");
    
    // Encabezado
    stream << "ID,Nombre,Tipo,Cantidad,Ubicacion,Fecha Compra,Stock Bajo\n";
    
    auto components = inventoryManager->getAllComponents();
    for (const auto& component : components) {
        stream << component.getId() << ","
               << "\"" << QString::fromStdString(component.getName()).replace("\"", "\"\"") << "\","
               << "\"" << QString::fromStdString(component.getType()).replace("\"", "\"\"") << "\","
               << component.getQuantity() << ","
               << "\"" << QString::fromStdString(component.getLocation()).replace("\"", "\"\"") << "\","
               << "\"" << QString::fromStdString(component.getPurchaseDateString()) << "\","
               << (component.isLowStock() ? "SI" : "NO") << "\n";
    }
    
    file.close();
    statusLabel->setText(QString("Reporte generado: %1").arg(QFileInfo(fileName).fileName()));
    statusLabel->setStyleSheet("padding: 5px; background-color: #d4edda; border: 1px solid #c3e6cb; color: #155724;");
    
    QMessageBox::information(this, "Reporte Generado", 
                             QString("Reporte guardado exitosamente en:\n%1\n\nTotal de componentes: %2")
                             .arg(fileName).arg(components.size()));
}

void MainWindow::checkLowStock()
{
    auto lowStock = inventoryManager->getLowStockComponents();
    if (!lowStock.empty()) {
        QString warningText = QString("¡ATENCIÓN! Hay %1 componentes con stock bajo").arg(lowStock.size());
        statusLabel->setText(warningText);
        statusLabel->setStyleSheet("padding: 5px; background-color: #fff3cd; border: 1px solid #ffeaa7; color: #856404; font-weight: bold;");
        
        // Mostrar notificación ocasionalmente
        static int notificationCount = 0;
        if (notificationCount % 10 == 0) { // Cada 5 minutos aproximadamente
            QMessageBox::warning(this, "Stock Bajo", 
                                QString("Hay %1 componentes con stock bajo.\nRevise el inventario.")
                                .arg(lowStock.size()));
        }
        notificationCount++;
    } else {
        // Restaurar estilo normal si no hay stock bajo
        if (!statusLabel->text().contains("Error") && 
            !statusLabel->text().contains("generado") &&
            !statusLabel->text().contains("encontrados")) {
            statusLabel->setStyleSheet("padding: 5px; background-color: #f0f0f0; border: 1px solid #ccc;");
        }
    }
}

void MainWindow::clearForm()
{
    nameEdit->clear();
    typeCombo->setCurrentIndex(0);
    quantitySpin->setValue(1);
    locationEdit->clear();
    dateEdit->setDate(QDate::currentDate());
    selectedId = -1;
    updateButton->setEnabled(false);
    deleteButton->setEnabled(false);
    
    // Deseleccionar en la tabla
    tableWidget->clearSelection();
}

void MainWindow::populateForm(const Component& component)
{
    std::cout << "DEBUG - populateForm:" << std::endl;
    std::cout << "  Component name: " << component.getName() << std::endl;
    std::cout << "  Component type: " << component.getType() << std::endl;
    std::cout << "  Component location: " << component.getLocation() << std::endl;
    
    // Establecer texto directamente, no usar fromStdString
    nameEdit->setText(QString::fromStdString(component.getName()));
    
    // Para el combo, buscar exactamente
    QString typeStr = QString::fromStdString(component.getType());
    int index = typeCombo->findText(typeStr, Qt::MatchExactly);
    
    std::cout << "  Looking for type: " << typeStr.toStdString() << std::endl;
    std::cout << "  Found at index: " << index << std::endl;
    
    if (index != -1) {
        typeCombo->setCurrentIndex(index);
    } else {
        // Si no está, agregarlo al final
        typeCombo->addItem(typeStr);
        typeCombo->setCurrentIndex(typeCombo->count() - 1);
    }
    
    quantitySpin->setValue(component.getQuantity());
    locationEdit->setText(QString::fromStdString(component.getLocation()));
    
    // Convertir time_t a QDate
    std::time_t purchaseTime = component.getPurchaseDate();
    if (purchaseTime > 0) {
        std::tm* timeInfo = std::localtime(&purchaseTime);
        QDate date(timeInfo->tm_year + 1900, timeInfo->tm_mon + 1, timeInfo->tm_mday);
        dateEdit->setDate(date);
    }
    
    selectedId = component.getId();
    updateButton->setEnabled(true);
    deleteButton->setEnabled(true);
}

Component MainWindow::getFormData() const
{
    std::cout << "DEBUG - getFormData():" << std::endl;
    std::cout << "  nameEdit text: " << nameEdit->text().toStdString() << std::endl;
    std::cout << "  typeCombo text: " << typeCombo->currentText().toStdString() << std::endl;
    std::cout << "  location text: " << locationEdit->text().toStdString() << std::endl;
    
    // Obtener texto SIN comillas
    QString nameQStr = nameEdit->text().trimmed();
    QString typeQStr = typeCombo->currentText().trimmed();
    QString locationQStr = locationEdit->text().trimmed();
    
    // Eliminar comillas dobles si las hay
    nameQStr = nameQStr.replace("\"", "");
    typeQStr = typeQStr.replace("\"", "");
    locationQStr = locationQStr.replace("\"", "");
    
    std::string name = nameQStr.toStdString();
    std::string type = typeQStr.toStdString();
    int quantity = quantitySpin->value();
    std::string location = locationQStr.toStdString();
    
    std::cout << "  After processing:" << std::endl;
    std::cout << "    name: " << name << std::endl;
    std::cout << "    type: " << type << std::endl;
    std::cout << "    quantity: " << quantity << std::endl;
    std::cout << "    location: " << location << std::endl;
    
    // Convertir QDate a time_t
    QDate qDate = dateEdit->date();
    std::tm timeInfo = {};
    timeInfo.tm_year = qDate.year() - 1900;
    timeInfo.tm_mon = qDate.month() - 1;
    timeInfo.tm_mday = qDate.day();
    timeInfo.tm_hour = 12;
    timeInfo.tm_min = 0;
    timeInfo.tm_sec = 0;
    timeInfo.tm_isdst = -1;
    
    std::time_t purchaseDate = std::mktime(&timeInfo);
    
    return Component(selectedId, name, type, quantity, location, purchaseDate);
}