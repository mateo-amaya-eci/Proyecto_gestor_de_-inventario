#ifndef INVENTORYMANAGER_H
#define INVENTORYMANAGER_H

#include "Component.h"
#include "DatabaseManager.h"
#include <vector>

class InventoryManager {
private:
    std::vector<Component> components;
    DatabaseManager dbManager;

public:
    InventoryManager();
    
    // Gestión de componentes
    bool addComponent(const Component& component);
    bool removeComponent(int id);
    bool updateComponent(int id, const Component& component);
    
    // Búsquedas y consultas
    std::vector<Component> getAllComponents();
    Component* findComponentById(int id);
    std::vector<Component> findComponentsByName(std::string name);
    std::vector<Component> findComponentsByType(std::string type);
    std::vector<Component> findComponentsByLocation(std::string location);
    std::vector<Component> getLowStockComponents(int threshold = 5);
    
    // Operaciones de base de datos
    bool loadFromDatabase();
    bool saveToDatabase();
};

#endif