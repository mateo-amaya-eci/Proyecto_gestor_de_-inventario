#ifndef INVENTORYMANAGER_H
#define INVENTORYMANAGER_H

#include "Component.h"
#include "DatabaseManager.h"
#include <vector>

/**
 * @class InventoryManager
 * @brief Gestiona el inventario de componentes y su interacción con la base de datos.
 *
 * La clase InventoryManager proporciona métodos para gestionar componentes en memoria,
 * realizar búsquedas y consultas, y sincronizar los datos con la base de datos.
 */
class InventoryManager {
private:
    std::vector<Component> components; /**< Vector que almacena los componentes en memoria. */
    DatabaseManager dbManager; /**< Gestor de la base de datos para operaciones de persistencia. */

public:
    /**
     * @brief Constructor de la clase InventoryManager.
     *
     * Inicializa el gestor de inventario y la conexión con la base de datos.
     */
    InventoryManager();

    /**
     * @brief Agrega un componente al inventario.
     *
     * @param component Componente a agregar.
     * @return true si el componente se agrega correctamente, false en caso contrario.
     */
    bool addComponent(const Component& component);

    /**
     * @brief Elimina un componente del inventario por su ID.
     *
     * @param id ID del componente a eliminar.
     * @return true si el componente se elimina correctamente, false en caso contrario.
     */
    bool removeComponent(int id);

    /**
     * @brief Actualiza un componente en el inventario.
     *
     * @param id ID del componente a actualizar.
     * @param component Componente con los datos actualizados.
     * @return true si el componente se actualiza correctamente, false en caso contrario.
     */
    bool updateComponent(int id, const Component& component);

    /**
     * @brief Obtiene todos los componentes del inventario.
     *
     * @return Vector con todos los componentes en memoria.
     */
    std::vector<Component> getAllComponents();

    /**
     * @brief Busca un componente en el inventario por su ID.
     *
     * @param id ID del componente a buscar.
     * @return Puntero al componente encontrado, o nullptr si no existe.
     */
    Component* findComponentById(int id);

    /**
     * @brief Busca componentes en el inventario por su nombre.
     *
     * @param name Nombre del componente a buscar.
     * @return Vector con los componentes que coinciden con el nombre.
     */
    std::vector<Component> findComponentsByName(std::string name);

    /**
     * @brief Busca componentes en el inventario por su tipo.
     *
     * @param type Tipo del componente a buscar.
     * @return Vector con los componentes que coinciden con el tipo.
     */
    std::vector<Component> findComponentsByType(std::string type);

    /**
     * @brief Busca componentes en el inventario por su ubicación.
     *
     * @param location Ubicación del componente a buscar.
     * @return Vector con los componentes que coinciden con la ubicación.
     */
    std::vector<Component> findComponentsByLocation(std::string location);

    /**
     * @brief Obtiene los componentes con bajo stock.
     *
     * @param threshold Umbral de stock bajo (por defecto es 5).
     * @return Vector con los componentes cuyo stock es menor o igual al umbral.
     */
    std::vector<Component> getLowStockComponents(int threshold = 5);

    /**
     * @brief Carga los componentes desde la base de datos al inventario.
     *
     * @return true si los componentes se cargan correctamente, false en caso contrario.
     */
    bool loadFromDatabase();

    /**
     * @brief Guarda los componentes del inventario en la base de datos.
     *
     * @return true si los componentes se guardan correctamente, false en caso contrario.
     */
    bool saveToDatabase();
};

#endif