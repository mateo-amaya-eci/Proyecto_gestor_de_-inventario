#include "Component.h"
#include <sstream>

Component::Component() : id(0), name(""), type(""), quantity(0), 
                        location(""), purchase_date("") {}

Component::Component(std::string name, std::string type, int quantity,
                   std::string location, std::string purchase_date)
    : id(0), name(name), type(type), quantity(quantity),
      location(location), purchase_date(purchase_date) {}

std::string Component::toString() const {
    std::stringstream ss;
    ss << "ID: " << id << ", Nombre: " << name << ", Tipo: " << type
       << ", Cantidad: " << quantity << ", Ubicación: " << location
       << ", Fecha: " << purchase_date;
    return ss.str();
}