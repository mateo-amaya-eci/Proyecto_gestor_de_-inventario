#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <vector>
#include <sqlite3.h>
#include "Component.h"

/**
 * @class DatabaseManager
 * @brief Gestiona la conexión y operaciones con la base de datos SQLite.
 * 
 * La clase DatabaseManager proporciona métodos para interactuar con la base de datos,
 * incluyendo operaciones CRUD (Crear, Leer, Actualizar, Eliminar) para los componentes.
 */
class DatabaseManager
{
private:
    sqlite3* db; /**< Puntero a la base de datos SQLite. */
    std::string databasePath; /**< Ruta del archivo de la base de datos. */

    /**
     * @brief Ejecuta una consulta SQL en la base de datos.
     * 
     * @param query Consulta SQL a ejecutar.
     * @return true si la consulta se ejecuta correctamente, false en caso contrario.
     */
    bool executeQuery(const std::string& query);

    /**
     * @brief Inicializa la base de datos creando las tablas necesarias.
     * 
     * @return true si la base de datos se inicializa correctamente, false en caso contrario.
     */
    bool initializeDatabase();

public:
    /**
     * @brief Constructor por defecto.
     * 
     * Inicializa el gestor de base de datos sin una ruta específica.
     */
    DatabaseManager();

    /**
     * @brief Constructor parametrizado.
     * 
     * Inicializa el gestor de base de datos con la ruta especificada.
     * 
     * @param path Ruta del archivo de la base de datos.
     */
    DatabaseManager(const std::string& path);

    /**
     * @brief Destructor de la clase DatabaseManager.
     * 
     * Cierra la conexión con la base de datos si está abierta.
     */
    ~DatabaseManager();

    /**
     * @brief Conecta a la base de datos usando la ruta especificada en el constructor.
     * 
     * @return true si la conexión se establece correctamente, false en caso contrario.
     */
    bool connect();

    /**
     * @brief Conecta a la base de datos usando una ruta específica.
     * 
     * @param path Ruta del archivo de la base de datos.
     * @return true si la conexión se establece correctamente, false en caso contrario.
     */
    bool connect(const std::string& path);

    /**
     * @brief Desconecta de la base de datos.
     */
    void disconnect();

    /**
     * @brief Verifica si la conexión con la base de datos está activa.
     * 
     * @return true si la conexión está activa, false en caso contrario.
     */
    bool isConnected() const;

    // Operaciones CRUD

    /**
     * @brief Agrega un componente a la base de datos.
     * 
     * @param component Componente a agregar.
     * @return true si el componente se agrega correctamente, false en caso contrario.
     */
    bool addComponent(const Component& component);

    /**
     * @brief Actualiza un componente en la base de datos.
     * 
     * @param component Componente con los datos actualizados.
     * @return true si el componente se actualiza correctamente, false en caso contrario.
     */
    bool updateComponent(const Component& component);

    /**
     * @brief Elimina un componente de la base de datos por su ID.
     * 
     * @param id ID del componente a eliminar.
     * @return true si el componente se elimina correctamente, false en caso contrario.
     */
    bool deleteComponent(int id);

    /**
     * @brief Obtiene un componente de la base de datos por su ID.
     * 
     * @param id ID del componente a obtener.
     * @return Componente con el ID especificado, o un objeto vacío si no se encuentra.
     */
    Component getComponent(int id);

    /**
     * @brief Obtiene todos los componentes de la base de datos.
     * 
     * @return Vector con todos los componentes almacenados en la base de datos.
     */
    std::vector<Component> getAllComponents();

    /**
     * @brief Busca componentes en la base de datos que coincidan con una palabra clave.
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

    // Métodos utilitarios

    /**
     * @brief Obtiene el número total de componentes en la base de datos.
     * 
     * @return Número total de componentes.
     */
    int getComponentCount() const;

    /**
     * @brief Obtiene los tipos de componentes únicos en la base de datos.
     * 
     * @return Vector con los tipos de componentes únicos.
     */
    std::vector<std::string> getComponentTypes() const;

    /**
     * @brief Muestra información de depuración sobre las tablas de la base de datos.
     */
    void debugTableInfo();

    /**
     * @brief Recrea la tabla de componentes en la base de datos.
     * 
     * @return true si la tabla se recrea correctamente, false en caso contrario.
     */
    bool recreateTable();

    /**
     * @brief Verifica la información del último componente insertado en la base de datos.
     */
    void verifyLastInsert();

private:
    /**
     * @brief Crea un objeto Component a partir de una fila de la base de datos.
     * 
     * @param stmt Puntero al objeto sqlite3_stmt que contiene la fila de datos.
     * @return Objeto Component creado a partir de la fila.
     */
    Component createComponentFromRow(sqlite3_stmt* stmt);
};

#endif // DATABASEMANAGER_H