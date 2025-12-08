#ifndef INVENTORYMANAGER_H
#define INVENTORYMANAGER_H

#include <vector>
#include <memory>
#include "Component.h"
#include "DatabaseManager.h"

/**
 * @class InventoryManager
 * @brief Gestiona el inventario de componentes y su interacción con la base de datos.
 * 
 * La clase InventoryManager proporciona métodos para gestionar componentes en memoria,
 * realizar búsquedas y consultas, y sincronizar los datos con la base de datos.
 */
class InventoryManager
{
private:
    DatabaseManager* dbManager; /**< Puntero al gestor de la base de datos. */

public:
    /**
     * @brief Constructor por defecto.
     * 
     * Inicializa el gestor de inventario sin un gestor de base de datos.
     */
    InventoryManager();
    
    /**
     * @brief Constructor parametrizado.
     * 
     * Inicializa el gestor de inventario con un gestor de base de datos.
     * 
     * @param dbManager Puntero al gestor de base de datos.
     */
    InventoryManager(DatabaseManager* dbManager);
    
    /**
     * @brief Destructor de la clase InventoryManager.
     * 
     * Libera los recursos utilizados por el gestor de inventario.
     */
    ~InventoryManager();
    
    /**
     * @brief Agrega un componente al inventario.
     * 
     * @param component Componente a agregar.
     * @return true si el componente se agrega correctamente, false en caso contrario.
     */
    bool addComponent(const Component& component);
    
    /**
     * @brief Actualiza un componente en el inventario.
     * 
     * @param component Componente con los datos actualizados.
     * @return true si el componente se actualiza correctamente, false en caso contrario.
     */
    bool updateComponent(const Component& component);
    
    /**
     * @brief Elimina un componente del inventario por su ID.
     * 
     * @param id ID del componente a eliminar.
     * @return true si el componente se elimina correctamente, false en caso contrario.
     */
    bool deleteComponent(int id);
    
    /**
     * @brief Obtiene todos los componentes del inventario.
     * 
     * @return Vector con todos los componentes en memoria.
     */
    std::vector<Component> getAllComponents();
    
    /**
     * @brief Busca componentes en el inventario que coincidan con una palabra clave.
     * 
     * @param keyword Palabra clave para buscar en los componentes.
     * @return Vector con los componentes que coinciden con la palabra clave.
     */
    std::vector<Component> searchComponents(const std::string& keyword);
    
    /**
     * @brief Obtiene los componentes con bajo stock.
     * 
     * @param threshold Umbral de stock bajo (por defecto es 5).
     * @return Vector con los componentes cuyo stock es menor o igual al umbral.
     */
    std::vector<Component> getLowStockComponents(int threshold = 5);
    
    /**
     * @brief Establece el gestor de base de datos.
     * 
     * @param dbManager Puntero al gestor de base de datos.
     */
    void setDatabaseManager(DatabaseManager* dbManager);
    
    /**
     * @brief Obtiene el gestor de base de datos.
     * 
     * @return Puntero al gestor de base de datos.
     */
    DatabaseManager* getDatabaseManager() const;
};

#endif // INVENTORYMANAGER_H