#include "ReportGenerator.h"
#include <fstream>
#include <iomanip>
#include <ctime>
#include <sstream>
#include <algorithm>

bool ReportGenerator::generateCSVReport(const std::vector<Component>& components, const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        return false;
    }
    
    // Encabezado del CSV
    file << "ID,Nombre,Tipo,Cantidad,Ubicación,Fecha Compra,Stock Bajo\n";
    
    // Datos
    for (const auto& component : components) {
        file << component.getId() << ","
             << "\"" << escapeCSV(component.getName()) << "\","
             << "\"" << escapeCSV(component.getType()) << "\","
             << component.getQuantity() << ","
             << "\"" << escapeCSV(component.getLocation()) << "\","
             << "\"" << component.getPurchaseDateString() << "\","
             << (component.isLowStock() ? "SI" : "NO") << "\n";
    }
    
    file.close();
    return true;
}

bool ReportGenerator::generateHTMLReport(const std::vector<Component>& components, const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        return false;
    }
    
    // Contar componentes con stock bajo
    int lowStockCount = std::count_if(components.begin(), components.end(),
                                      [](const Component& c) { return c.isLowStock(); });
    
    // Calcular valor total del inventario (asumiendo un valor promedio por tipo)
    // Esta es una implementación simple - puedes mejorarla
    int totalQuantity = 0;
    for (const auto& component : components) {
        totalQuantity += component.getQuantity();
    }
    
    // Generar HTML
    file << "<!DOCTYPE html>\n"
         << "<html lang=\"es\">\n"
         << "<head>\n"
         << "    <meta charset=\"UTF-8\">\n"
         << "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
         << "    <title>Reporte de Inventario</title>\n"
         << "    <style>\n"
         << "        * {\n"
         << "            margin: 0;\n"
         << "            padding: 0;\n"
         << "            box-sizing: border-box;\n"
         << "            font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;\n"
         << "        }\n"
         << "        body {\n"
         << "            background-color: #f5f5f5;\n"
         << "            color: #333;\n"
         << "            line-height: 1.6;\n"
         << "            padding: 20px;\n"
         << "        }\n"
         << "        .container {\n"
         << "            max-width: 1200px;\n"
         << "            margin: 0 auto;\n"
         << "            background: white;\n"
         << "            border-radius: 10px;\n"
         << "            box-shadow: 0 0 20px rgba(0,0,0,0.1);\n"
         << "            padding: 30px;\n"
         << "        }\n"
         << "        .header {\n"
         << "            text-align: center;\n"
         << "            margin-bottom: 30px;\n"
         << "            border-bottom: 3px solid #4CAF50;\n"
         << "            padding-bottom: 20px;\n"
         << "        }\n"
         << "        h1 {\n"
         << "            color: #2c3e50;\n"
         << "            font-size: 2.5em;\n"
         << "            margin-bottom: 10px;\n"
         << "        }\n"
         << "        .subtitle {\n"
         << "            color: #7f8c8d;\n"
         << "            font-size: 1.1em;\n"
         << "        }\n"
         << "        .summary {\n"
         << "            display: grid;\n"
         << "            grid-template-columns: repeat(auto-fit, minmax(200px, 1fr));\n"
         << "            gap: 20px;\n"
         << "            margin-bottom: 30px;\n"
         << "        }\n"
         << "        .summary-card {\n"
         << "            background: #f8f9fa;\n"
         << "            border-radius: 8px;\n"
         << "            padding: 20px;\n"
         << "            text-align: center;\n"
         << "            border-left: 4px solid #4CAF50;\n"
         << "        }\n"
         << "        .summary-card.warning {\n"
         << "            border-left-color: #e74c3c;\n"
         << "        }\n"
         << "        .summary-card h3 {\n"
         << "            font-size: 1.2em;\n"
         << "            color: #2c3e50;\n"
         << "            margin-bottom: 10px;\n"
         << "        }\n"
         << "        .summary-card .number {\n"
         << "            font-size: 2em;\n"
         << "            font-weight: bold;\n"
         << "            color: #4CAF50;\n"
         << "        }\n"
         << "        .summary-card.warning .number {\n"
         << "            color: #e74c3c;\n"
         << "        }\n"
         << "        table {\n"
         << "            width: 100%;\n"
         << "            border-collapse: collapse;\n"
         << "            margin-top: 20px;\n"
         << "            font-size: 0.95em;\n"
         << "        }\n"
         << "        th {\n"
         << "            background-color: #4CAF50;\n"
         << "            color: white;\n"
         << "            padding: 12px 15px;\n"
         << "            text-align: left;\n"
         << "            font-weight: 600;\n"
         << "        }\n"
         << "        td {\n"
         << "            padding: 10px 15px;\n"
         << "            border-bottom: 1px solid #ddd;\n"
         << "        }\n"
         << "        tr:hover {\n"
         << "            background-color: #f5f5f5;\n"
         << "        }\n"
         << "        .low-stock {\n"
         << "            background-color: #ffeaea !important;\n"
         << "            font-weight: bold;\n"
         << "        }\n"
         << "        .quantity-low {\n"
         << "            color: #e74c3c;\n"
         << "            font-weight: bold;\n"
         << "        }\n"
         << "        .footer {\n"
         << "            margin-top: 40px;\n"
         << "            text-align: center;\n"
         << "            color: #7f8c8d;\n"
         << "            font-size: 0.9em;\n"
         << "            border-top: 1px solid #eee;\n"
         << "            padding-top: 20px;\n"
         << "        }\n"
         << "        @media print {\n"
         << "            body {\n"
         << "                background: white;\n"
                 << "            }\n"
                 << "            .container {\n"
                 << "                box-shadow: none;\n"
                 << "            }\n"
                 << "        }\n"
                 << "    </style>\n"
                 << "</head>\n"
                 << "<body>\n"
                 << "    <div class=\"container\">\n"
                 << "        <div class=\"header\">\n"
                 << "            <h1>📦 Reporte de Inventario</h1>\n"
                 << "            <p class=\"subtitle\">Sistema de Gestión para Hogar/Laboratorio</p>\n"
                 << "            <p>Generado: " << getCurrentDateTime() << "</p>\n"
                 << "        </div>\n"
                 << "        \n"
                 << "        <div class=\"summary\">\n"
                 << "            <div class=\"summary-card\">\n"
                 << "                <h3>Total de Componentes</h3>\n"
                 << "                <div class=\"number\">" << components.size() << "</div>\n"
                 << "            </div>\n"
                 << "            <div class=\"summary-card\">\n"
                 << "                <h3>Cantidad Total</h3>\n"
                 << "                <div class=\"number\">" << totalQuantity << "</div>\n"
                 << "            </div>\n"
                 << "            <div class=\"summary-card" << (lowStockCount > 0 ? " warning" : "") << "\">\n"
                 << "                <h3>Componentes con Stock Bajo</h3>\n"
                 << "                <div class=\"number\">" << lowStockCount << "</div>\n"
                 << "            </div>\n"
                 << "        </div>\n"
                 << "        \n"
                 << "        <h2>📋 Lista de Componentes</h2>\n"
                 << "        <table>\n"
                 << "            <thead>\n"
                 << "                <tr>\n"
                 << "                    <th>ID</th>\n"
                 << "                    <th>Nombre</th>\n"
                 << "                    <th>Tipo</th>\n"
                 << "                    <th>Cantidad</th>\n"
                 << "                    <th>Ubicación</th>\n"
                 << "                    <th>Fecha Compra</th>\n"
                 << "                    <th>Estado</th>\n"
                 << "                </tr>\n"
                 << "            </thead>\n"
                 << "            <tbody>\n";
    
    // Filas de datos
    for (const auto& component : components) {
        bool isLowStock = component.isLowStock();
        std::string rowClass = isLowStock ? " class=\"low-stock\"" : "";
        
        file << "                <tr" << rowClass << ">\n"
             << "                    <td>" << component.getId() << "</td>\n"
             << "                    <td>" << escapeHTML(component.getName()) << "</td>\n"
             << "                    <td>" << escapeHTML(component.getType()) << "</td>\n"
             << "                    <td class=\"" << (isLowStock ? "quantity-low" : "") << "\">"
             << component.getQuantity() << "</td>\n"
             << "                    <td>" << escapeHTML(component.getLocation()) << "</td>\n"
             << "                    <td>" << component.getPurchaseDateString() << "</td>\n"
             << "                    <td>" << (isLowStock ? "⚠️ STOCK BAJO" : "✅ OK") << "</td>\n"
             << "                </tr>\n";
    }
    
    file << "            </tbody>\n"
         << "        </table>\n"
         << "        \n"
         << "        <div class=\"footer\">\n"
         << "            <p>Sistema desarrollado en C++ con Qt y SQLite</p>\n"
         << "            <p>© 2024 - Gestor de Inventario para Hogar/Laboratorio</p>\n"
         << "            <p>Reporte generado automáticamente</p>\n"
         << "        </div>\n"
         << "    </div>\n"
         << "</body>\n"
         << "</html>\n";
    
    file.close();
    return true;
}

bool ReportGenerator::generateTextReport(const std::vector<Component>& components, const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        return false;
    }
    
    file << "=========================================\n";
    file << "      REPORTE DE INVENTARIO\n";
    file << "=========================================\n";
    file << "Generado: " << getCurrentDateTime() << "\n";
    file << "Total de componentes: " << components.size() << "\n";
    file << "=========================================\n\n";
    
    for (const auto& component : components) {
        file << "ID: " << component.getId() << "\n";
        file << "Nombre: " << component.getName() << "\n";
        file << "Tipo: " << component.getType() << "\n";
        file << "Cantidad: " << component.getQuantity();
        
        if (component.isLowStock()) {
            file << " [STOCK BAJO!]";
        }
        file << "\n";
        
        file << "Ubicación: " << component.getLocation() << "\n";
        file << "Fecha Compra: " << component.getPurchaseDateString() << "\n";
        file << "-----------------------------------------\n";
    }
    
    file.close();
    return true;
}

bool ReportGenerator::generateLowStockReport(const std::vector<Component>& components, const std::string& filename, int threshold) {
    // Filtrar componentes con stock bajo
    std::vector<Component> lowStockComponents;
    std::copy_if(components.begin(), components.end(),
                 std::back_inserter(lowStockComponents),
                 [threshold](const Component& c) { return c.getQuantity() <= threshold; });
    
    if (lowStockComponents.empty()) {
        // Si no hay componentes con stock bajo, crear un reporte indicándolo
        std::ofstream file(filename);
        if (!file.is_open()) return false;
        
        file << "REPORTE DE STOCK BAJO\n";
        file << "=====================\n";
        file << "Fecha: " << getCurrentDateTime() << "\n";
        file << "Umbral: " << threshold << " unidades\n";
        file << "=====================\n\n";
        file << "✅ No hay componentes con stock bajo.\n";
        file << "Todo el inventario está en niveles adecuados.\n";
        
        file.close();
        return true;
    }
    
    // Generar reporte HTML específico para stock bajo
    std::ofstream file(filename);
    if (!file.is_open()) {
        return false;
    }
    
    file << "<!DOCTYPE html>\n"
         << "<html lang=\"es\">\n"
         << "<head>\n"
         << "    <meta charset=\"UTF-8\">\n"
         << "    <title>Alerta: Stock Bajo</title>\n"
         << "    <style>\n"
         << "        body { font-family: Arial, sans-serif; margin: 40px; }\n"
         << "        .alert { background-color: #fff3cd; border: 1px solid #ffeaa7; padding: 20px; margin: 20px 0; }\n"
         << "        .warning { color: #856404; }\n"
         << "        table { border-collapse: collapse; width: 100%; margin-top: 20px; }\n"
         << "        th, td { border: 1px solid #ddd; padding: 8px; text-align: left; }\n"
         << "        th { background-color: #dc3545; color: white; }\n"
         << "        tr:nth-child(even) { background-color: #f8d7da; }\n"
         << "    </style>\n"
         << "</head>\n"
         << "<body>\n"
         << "    <h1 class=\"warning\">⚠️ ALERTA: COMPONENTES CON STOCK BAJO</h1>\n"
         << "    <div class=\"alert\">\n"
         << "        <p><strong>Fecha:</strong> " << getCurrentDateTime() << "</p>\n"
         << "        <p><strong>Umbral de stock bajo:</strong> " << threshold << " unidades</p>\n"
         << "        <p><strong>Total de componentes afectados:</strong> " << lowStockComponents.size() << "</p>\n"
         << "    </div>\n"
         << "    \n"
         << "    <table>\n"
         << "        <tr>\n"
         << "            <th>ID</th>\n"
         << "            <th>Nombre</th>\n"
         << "            <th>Tipo</th>\n"
         << "            <th>Cantidad Actual</th>\n"
         << "            <th>Ubicación</th>\n"
         << "            <th>Acción Recomendada</th>\n"
         << "        </tr>\n";
    
    for (const auto& component : lowStockComponents) {
        file << "        <tr>\n"
             << "            <td>" << component.getId() << "</td>\n"
             << "            <td>" << escapeHTML(component.getName()) << "</td>\n"
             << "            <td>" << escapeHTML(component.getType()) << "</td>\n"
             << "            <td><strong>" << component.getQuantity() << "</strong> unidades</td>\n"
             << "            <td>" << escapeHTML(component.getLocation()) << "</td>\n"
             << "            <td>REABASTECER URGENTE</td>\n"
             << "        </tr>\n";
    }
    
    file << "    </table>\n"
         << "    \n"
         << "    <div style=\"margin-top: 30px; padding: 15px; background-color: #d4edda; border: 1px solid #c3e6cb;\">\n"
         << "        <h3>Recomendaciones:</h3>\n"
         << "        <ul>\n"
         << "            <li>Verificar inventario de estos componentes</li>\n"
         << "            <li>Realizar pedido de reposición</li>\n"
         << "            <li>Actualizar niveles mínimos de stock si es necesario</li>\n"
         << "        </ul>\n"
         << "    </div>\n"
         << "</body>\n"
         << "</html>\n";
    
    file.close();
    return true;
}

// Métodos privados
std::string ReportGenerator::getCurrentDateTime() {
    std::time_t now = std::time(nullptr);
    std::tm* timeinfo = std::localtime(&now);
    char buffer[80];
    std::strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", timeinfo);
    return std::string(buffer);
}

std::string ReportGenerator::escapeCSV(const std::string& input) {
    std::string output = input;
    
    // Reemplazar comillas dobles por dos comillas dobles (escapado CSV)
    size_t pos = 0;
    while ((pos = output.find("\"", pos)) != std::string::npos) {
        output.replace(pos, 1, "\"\"");
        pos += 2;
    }
    
    return output;
}

std::string ReportGenerator::escapeHTML(const std::string& input) {
    std::string output = input;
    
    // Reemplazar caracteres especiales HTML
    size_t pos = 0;
    while ((pos = output.find("&", pos)) != std::string::npos) {
        output.replace(pos, 1, "&amp;");
        pos += 5;
    }
    
    pos = 0;
    while ((pos = output.find("<", pos)) != std::string::npos) {
        output.replace(pos, 1, "&lt;");
        pos += 4;
    }
    
    pos = 0;
    while ((pos = output.find(">", pos)) != std::string::npos) {
        output.replace(pos, 1, "&gt;");
        pos += 4;
    }
    
    pos = 0;
    while ((pos = output.find("\"", pos)) != std::string::npos) {
        output.replace(pos, 1, "&quot;");
        pos += 6;
    }
    
    return output;
}