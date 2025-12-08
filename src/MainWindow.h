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

/**
 * @class MainWindow
 * @brief Representa la ventana principal de la aplicación de gestión de inventario.
 * 
 * La clase MainWindow proporciona una interfaz gráfica para interactuar con el inventario,
 * permitiendo agregar, actualizar, eliminar, buscar componentes y generar reportes.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief Constructor de la clase MainWindow.
     * 
     * Inicializa la ventana principal y configura la interfaz gráfica.
     * 
     * @param parent Widget padre de la ventana principal (por defecto es nullptr).
     */
    MainWindow(QWidget *parent = nullptr);

    /**
     * @brief Destructor de la clase MainWindow.
     * 
     * Libera los recursos utilizados por la ventana principal.
     */
    ~MainWindow();

private slots:
    /**
     * @brief Slot que se ejecuta cuando se presiona el botón de agregar.
     * 
     * Agrega un nuevo componente al inventario.
     */
    void addComponent();

    /**
     * @brief Slot que se ejecuta cuando se presiona el botón de actualizar.
     * 
     * Actualiza el componente seleccionado en el inventario.
     */
    void updateComponent();

    /**
     * @brief Slot que se ejecuta cuando se presiona el botón de eliminar.
     * 
     * Elimina el componente seleccionado del inventario.
     */
    void deleteComponent();

    /**
     * @brief Slot que se ejecuta cuando se presiona el botón de buscar.
     * 
     * Busca componentes en el inventario según el texto ingresado.
     */
    void searchComponents();

    /**
     * @brief Slot que se ejecuta cuando cambia la selección en la tabla.
     * 
     * Actualiza los campos del formulario con los datos del componente seleccionado.
     */
    void onTableSelectionChanged();

    /**
     * @brief Slot que se ejecuta cuando se presiona el botón de generar reporte.
     * 
     * Genera un reporte de los componentes del inventario.
     */
    void generateReport();

    /**
     * @brief Slot que verifica si hay componentes con bajo stock.
     * 
     * Muestra una alerta si se detectan componentes con bajo stock.
     */
    void checkLowStock();

private:
    /**
     * @brief Configura la interfaz gráfica de la ventana principal.
     */
    void setupUI();

    /**
     * @brief Carga los componentes desde la base de datos al inventario.
     */
    void loadComponents();

    /**
     * @brief Limpia los campos del formulario.
     */
    void clearForm();

    /**
     * @brief Llena los campos del formulario con los datos de un componente.
     * 
     * @param component Componente cuyos datos se usarán para llenar el formulario.
     */
    void populateForm(const Component& component);

    /**
     * @brief Obtiene los datos ingresados en el formulario como un objeto Component.
     * 
     * @return Objeto Component con los datos del formulario.
     */
    Component getFormData() const;

    // Widgets
    QTableWidget *tableWidget; /**< Tabla para mostrar los componentes del inventario. */
    QLineEdit *nameEdit; /**< Campo de texto para ingresar el nombre del componente. */
    QComboBox *typeCombo; /**< ComboBox para seleccionar el tipo del componente. */
    QSpinBox *quantitySpin; /**< Campo para ingresar la cantidad del componente. */
    QLineEdit *locationEdit; /**< Campo de texto para ingresar la ubicación del componente. */
    QDateEdit *dateEdit; /**< Campo para ingresar la fecha de compra del componente. */
    
    QPushButton *addButton; /**< Botón para agregar un nuevo componente. */
    QPushButton *updateButton; /**< Botón para actualizar el componente seleccionado. */
    QPushButton *deleteButton; /**< Botón para eliminar el componente seleccionado. */
    QPushButton *searchButton; /**< Botón para buscar componentes en el inventario. */
    QPushButton *reportButton; /**< Botón para generar un reporte de los componentes. */
    QLineEdit *searchEdit; /**< Campo de texto para buscar componentes. */
    
    QLabel *statusLabel; /**< Etiqueta para mostrar el estado de la aplicación. */

    // Managers
    DatabaseManager *dbManager; /**< Gestor de la base de datos para manejar los componentes. */
    InventoryManager *inventoryManager; /**< Gestor del inventario para manejar los componentes. */
    int selectedId; /**< ID del componente actualmente seleccionado en la tabla. */
};

#endif // MAINWINDOW_H