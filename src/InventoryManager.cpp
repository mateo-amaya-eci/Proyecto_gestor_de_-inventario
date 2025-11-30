#include "InventoryManager.h"
#include <algorithm>

InventoryManager::InventoryManager() : dbManager("inventory.db") {
    dbManager.openDatabase();
    loadFromDatabase();
}

bool InventoryManager::addComponent(const Component& component) {
    // Agregar a la base de datos
    if (dbManager.insertComponent(component)) {
        // Recargar desde BD para obtener el ID correcto
        loadFromDatabase();
        return true;
    }
    return false;
}

bool InventoryManager::removeComponent(int id) {
    if (dbManager.deleteComponent(id)) {
        // Eliminar del vector local
        components.erase(
            std::remove_if(components.begin(), components.end(),
                [id](const Component& comp) { return comp.getId() == id; }),
            components.end()
        );
        return true;
    }
    return false;
}

std::vector<Component> InventoryManager::getAllComponents() {
    return components;
}

std::vector<Component> InventoryManager::getLowStockComponents(int threshold) {
    std::vector<Component> lowStock;
    for (const auto& comp : components) {
        if (comp.getQuantity() <= threshold) {
            lowStock.push_back(comp);
        }
    }
    return lowStock;
}

bool InventoryManager::loadFromDatabase() {
    components = dbManager.selectAllComponents();
    return true;
}