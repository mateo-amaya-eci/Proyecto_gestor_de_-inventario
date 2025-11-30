#include "DatabaseManager.h"
#include <iostream>

// Callback para consultas SELECT
static int selectCallback(void* data, int argc, char** argv, char** azColName) {
    std::vector<Component>* components = static_cast<std::vector<Component>*>(data);
    
    Component comp;
    comp.setId(std::stoi(argv[0]));
    comp.setName(argv[1] ? argv[1] : "");
    comp.setType(argv[2] ? argv[2] : "");
    comp.setQuantity(std::stoi(argv[3]));
    comp.setLocation(argv[4] ? argv[4] : "");
    comp.setPurchaseDate(argv[5] ? argv[5] : "");
    
    components->push_back(comp);
    return 0;
}

DatabaseManager::DatabaseManager(std::string path) : db(nullptr), dbPath(path) {}

DatabaseManager::~DatabaseManager() {
    closeDatabase();
}

bool DatabaseManager::openDatabase() {
    int rc = sqlite3_open(dbPath.c_str(), &db);
    if (rc) {
        std::cerr << "Error abriendo BD: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }
    return createTable();
}

void DatabaseManager::closeDatabase() {
    if (db) {
        sqlite3_close(db);
        db = nullptr;
    }
}

bool DatabaseManager::createTable() {
    const char* sql = "CREATE TABLE IF NOT EXISTS componentes ("
                      "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                      "name TEXT NOT NULL, "
                      "type TEXT NOT NULL, "
                      "quantity INTEGER NOT NULL, "
                      "location TEXT NOT NULL, "
                      "purchase_date TEXT NOT NULL);";
    
    char* errorMsg = nullptr;
    int rc = sqlite3_exec(db, sql, nullptr, nullptr, &errorMsg);
    
    if (rc != SQLITE_OK) {
        std::cerr << "Error creando tabla: " << errorMsg << std::endl;
        sqlite3_free(errorMsg);
        return false;
    }
    return true;
}

bool DatabaseManager::insertComponent(const Component& component) {
    const char* sql = "INSERT INTO componentes (name, type, quantity, location, purchase_date) "
                      "VALUES (?, ?, ?, ?, ?);";
    
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    
    if (rc != SQLITE_OK) {
        std::cerr << "Error preparando statement: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }
    
    sqlite3_bind_text(stmt, 1, component.getName().c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, component.getType().c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 3, component.getQuantity());
    sqlite3_bind_text(stmt, 4, component.getLocation().c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 5, component.getPurchaseDate().c_str(), -1, SQLITE_STATIC);
    
    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    
    return rc == SQLITE_DONE;
}

std::vector<Component> DatabaseManager::selectAllComponents() {
    std::vector<Component> components;
    const char* sql = "SELECT * FROM componentes;";
    
    char* errorMsg = nullptr;
    int rc = sqlite3_exec(db, sql, selectCallback, &components, &errorMsg);
    
    if (rc != SQLITE_OK) {
        std::cerr << "Error en SELECT: " << errorMsg << std::endl;
        sqlite3_free(errorMsg);
    }
    
    return components;
}