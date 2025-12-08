#ifndef PDFGENERATOR_H
#define PDFGENERATOR_H

#include <vector>
#include <string>
#include "Component.h"

// Incluir headers de Qt para PDF
#include <QPrinter>
#include <QPainter>
#include <QFont>
#include <QFontMetrics>
#include <QPen>
#include <QBrush>
#include <QRect>
#include <QColor>
#include <QString>
#include <QStringList>
#include <QVector>
#include <QRectF>
#include <QLinearGradient>
#include <QDateTime>
#include <QPageSize>
#include <QPageLayout>
#include <QMarginsF>

class PDFGenerator
{
public:
    /**
     * @brief Genera un reporte PDF completo del inventario
     * @param components Vector de componentes a incluir
     * @param filename Ruta del archivo PDF a crear
     * @param landscape Modo apaisado (horizontal)
     * @param colorMode Modo color (true) o blanco/negro (false)
     * @return true si se generó correctamente, false en caso contrario
     */
    static bool generatePDFReport(const std::vector<Component>& components, 
                                  const std::string& filename,
                                  bool landscape = false,
                                  bool colorMode = true);
    
    /**
     * @brief Genera un reporte PDF específico de componentes con stock bajo
     * @param components Vector de componentes a incluir
     * @param filename Ruta del archivo PDF a crear
     * @param threshold Umbral para stock bajo
     * @param landscape Modo apaisado
     * @param colorMode Modo color
     * @return true si se generó correctamente, false en caso contrario
     */
    static bool generateLowStockPDF(const std::vector<Component>& components,
                                    const std::string& filename,
                                    int threshold = 5,
                                    bool landscape = false,
                                    bool colorMode = true);
    
    /**
     * @brief Genera un reporte PDF de estadísticas del inventario
     * @param components Vector de componentes
     * @param filename Ruta del archivo PDF
     * @return true si se generó correctamente, false en caso contrario
     */
    static bool generateStatisticsPDF(const std::vector<Component>& components,
                                      const std::string& filename);
    
private:
    // Métodos auxiliares privados
    static void drawHeader(QPainter& painter, const QRect& pageRect, 
                          const QString& title, const QString& subtitle,
                          bool colorMode);
    
    static void drawFooter(QPainter& painter, const QRect& pageRect,
                          int pageNumber, int totalPages,
                          bool colorMode);
    
    static int drawTable(QPainter& painter, int startY, int pageHeight,
                        const QStringList& headers, 
                        const QVector<QStringList>& rows,
                        bool colorMode);
    
    static void drawSummaryBox(QPainter& painter, int startY, int pageWidth,
                              const std::vector<Component>& components,
                              bool colorMode);
    
    static QString getCurrentDateTime();
    static QString formatQuantity(int quantity, bool isLowStock);
    
    // Métodos de utilidad
    static void drawRoundedRect(QPainter& painter, const QRect& rect,
                               int radius, const QColor& fillColor,
                               const QColor& borderColor, int borderWidth = 1);
    
    static QString truncateText(const QString& text, 
                               const QFontMetrics& metrics,
                               int maxWidth);
    
    // Colores para modo color y B/N
    static QColor getHeaderColor(bool colorMode);
    static QColor getRowColor(int rowIndex, bool isLowStock, bool colorMode);
    static QColor getTextColor(bool isLowStock, bool colorMode);
};

#endif // PDFGENERATOR_H