#include "MainWindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QHeaderView>
#include <QFileDialog>
#include <QGroupBox>
#include <ctime>

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

void MainWindow::loadComponents()
{
    tableWidget->setRowCount(0);
    
    auto components = inventoryManager->getAllComponents();
    for (const auto& component : components) {
        int row = tableWidget->rowCount();
        tableWidget->insertRow(row);
        
        tableWidget->setItem(row, 0, new QTableWidgetItem(QString::number(component.getId())));
        tableWidget->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(component.getName())));
        tableWidget->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(component.getType())));
        tableWidget->setItem(row, 3, new QTableWidgetItem(QString::number(component.getQuantity())));
        tableWidget->setItem(row, 4, new QTableWidgetItem(QString::fromStdString(component.getLocation())));
        tableWidget->setItem(row, 5, new QTableWidgetItem(QString::fromStdString(component.getPurchaseDateString())));
    }
    
    statusLabel->setText(QString("Cargados %1 componentes").arg(components.size()));
    checkLowStock();
}

void MainWindow::addComponent()
{
    Component component = getFormData();
    if (component.getName().empty()) {
        QMessageBox::warning(this, "Advertencia", "El nombre es requerido");
        return;
    }
    
    if (inventoryManager->addComponent(component)) {
        clearForm();
        loadComponents();
        statusLabel->setText("Componente agregado exitosamente");
    } else {
        QMessageBox::critical(this, "Error", "No se pudo agregar el componente");
    }
}

void MainWindow::updateComponent()
{
    if (selectedId == -1) return;
    
    Component component = getFormData();
    component.setId(selectedId);
    
    if (inventoryManager->updateComponent(component)) {
        loadComponents();
        statusLabel->setText("Componente actualizado exitosamente");
    } else {
        QMessageBox::critical(this, "Error", "No se pudo actualizar el componente");
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
        } else {
            QMessageBox::critical(this, "Error", "No se pudo eliminar el componente");
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
        tableWidget->setItem(row, 3, new QTableWidgetItem(QString::number(component.getQuantity())));
        tableWidget->setItem(row, 4, new QTableWidgetItem(QString::fromStdString(component.getLocation())));
        tableWidget->setItem(row, 5, new QTableWidgetItem(QString::fromStdString(component.getPurchaseDateString())));
    }
    
    statusLabel->setText(QString("Encontrados %1 componentes").arg(components.size()));
}

void MainWindow::onTableSelectionChanged()
{
    QList<QTableWidgetItem*> selectedItems = tableWidget->selectedItems();
    if (selectedItems.isEmpty()) return;
    
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
                                                    "CSV Files (*.csv);;All Files (*)");
    
    if (fileName.isEmpty()) return;
    
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Error", "No se pudo abrir el archivo para escritura");
        return;
    }
    
    QTextStream stream(&file);
    stream << "ID,Nombre,Tipo,Cantidad,Ubicacion,Fecha Compra\n";
    
    auto components = inventoryManager->getAllComponents();
    for (const auto& component : components) {
        stream << component.getId() << ","
               << QString::fromStdString(component.getName()) << ","
               << QString::fromStdString(component.getType()) << ","
               << component.getQuantity() << ","
               << QString::fromStdString(component.getLocation()) << ","
               << QString::fromStdString(component.getPurchaseDateString()) << "\n";
    }
    
    file.close();
    statusLabel->setText(QString("Reporte generado: %1").arg(fileName));
}

void MainWindow::checkLowStock()
{
    auto lowStock = inventoryManager->getLowStockComponents();
    if (!lowStock.empty()) {
        statusLabel->setText(QString("¡ATENCIÓN! Hay %1 componentes con stock bajo").arg(lowStock.size()));
        statusLabel->setStyleSheet("color: red; font-weight: bold;");
    } else {
        statusLabel->setStyleSheet("");
    }
}