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

/**
 * @class MainWindow
 * @brief Representa la ventana principal de la aplicación de gestión de inventario.
 *
 * La clase MainWindow proporciona una interfaz gráfica para interactuar con el inventario,
 * permitiendo agregar, eliminar, buscar componentes y generar reportes.
 */
class MainWindow : public QMainWindow {
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
    void onAddButtonClicked();

    /**
     * @brief Slot que se ejecuta cuando se presiona el botón de eliminar.
     *
     * Elimina el componente seleccionado del inventario.
     */
    void onDeleteButtonClicked();

    /**
     * @brief Slot que se ejecuta cuando el texto de búsqueda cambia.
     *
     * Filtra los componentes en la tabla según el texto ingresado.
     */
    void onSearchTextChanged();

    /**
     * @brief Slot que se ejecuta cuando se presiona el botón de generar reporte.
     *
     * Genera un reporte de los componentes del inventario.
     */
    void onGenerateReportClicked();

    /**
     * @brief Slot que se ejecuta cuando cambia la selección en la tabla.
     *
     * Actualiza los campos del formulario con los datos del componente seleccionado.
     */
    void onTableSelectionChanged();

private:
    Ui::MainWindow *ui; /**< Puntero a la interfaz gráfica generada por Qt Designer. */
    InventoryManager inventoryManager; /**< Gestor del inventario para manejar los componentes. */

    /**
     * @brief Configura la interfaz gráfica de la ventana principal.
     */
    void setupUI();

    /**
     * @brief Actualiza la tabla con los componentes del inventario.
     */
    void refreshTable();

    /**
     * @brief Limpia los campos del formulario.
     */
    void clearForm();

    /**
     * @brief Muestra una alerta si hay componentes con bajo stock.
     */
    void showLowStockAlert();

    // Widgets de la interfaz gráfica
    QTableWidget *tableWidget; /**< Tabla para mostrar los componentes del inventario. */
    QLineEdit *nameEdit; /**< Campo de texto para ingresar el nombre del componente. */
    QLineEdit *typeEdit; /**< Campo de texto para ingresar el tipo del componente. */
    QSpinBox *quantitySpin; /**< Campo para ingresar la cantidad del componente. */
    QLineEdit *locationEdit; /**< Campo de texto para ingresar la ubicación del componente. */
    QDateEdit *dateEdit; /**< Campo para ingresar la fecha de compra del componente. */
    QLineEdit *searchEdit; /**< Campo de texto para buscar componentes. */
    QPushButton *addButton; /**< Botón para agregar un nuevo componente. */
    QPushButton *deleteButton; /**< Botón para eliminar el componente seleccionado. */
    QPushButton *reportButton; /**< Botón para generar un reporte de los componentes. */
};

#endif