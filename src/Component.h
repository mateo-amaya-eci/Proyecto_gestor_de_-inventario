#ifndef COMPONENT_H
#define COMPONENT_H

#include <string>
#include <iostream>

/**
 * @class Component
 * @brief Representa un componente en el sistema de inventario.
 * 
 * La clase Component almacena información sobre un componente, incluyendo su ID, nombre, tipo, cantidad, ubicación y fecha de compra.
 */
class Component {
private:
    int id; /**< Identificador único del componente. */
    std::string name; /**< Nombre del componente. */
    std::string type; /**< Tipo del componente. */
    int quantity; /**< Cantidad disponible del componente. */
    std::string location; /**< Ubicación del componente en el inventario. */
    std::string purchase_date; /**< Fecha de compra del componente. */

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
     * @param purchase_date Fecha de compra del componente.
     */
    Component(std::string name, std::string type, int quantity, 
              std::string location, std::string purchase_date);
    
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
     * @return Fecha de compra del componente.
     */
    std::string getPurchaseDate() const;
    
    /**
     * @brief Establece el ID del componente.
     * @param newId Nuevo ID del componente.
     */
    void setId(int newId);

    /**
     * @brief Establece el nombre del componente.
     * @param newName Nuevo nombre del componente.
     */
    void setName(std::string newName);

    /**
     * @brief Establece el tipo del componente.
     * @param newType Nuevo tipo del componente.
     */
    void setType(std::string newType);

    /**
     * @brief Establece la cantidad disponible del componente.
     * @param newQuantity Nueva cantidad del componente.
     */
    void setQuantity(int newQuantity);

    /**
     * @brief Establece la ubicación del componente en el inventario.
     * @param newLocation Nueva ubicación del componente.
     */
    void setLocation(std::string newLocation);

    /**
     * @brief Establece la fecha de compra del componente.
     * @param newDate Nueva fecha de compra del componente.
     */
    void setPurchaseDate(std::string newDate);
    
    /**
     * @brief Convierte la información del componente en una cadena de texto.
     * @return Cadena de texto con la información del componente.
     */
    std::string toString() const;
};

#endif