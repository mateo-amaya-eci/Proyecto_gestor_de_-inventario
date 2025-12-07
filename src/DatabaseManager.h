#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <vector>
#include <sqlite3.h>
#include "Component.h"

class DatabaseManager
{
private:
    sqlite3* db;
    std::string databasePath;
    
    bool executeQuery(const std::string& query);
    bool initializeDatabase();
    
public:
    DatabaseManager();
    DatabaseManager(const std::string& path);
    ~DatabaseManager();
    
    bool connect();
    bool connect(const std::string& path);
    void disconnect();
    bool isConnected() const;
    
    // Operaciones CRUD
    bool addComponent(const Component& component);
    bool updateComponent(const Component& component);
    bool deleteComponent(int id);
    Component getComponent(int id);
    std::vector<Component> getAllComponents();
    std::vector<Component> searchComponents(const std::string& keyword);
    std::vector<Component> getLowStockComponents(int threshold = 5);
    
    // Métodos utilitarios
    int getComponentCount() const;
    std::vector<std::string> getComponentTypes() const;
    void debugTableInfo();
    bool recreateTable(); 
    void verifyLastInsert();
private:
    Component createComponentFromRow(sqlite3_stmt* stmt);
};

#endif // DATABASEMANAGER_H