#ifndef REPORTGENERATOR_H
#define REPORTGENERATOR_H

#include <vector>
#include <string>
#include "Component.h"

class ReportGenerator
{
public:
    /**
     * @brief Genera un reporte en formato CSV
     * @param components Vector de componentes a incluir en el reporte
     * @param filename Ruta del archivo donde guardar el reporte
     * @return true si se generó correctamente, false en caso contrario
     */
    static bool generateCSVReport(const std::vector<Component>& components, const std::string& filename);
    
    /**
     * @brief Genera un reporte en formato HTML
     * @param components Vector de componentes a incluir en el reporte
     * @param filename Ruta del archivo donde guardar el reporte
     * @return true si se generó correctamente, false en caso contrario
     */
    static bool generateHTMLReport(const std::vector<Component>& components, const std::string& filename);
    
    /**
     * @brief Genera un reporte en formato de texto plano
     * @param components Vector de componentes a incluir en el reporte
     * @param filename Ruta del archivo donde guardar el reporte
     * @return true si se generó correctamente, false en caso contrario
     */
    static bool generateTextReport(const std::vector<Component>& components, const std::string& filename);
    
    /**
     * @brief Genera un reporte específico de componentes con stock bajo
     * @param components Vector de componentes a verificar
     * @param filename Ruta del archivo donde guardar el reporte
     * @param threshold Umbral para considerar stock bajo (por defecto 5)
     * @return true si se generó correctamente, false en caso contrario
     */
    static bool generateLowStockReport(const std::vector<Component>& components, const std::string& filename, int threshold = 5);
    
private:
    /**
     * @brief Obtiene la fecha y hora actual formateada
     * @return String con la fecha y hora actual
     */
    static std::string getCurrentDateTime();
    
    /**
     * @brief Escapa comillas dobles en un string para CSV
     * @param input String a escapar
     * @return String con comillas escapadas
     */
    static std::string escapeCSV(const std::string& input);
    
    /**
     * @brief Escapa caracteres HTML especiales
     * @param input String a escapar
     * @return String con caracteres HTML escapados
     */
    static std::string escapeHTML(const std::string& input);
    
    /**
     * @brief Formatea una cantidad para resaltar stock bajo
     * @param quantity Cantidad a formatear
     * @param isLowStock Indica si es stock bajo
     * @return String formateado
     */
    static std::string formatQuantity(int quantity, bool isLowStock);
};

#endif // REPORTGENERATOR_H