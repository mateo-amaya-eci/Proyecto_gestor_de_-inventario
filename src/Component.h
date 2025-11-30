#ifndef COMPONENT_H
#define COMPONENT_H

#include <string>
#include <iostream>

class Component {
private:
    int id;
    std::string name;
    std::string type;
    int quantity;
    std::string location;
    std::string purchase_date;

public:
    // Constructores
    Component();
    Component(std::string name, std::string type, int quantity, 
              std::string location, std::string purchase_date);
    
    // Getters
    int getId() const { return id; }
    std::string getName() const { return name; }
    std::string getType() const { return type; }
    int getQuantity() const { return quantity; }
    std::string getLocation() const { return location; }
    std::string getPurchaseDate() const { return purchase_date; }
    
    // Setters
    void setId(int newId) { id = newId; }
    void setName(std::string newName) { name = newName; }
    void setType(std::string newType) { type = newType; }
    void setQuantity(int newQuantity) { quantity = newQuantity; }
    void setLocation(std::string newLocation) { location = newLocation; }
    void setPurchaseDate(std::string newDate) { purchase_date = newDate; }
    
    // Método para mostrar información
    std::string toString() const;
};

#endif