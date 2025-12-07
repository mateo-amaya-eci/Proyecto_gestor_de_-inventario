#include "InventoryManager.h"

InventoryManager::InventoryManager() : dbManager(nullptr) {}

InventoryManager::InventoryManager(DatabaseManager* dbManager) 
    : dbManager(dbManager) {}

InventoryManager::~InventoryManager() {
    // No eliminamos dbManager aquí, ya que es manejado externamente
}

bool InventoryManager::addComponent(const Component& component) {
    if (!dbManager) return false;
    return dbManager->addComponent(component);
}

bool InventoryManager::updateComponent(const Component& component) {
    if (!dbManager) return false;
    return dbManager->updateComponent(component);
}

bool InventoryManager::deleteComponent(int id) {
    if (!dbManager) return false;
    return dbManager->deleteComponent(id);
}

std::vector<Component> InventoryManager::getAllComponents() {
    if (!dbManager) return {};
    return dbManager->getAllComponents();
}

std::vector<Component> InventoryManager::searchComponents(const std::string& keyword) {
    if (!dbManager) return {};
    return dbManager->searchComponents(keyword);
}

std::vector<Component> InventoryManager::getLowStockComponents(int threshold) {
    if (!dbManager) return {};
    return dbManager->getLowStockComponents(threshold);
}

void InventoryManager::setDatabaseManager(DatabaseManager* dbManager) {
    this->dbManager = dbManager;
}

DatabaseManager* InventoryManager::getDatabaseManager() const {
    return dbManager;
}