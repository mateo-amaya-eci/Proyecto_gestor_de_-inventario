#include "DatabaseManager.h"
#include <iostream>
#include <algorithm>
#include <cstring>
#include <iomanip> 

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
    
    // Ser explícito sobre el orden de las columnas
    std::string sql = "SELECT id, name, type, quantity, location, purchase_date FROM components ORDER BY name";
    
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
void DatabaseManager::debugTableInfo() {
    if (!isConnected()) {
        std::cout << "DEBUG: No hay conexión a la base de datos" << std::endl;
        return;
    }
    
    std::cout << "\n=== DEBUG: Información de la tabla components ===" << std::endl;
    
    // 1. Ver estructura con PRAGMA table_info
    std::cout << "\n1. Estructura de la tabla (PRAGMA table_info):" << std::endl;
    std::string sql = "PRAGMA table_info(components);";
    sqlite3_stmt* stmt;
    
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        std::cout << std::left << std::setw(5) << "CID" 
                  << std::setw(15) << "Nombre" 
                  << std::setw(15) << "Tipo" 
                  << std::setw(8) << "NotNULL" 
                  << std::setw(10) << "Default" 
                  << "PK" << std::endl;
        std::cout << std::string(60, '-') << std::endl;
        
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int cid = sqlite3_column_int(stmt, 0);
            const char* name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            const char* type = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
            int notnull = sqlite3_column_int(stmt, 3);
            const char* dflt_value = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
            int pk = sqlite3_column_int(stmt, 5);
            
            std::cout << std::left << std::setw(5) << cid
                      << std::setw(15) << (name ? name : "NULL")
                      << std::setw(15) << (type ? type : "NULL")
                      << std::setw(8) << notnull
                      << std::setw(10) << (dflt_value ? dflt_value : "NULL")
                      << pk << std::endl;
        }
        sqlite3_finalize(stmt);
    } else {
        std::cerr << "Error al obtener estructura de la tabla: " << sqlite3_errmsg(db) << std::endl;
    }
    
    // 2. Ver esquema completo
    std::cout << "\n2. Esquema completo de la tabla:" << std::endl;
    sql = "SELECT sql FROM sqlite_master WHERE type='table' AND name='components';";
    
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            const char* createSql = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            if (createSql) {
                std::cout << createSql << std::endl;
            }
        }
        sqlite3_finalize(stmt);
    }
    
    // 3. Ver nombres de columnas y datos
    std::cout << "\n3. Datos actuales en la tabla:" << std::endl;
    sql = "SELECT * FROM components;";
    
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        int colCount = sqlite3_column_count(stmt);
        std::cout << "Número de columnas: " << colCount << std::endl;
        
        // Encabezados de columnas
        std::cout << "\nNombres de columnas:" << std::endl;
        for (int i = 0; i < colCount; i++) {
            const char* colName = sqlite3_column_name(stmt, i);
            std::cout << "  [" << i << "] " << (colName ? colName : "NULL") << std::endl;
        }
        
        // Contar filas
        int rowCount = 0;
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            rowCount++;
        }
        
        std::cout << "\nNúmero de filas: " << rowCount << std::endl;
        
        // Reiniciar para leer datos
        sqlite3_reset(stmt);
        
        // Mostrar datos
        if (rowCount > 0) {
            std::cout << "\nContenido de la tabla:" << std::endl;
            std::cout << std::string(80, '-') << std::endl;
            
            // Encabezados
            for (int i = 0; i < colCount; i++) {
                const char* colName = sqlite3_column_name(stmt, i);
                std::cout << std::left << std::setw(20) << (colName ? colName : "NULL");
            }
            std::cout << std::endl << std::string(80, '-') << std::endl;
            
            // Datos
            while (sqlite3_step(stmt) == SQLITE_ROW) {
                for (int i = 0; i < colCount; i++) {
                    const char* value = reinterpret_cast<const char*>(sqlite3_column_text(stmt, i));
                    std::cout << std::left << std::setw(20) << (value ? value : "NULL");
                }
                std::cout << std::endl;
            }
        }
        
        sqlite3_finalize(stmt);
    }
    
    // 4. Ver índices
    std::cout << "\n4. Índices en la tabla:" << std::endl;
    sql = "SELECT name, sql FROM sqlite_master WHERE type='index' AND tbl_name='components';";
    
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        bool hasIndexes = false;
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            hasIndexes = true;
            const char* indexName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            const char* indexSql = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            
            std::cout << "  Índice: " << (indexName ? indexName : "NULL") << std::endl;
            std::cout << "  SQL: " << (indexSql ? indexSql : "NULL") << std::endl;
        }
        
        if (!hasIndexes) {
            std::cout << "  No hay índices definidos" << std::endl;
        }
        
        sqlite3_finalize(stmt);
    }
    
    std::cout << "=== FIN DEBUG ===\n" << std::endl;
}
bool DatabaseManager::recreateTable() {
    if (!isConnected()) return false;
    
    std::cout << "\n=== RECREANDO TABLA COMPONENTS ===" << std::endl;
    
    // 1. Hacer backup de datos existentes si los hay
    std::vector<Component> backup;
    std::string backupSql = "SELECT * FROM components;";
    sqlite3_stmt* stmt;
    
    if (sqlite3_prepare_v2(db, backupSql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            backup.push_back(createComponentFromRow(stmt));
        }
        sqlite3_finalize(stmt);
        std::cout << "Backup realizado: " << backup.size() << " componentes" << std::endl;
    }
    
    // 2. Eliminar tabla existente
    if (!executeQuery("DROP TABLE IF EXISTS components;")) {
        std::cerr << "Error al eliminar tabla" << std::endl;
        return false;
    }
    
    // 3. Eliminar índices
    executeQuery("DROP INDEX IF EXISTS idx_name;");
    executeQuery("DROP INDEX IF EXISTS idx_type;");
    executeQuery("DROP INDEX IF EXISTS idx_location;");
    
    // 4. Crear tabla con orden explícito
    std::string createTableSQL = R"(
        CREATE TABLE components (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT NOT NULL,
            type TEXT NOT NULL,
            quantity INTEGER NOT NULL DEFAULT 0,
            location TEXT,
            purchase_date INTEGER
        );
    )";
    
    if (!executeQuery(createTableSQL)) {
        std::cerr << "Error al crear tabla" << std::endl;
        return false;
    }
    
    // 5. Crear índices
    executeQuery("CREATE INDEX idx_name ON components(name);");
    executeQuery("CREATE INDEX idx_type ON components(type);");
    executeQuery("CREATE INDEX idx_location ON components(location);");
    
    std::cout << "Tabla recreada exitosamente" << std::endl;
    
    // 6. Restaurar datos si los había
    if (!backup.empty()) {
        std::cout << "Restaurando " << backup.size() << " componentes..." << std::endl;
        for (const auto& component : backup) {
            addComponent(component);
        }
        std::cout << "Datos restaurados exitosamente" << std::endl;
    }
    
    // 7. Verificar nueva estructura
    debugTableInfo();
    
    return true;
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