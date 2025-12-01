#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include "Component.h"
#include <sqlite3.h>
#include <vector>
#include <string>

/**
 * @class DatabaseManager
 * @brief Gestiona la conexión y operaciones con la base de datos SQLite.
 * 
 * La clase DatabaseManager proporciona métodos para interactuar con la base de datos,
 * incluyendo operaciones CRUD (Crear, Leer, Actualizar, Eliminar) para los componentes.
 */
class DatabaseManager {
private:
    sqlite3* db; /**< Puntero a la base de datos SQLite. */
    std::string dbPath; /**< Ruta del archivo de la base de datos. */

public:
    /**
     * @brief Constructor de la clase DatabaseManager.
     * 
     * Inicializa el gestor de base de datos con la ruta especificada.
     * 
     * @param path Ruta del archivo de la base de datos. Por defecto es "inventory.db".
     */
    DatabaseManager(std::string path = "inventory.db");

    /**
     * @brief Destructor de la clase DatabaseManager.
     * 
     * Cierra la conexión con la base de datos si está abierta.
     */
    ~DatabaseManager();
    
    /**
     * @brief Abre la conexión con la base de datos.
     * 
     * @return true si la conexión se abre correctamente, false en caso contrario.
     */
    bool openDatabase();

    /**
     * @brief Cierra la conexión con la base de datos.
     */
    void closeDatabase();

    /**
     * @brief Crea la tabla de componentes en la base de datos.
     * 
     * @return true si la tabla se crea correctamente, false en caso contrario.
     */
    bool createTable();

    /**
     * @brief Inserta un componente en la base de datos.
     * 
     * @param component Componente a insertar.
     * @return true si el componente se inserta correctamente, false en caso contrario.
     */
    bool insertComponent(const Component& component);

    /**
     * @brief Elimina un componente de la base de datos por su ID.
     * 
     * @param id ID del componente a eliminar.
     * @return true si el componente se elimina correctamente, false en caso contrario.
     */
    bool deleteComponent(int id);

    /**
     * @brief Actualiza un componente en la base de datos.
     * 
     * @param component Componente con los datos actualizados.
     * @return true si el componente se actualiza correctamente, false en caso contrario.
     */
    bool updateComponent(const Component& component);

    /**
     * @brief Selecciona todos los componentes de la base de datos.
     * 
     * @return Vector con todos los componentes almacenados en la base de datos.
     */
    std::vector<Component> selectAllComponents();

    /**
     * @brief Selecciona un componente de la base de datos por su ID.
     * 
     * @param id ID del componente a seleccionar.
     * @return Componente con el ID especificado, o un objeto vacío si no se encuentra.
     */
    Component selectComponentById(int id);

    /**
     * @brief Busca componentes en la base de datos que coincidan con una palabra clave.
     * 
     * @param keyword Palabra clave para buscar en los componentes.
     * @return Vector con los componentes que coinciden con la palabra clave.
     */
    std::vector<Component> searchComponents(std::string keyword);
};

#endif