#include "Component.h"
#include <iomanip>
#include <sstream>

Component::Component() 
    : id(-1), name(""), type(""), quantity(0), location(""), purchaseDate(0) {}

Component::Component(const std::string& name, const std::string& type, 
                     int quantity, const std::string& location, 
                     std::time_t purchaseDate)
    : id(-1), name(name), type(type), quantity(quantity), 
      location(location), purchaseDate(purchaseDate) {}

Component::Component(int id, const std::string& name, const std::string& type, 
                     int quantity, const std::string& location, 
                     std::time_t purchaseDate)
    : id(id), name(name), type(type), quantity(quantity), 
      location(location), purchaseDate(purchaseDate) {}

// Getters
int Component::getId() const { return id; }
std::string Component::getName() const { return name; }
std::string Component::getType() const { return type; }
int Component::getQuantity() const { return quantity; }
std::string Component::getLocation() const { return location; }
std::time_t Component::getPurchaseDate() const { return purchaseDate; }

std::string Component::getPurchaseDateString() const {
    if (purchaseDate == 0) return "No date";
    
    std::tm* ptm = std::localtime(&purchaseDate);
    char buffer[32];
    std::strftime(buffer, 32, "%Y-%m-%d", ptm);
    return std::string(buffer);
}

// Setters
void Component::setId(int newId) { id = newId; }
void Component::setName(const std::string& newName) { name = newName; }
void Component::setType(const std::string& newType) { type = newType; }
void Component::setQuantity(int newQuantity) { quantity = newQuantity; }
void Component::setLocation(const std::string& newLocation) { location = newLocation; }
void Component::setPurchaseDate(std::time_t newDate) { purchaseDate = newDate; }

bool Component::isLowStock(int threshold) const {
    return quantity <= threshold;
}