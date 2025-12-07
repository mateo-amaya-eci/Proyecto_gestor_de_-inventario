#include "DatabaseManager.h"
#include <iostream>
#include <algorithm>
#include <cstring>

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

bool DatabaseManager::updateComponent(const Component& component) {
    if (!isConnected()) return false;
    
    std::string sql = R"(
        UPDATE components 
        SET name = ?, type = ?, quantity = ?, location = ?, purchase_date = ?
        WHERE id = ?
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
    sqlite3_bind_int(stmt, 6, component.getId());
    
    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    
    return rc == SQLITE_DONE;
}

bool DatabaseManager::deleteComponent(int id) {
    if (!isConnected()) return false;
    
    std::string sql = "DELETE FROM components WHERE id = ?";
    
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    
    if (rc != SQLITE_OK) {
        std::cerr << "Error al preparar consulta: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }
    
    sqlite3_bind_int(stmt, 1, id);
    
    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    
    return rc == SQLITE_DONE;
}

Component DatabaseManager::getComponent(int id) {
    if (!isConnected()) return Component();
    
    std::string sql = "SELECT * FROM components WHERE id = ?";
    
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    
    if (rc != SQLITE_OK) {
        std::cerr << "Error al preparar consulta: " << sqlite3_errmsg(db) << std::endl;
        return Component();
    }
    
    sqlite3_bind_int(stmt, 1, id);
    
    rc = sqlite3_step(stmt);
    Component component;
    
    if (rc == SQLITE_ROW) {
        component = createComponentFromRow(stmt);
    }
    
    sqlite3_finalize(stmt);
    return component;
}

std::vector<Component> DatabaseManager::getAllComponents() {
    std::vector<Component> components;
    
    if (!isConnected()) return components;
    
    std::string sql = "SELECT * FROM components ORDER BY name";
    
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    
    if (rc != SQLITE_OK) {
        std::cerr << "Error al preparar consulta: " << sqlite3_errmsg(db) << std::endl;
        return components;
    }
    
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        components.push_back(createComponentFromRow(stmt));
    }
    
    sqlite3_finalize(stmt);
    return components;
}

std::vector<Component> DatabaseManager::searchComponents(const std::string& keyword) {
    std::vector<Component> components;
    
    if (!isConnected()) return components;
    
    std::string sql = R"(
        SELECT * FROM components 
        WHERE name LIKE ? OR type LIKE ? OR location LIKE ?
        ORDER BY name
    )";
    
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    
    if (rc != SQLITE_OK) {
        std::cerr << "Error al preparar consulta: " << sqlite3_errmsg(db) << std::endl;
        return components;
    }
    
    std::string searchPattern = "%" + keyword + "%";
    sqlite3_bind_text(stmt, 1, searchPattern.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, searchPattern.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, searchPattern.c_str(), -1, SQLITE_STATIC);
    
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        components.push_back(createComponentFromRow(stmt));
    }
    
    sqlite3_finalize(stmt);
    return components;
}

std::vector<Component> DatabaseManager::getLowStockComponents(int threshold) {
    std::vector<Component> components;
    
    if (!isConnected()) return components;
    
    std::string sql = "SELECT * FROM components WHERE quantity <= ? ORDER BY quantity";
    
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    
    if (rc != SQLITE_OK) {
        std::cerr << "Error al preparar consulta: " << sqlite3_errmsg(db) << std::endl;
        return components;
    }
    
    sqlite3_bind_int(stmt, 1, threshold);
    
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        components.push_back(createComponentFromRow(stmt));
    }
    
    sqlite3_finalize(stmt);
    return components;
}

int DatabaseManager::getComponentCount() const {
    if (!isConnected()) return 0;
    
    std::string sql = "SELECT COUNT(*) FROM components";
    
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    
    if (rc != SQLITE_OK) {
        std::cerr << "Error al preparar consulta: " << sqlite3_errmsg(db) << std::endl;
        return 0;
    }
    
    rc = sqlite3_step(stmt);
    int count = 0;
    
    if (rc == SQLITE_ROW) {
        count = sqlite3_column_int(stmt, 0);
    }
    
    sqlite3_finalize(stmt);
    return count;
}

std::vector<std::string> DatabaseManager::getComponentTypes() const {
    std::vector<std::string> types;
    
    if (!isConnected()) return types;
    
    std::string sql = "SELECT DISTINCT type FROM components ORDER BY type";
    
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    
    if (rc != SQLITE_OK) {
        std::cerr << "Error al preparar consulta: " << sqlite3_errmsg(db) << std::endl;
        return types;
    }
    
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        const char* type = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        if (type) {
            types.push_back(std::string(type));
        }
    }
    
    sqlite3_finalize(stmt);
    return types;
}

Component DatabaseManager::createComponentFromRow(sqlite3_stmt* stmt) {
    // Verificar el orden de las columnas según el CREATE TABLE
    // CREATE TABLE components (
    //     id INTEGER PRIMARY KEY AUTOINCREMENT,        // columna 0
    //     name TEXT NOT NULL,                         // columna 1  
    //     type TEXT NOT NULL,                         // columna 2
    //     quantity INTEGER NOT NULL DEFAULT 0,        // columna 3
    //     location TEXT,                              // columna 4
    //     purchase_date INTEGER                       // columna 5
    // );
    
    int id = sqlite3_column_int(stmt, 0);                    // columna 0: id
    
    const char* name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));  // columna 1: name
    const char* type = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));  // columna 2: type
    int quantity = sqlite3_column_int(stmt, 3);              // columna 3: quantity
    
    const char* location = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));  // columna 4: location
    std::time_t purchaseDate = static_cast<std::time_t>(sqlite3_column_int64(stmt, 5));  // columna 5: purchase_date
    
    // DEPURACIÓN: Verificar qué se está leyendo
    std::cout << "\n=== DEBUG createComponentFromRow ===" << std::endl;
    std::cout << "Columna 1 (name): " << (name ? std::string(name) : "NULL") << std::endl;
    std::cout << "Columna 2 (type): " << (type ? std::string(type) : "NULL") << std::endl;
    std::cout << "Columna 4 (location): " << (location ? std::string(location) : "NULL") << std::endl;
    
    return Component(id,
                     name ? std::string(name) : "",
                     type ? std::string(type) : "",
                     quantity,
                     location ? std::string(location) : "",
                     purchaseDate);
}