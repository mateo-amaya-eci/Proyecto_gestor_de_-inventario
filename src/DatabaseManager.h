#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include "Component.h"
#include <sqlite3.h>
#include <vector>
#include <string>

class DatabaseManager {
private:
    sqlite3* db;
    std::string dbPath;

public:
    DatabaseManager(std::string path = "inventory.db");
    ~DatabaseManager();
    
    bool openDatabase();
    void closeDatabase();
    bool createTable();
    bool insertComponent(const Component& component);
    bool deleteComponent(int id);
    bool updateComponent(const Component& component);
    std::vector<Component> selectAllComponents();
    Component selectComponentById(int id);
    std::vector<Component> searchComponents(std::string keyword);
};

#endif