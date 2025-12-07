#include "DatabaseManager.h"
#include <iostream>
#include <algorithm>

DatabaseManager::DatabaseManager() : db(nullptr), databasePath("inventory.db") {}

DatabaseManager::DatabaseManager(const std::string& path) 
    : db(nullptr), databasePath(path) {}

DatabaseManager::~DatabaseManager() {
    disconnect();
}

bool DatabaseManager::connect() {
    return connect(databasePath);
}

bool DatabaseManager::connect(const std::string& path) {
    if (isConnected()) {
        disconnect();
    }
    
    int rc = sqlite3_open(path.c_str(), &db);
    if (rc != SQLITE_OK) {
        std::cerr << "Error al abrir la base de datos: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }
    
    // Inicializar la base de datos
    return initializeDatabase();
}

void DatabaseManager::disconnect() {
    if (db) {
        sqlite3_close(db);
        db = nullptr;
    }
}

bool DatabaseManager::isConnected() const {
    return db != nullptr;
}

bool DatabaseManager::initializeDatabase() {
    if (!isConnected()) return false;
    
    std::string createTableSQL = R"(
        CREATE TABLE IF NOT EXISTS components (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT NOT NULL,
            type TEXT NOT NULL,
            quantity INTEGER NOT NULL DEFAULT 0,
            location TEXT,
            purchase_date INTEGER
        );
        
        CREATE INDEX IF NOT EXISTS idx_name ON components(name);
        CREATE INDEX IF NOT EXISTS idx_type ON components(type);
        CREATE INDEX IF NOT EXISTS idx_location ON components(location);
    )";
    
    return executeQuery(createTableSQL);
}

bool DatabaseManager::executeQuery(const std::string& query) {
    if (!isConnected()) return false;
    
    char* errorMessage = nullptr;
    int rc = sqlite3_exec(db, query.c_str(), nullptr, nullptr, &errorMessage);
    
    if (rc != SQLITE_OK) {
        std::cerr << "Error en consulta SQL: " << errorMessage << std::endl;
        sqlite3_free(errorMessage);
        return false;
    }
    
    return true;
}

bool DatabaseManager::addComponent(const Component& component) {
    if (!isConnected()) return false;
    
    std::string sql = R"(
        INSERT INTO components (name, type, quantity, location, purchase_date)
        VALUES (?, ?, ?, ?, ?)
    )";
    
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    
    if (rc != SQLITE_OK) {
        std::cerr << "Error al preparar consulta: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }
    
    sqlite3_bind_text(stmt, 1, component.getName().c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, component.getType().c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 3, component.getQuantity());
    sqlite3_bind_text(stmt, 4, component.getLocation().c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int64(stmt, 5, static_cast<sqlite3_int64>(component.getPurchaseDate()));
    
    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    
    return rc == SQLITE_DONE;
}

// Los demás métodos los implementaremos después