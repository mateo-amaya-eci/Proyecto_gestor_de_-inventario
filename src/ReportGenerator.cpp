#include "ReportGenerator.h"
#include <fstream>
#include <sstream>

bool ReportGenerator::generateCSVReport(const std::vector<Component>& components, 
                                       const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        return false;
    }
    
    // Encabezado CSV
    file << "ID,Nombre,Tipo,Cantidad,Ubicacion,Fecha_Compra\n";
    
    // Datos
    for (const auto& comp : components) {
        file << comp.getId() << ","
             << comp.getName() << ","
             << comp.getType() << ","
             << comp.getQuantity() << ","
             << comp.getLocation() << ","
             << comp.getPurchaseDate() << "\n";
    }
    
    file.close();
    return true;
}