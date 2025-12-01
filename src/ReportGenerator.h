#ifndef REPORTGENERATOR_H
#define REPORTGENERATOR_H

#include "Component.h"
#include <vector>
#include <string>

/**
 * @class ReportGenerator
 * @brief Genera diferentes tipos de reportes para los componentes del inventario.
 * 
 * La clase ReportGenerator proporciona métodos estáticos para generar reportes en formatos CSV, HTML,
 * y reportes específicos de bajo stock.
 */
class ReportGenerator {
public:
    /**
     * @brief Genera un reporte en formato CSV con los componentes proporcionados.
     * 
     * @param components Vector de componentes a incluir en el reporte.
     * @param filename Nombre del archivo donde se guardará el reporte.
     * @return true si el reporte se genera correctamente, false en caso contrario.
     */
    static bool generateCSVReport(const std::vector<Component>& components, 
                                 const std::string& filename);

    /**
     * @brief Genera un reporte en formato HTML con los componentes proporcionados.
     * 
     * @param components Vector de componentes a incluir en el reporte.
     * @param filename Nombre del archivo donde se guardará el reporte.
     * @return true si el reporte se genera correctamente, false en caso contrario.
     */
    static bool generateHTMLReport(const std::vector<Component>& components,
                                  const std::string& filename);

    /**
     * @brief Genera un reporte de componentes con bajo stock en formato CSV.
     * 
     * @param components Vector de componentes a analizar.
     * @param filename Nombre del archivo donde se guardará el reporte.
     * @param threshold Umbral de stock bajo (por defecto es 5).
     * @return true si el reporte se genera correctamente, false en caso contrario.
     */
    static bool generateLowStockReport(const std::vector<Component>& components,
                                      const std::string& filename, int threshold = 5);
};

#endif