#ifndef COMPONENT_H
#define COMPONENT_H

#include <string>
#include <ctime>

class Component
{
private:
    int id;
    std::string name;
    std::string type;
    int quantity;
    std::string location;
    std::time_t purchaseDate;

public:
    // Constructor por defecto
    Component();
    
    // Constructor con parámetros
    Component(const std::string& name, const std::string& type, 
              int quantity, const std::string& location, 
              std::time_t purchaseDate);
    
    // Constructor con ID (para cargar desde BD)
    Component(int id, const std::string& name, const std::string& type, 
              int quantity, const std::string& location, 
              std::time_t purchaseDate);
    
    // Getters
    int getId() const;
    std::string getName() const;
    std::string getType() const;
    int getQuantity() const;
    std::string getLocation() const;
    std::time_t getPurchaseDate() const;
    std::string getPurchaseDateString() const;
    
    // Setters
    void setId(int newId);
    void setName(const std::string& newName);
    void setType(const std::string& newType);
    void setQuantity(int newQuantity);
    void setLocation(const std::string& newLocation);
    void setPurchaseDate(std::time_t newDate);
    
    // Método para verificar si el stock es bajo
    bool isLowStock(int threshold = 5) const;
};

#endif // COMPONENT_H