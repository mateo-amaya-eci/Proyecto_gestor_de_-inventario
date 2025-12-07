#ifndef INVENTORYMANAGER_H
#define INVENTORYMANAGER_H

#include <vector>
#include <memory>
#include "Component.h"
#include "DatabaseManager.h"

class InventoryManager
{
private:
    DatabaseManager* dbManager;
    
public:
    // Constructor por defecto
    InventoryManager();
    
    // Constructor con DatabaseManager
    InventoryManager(DatabaseManager* dbManager);
    
    // Destructor
    ~InventoryManager();
    
    // Métodos para gestionar componentes
    bool addComponent(const Component& component);
    bool updateComponent(const Component& component);
    bool deleteComponent(int id);
    std::vector<Component> getAllComponents();
    std::vector<Component> searchComponents(const std::string& keyword);
    std::vector<Component> getLowStockComponents(int threshold = 5);
    
    // Getters y setters
    void setDatabaseManager(DatabaseManager* dbManager);
    DatabaseManager* getDatabaseManager() const;
};

#endif // INVENTORYMANAGER_H