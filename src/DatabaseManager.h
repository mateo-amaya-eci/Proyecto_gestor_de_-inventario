#ifndef COMPONENT_H
#define COMPONENT_H

#include <string>
#include <ctime>

/**
 * @class Component
 * @brief Representa un componente en el sistema de inventario.
 * 
 * La clase Component almacena información sobre un componente, incluyendo su ID, nombre, tipo, cantidad, ubicación y fecha de compra.
 */
class Component
{
private:
    int id; /**< Identificador único del componente. */
    std::string name; /**< Nombre del componente. */
    std::string type; /**< Tipo del componente. */
    int quantity; /**< Cantidad disponible del componente. */
    std::string location; /**< Ubicación del componente en el inventario. */
    std::time_t purchaseDate; /**< Fecha de compra del componente. */

public:
    /**
     * @brief Constructor por defecto.
     * 
     * Inicializa un objeto Component con valores predeterminados.
     */
    Component();
    
    /**
     * @brief Constructor parametrizado.
     * 
     * Inicializa un objeto Component con los valores proporcionados.
     * 
     * @param name Nombre del componente.
     * @param type Tipo del componente.
     * @param quantity Cantidad disponible del componente.
     * @param location Ubicación del componente en el inventario.
     * @param purchaseDate Fecha de compra del componente.
     */
    Component(const std::string& name, const std::string& type, 
              int quantity, const std::string& location, 
              std::time_t purchaseDate);
    
    /**
     * @brief Constructor parametrizado con ID.
     * 
     * Inicializa un objeto Component con los valores proporcionados, incluyendo el ID.
     * 
     * @param id Identificador único del componente.
     * @param name Nombre del componente.
     * @param type Tipo del componente.
     * @param quantity Cantidad disponible del componente.
     * @param location Ubicación del componente en el inventario.
     * @param purchaseDate Fecha de compra del componente.
     */
    Component(int id, const std::string& name, const std::string& type, 
              int quantity, const std::string& location, 
              std::time_t purchaseDate);
    
    /**
     * @brief Obtiene el ID del componente.
     * @return ID del componente.
     */
    int getId() const;

    /**
     * @brief Obtiene el nombre del componente.
     * @return Nombre del componente.
     */
    std::string getName() const;

    /**
     * @brief Obtiene el tipo del componente.
     * @return Tipo del componente.
     */
    std::string getType() const;

    /**
     * @brief Obtiene la cantidad disponible del componente.
     * @return Cantidad disponible del componente.
     */
    int getQuantity() const;

    /**
     * @brief Obtiene la ubicación del componente en el inventario.
     * @return Ubicación del componente.
     */
    std::string getLocation() const;

    /**
     * @brief Obtiene la fecha de compra del componente.
     * @return Fecha de compra del componente como un objeto std::time_t.
     */
    std::time_t getPurchaseDate() const;

    /**
     * @brief Obtiene la fecha de compra del componente como una cadena de texto.
     * @return Fecha de compra del componente en formato legible.
     */
    std::string getPurchaseDateString() const;
    
    /**
     * @brief Establece el ID del componente.
     * @param newId Nuevo ID del componente.
     */
    void setId(int newId);

    /**
     * @brief Establece el nombre del componente.
     * @param newName Nuevo nombre del componente.
     */
    void setName(const std::string& newName);

    /**
     * @brief Establece el tipo del componente.
     * @param newType Nuevo tipo del componente.
     */
    void setType(const std::string& newType);

    /**
     * @brief Establece la cantidad disponible del componente.
     * @param newQuantity Nueva cantidad del componente.
     */
    void setQuantity(int newQuantity);

    /**
     * @brief Establece la ubicación del componente en el inventario.
     * @param newLocation Nueva ubicación del componente.
     */
    void setLocation(const std::string& newLocation);

    /**
     * @brief Establece la fecha de compra del componente.
     * @param newDate Nueva fecha de compra del componente como un objeto std::time_t.
     */
    void setPurchaseDate(std::time_t newDate);
    
    /**
     * @brief Verifica si el stock del componente es bajo.
     * 
     * @param threshold Umbral de stock bajo (por defecto es 5).
     * @return true si el stock es menor o igual al umbral, false en caso contrario.
     */
    bool isLowStock(int threshold = 5) const;
};

#endif // COMPONENT_H