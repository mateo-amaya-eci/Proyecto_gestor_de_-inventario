#include "PDFGenerator.h"
#include <algorithm>
#include <map>
#include <cmath>
#include <numeric>

bool PDFGenerator::generatePDFReport(const std::vector<Component>& components, 
                                     const std::string& filename,
                                     bool landscape,
                                     bool colorMode) {
    if (components.empty()) {
        return false;
    }
    
    // Configurar la impresora para PDF
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(QString::fromStdString(filename));
    
    // Configurar tamaño y orientación de página
    printer.setPageSize(QPageSize(QPageSize::A4));
    printer.setPageOrientation(landscape ? QPageLayout::Landscape : QPageLayout::Portrait);
    
    // Configurar márgenes (en milímetros)
    printer.setPageMargins(QMarginsF(15, 15, 15, 15), QPageLayout::Millimeter);
    
    // Iniciar el pintor
    QPainter painter;
    if (!painter.begin(&printer)) {
        return false;
    }
    
    // Obtener dimensiones de la página
    QRect pageRect = printer.pageRect(QPrinter::DevicePixel).toRect();
    int pageWidth = pageRect.width();
    int pageHeight = pageRect.height();
    
    // Configurar fuentes
    QFont titleFont("Arial", 16, QFont::Bold);
    QFont headerFont("Arial", 10, QFont::Bold);
    QFont normalFont("Arial", 9);
    QFont smallFont("Arial", 8);
    
    // ===== DIBUJAR ENCABEZADO =====
    drawHeader(painter, pageRect, 
               "REPORTE DE INVENTARIO COMPLETO",
               "Sistema de Gestión para Hogar/Laboratorio",
               colorMode);
    
    int currentY = 100; // Posición después del encabezado
    
    // ===== DIBUJAR RESUMEN ESTADÍSTICO =====
    drawSummaryBox(painter, currentY, pageWidth, components, colorMode);
    currentY += 150; // Espacio después del resumen
    
    // ===== PREPARAR DATOS PARA LA TABLA =====
    QStringList headers = {"ID", "Nombre", "Tipo", "Cantidad", "Ubicación", "Fecha Compra"};
    QVector<QStringList> rows;
    
    for (const auto& component : components) {
        QStringList row;
        row << QString::number(component.getId());
        row << QString::fromStdString(component.getName());
        row << QString::fromStdString(component.getType());
        row << formatQuantity(component.getQuantity(), component.isLowStock());
        row << QString::fromStdString(component.getLocation());
        row << QString::fromStdString(component.getPurchaseDateString());
        rows.append(row);
    }
    
    // ===== DIBUJAR TABLA (CON MANEJO DE MÚLTIPLES PÁGINAS) =====
    int rowsPerPage = landscape ? 25 : 15; // Más filas en modo apaisado
    int totalRows = rows.size();
    int totalPages = std::ceil(static_cast<double>(totalRows) / rowsPerPage);
    int currentPage = 1;
    int startRow = 0;
    
    while (startRow < totalRows) {
        if (currentPage > 1) {
            // Nueva página para más datos
            printer.newPage();
            drawHeader(painter, pageRect,
                      "REPORTE DE INVENTARIO (Continuación)",
                      QString("Página %1").arg(currentPage),
                      colorMode);
            currentY = 100;
        }
        
        // Título de la tabla
        painter.setFont(headerFont);
        painter.setPen(colorMode ? QColor(50, 50, 50) : Qt::black);
        painter.drawText(50, currentY, "LISTA DE COMPONENTES");
        currentY += 30;
        
        // Obtener filas para esta página
        int endRow = std::min(startRow + rowsPerPage, totalRows);
        QVector<QStringList> pageRows;
        for (int i = startRow; i < endRow; i++) {
            pageRows.append(rows[i]);
        }
        
        // Dibujar tabla
        currentY = drawTable(painter, currentY, pageHeight, headers, pageRows, colorMode);
        
        // Dibujar pie de página
        drawFooter(painter, pageRect, currentPage, totalPages, colorMode);
        
        // Preparar para siguiente página
        startRow = endRow;
        currentPage++;
        currentY = 100; // Reset para nueva página
    }
    
    painter.end();
    return true;
}

bool PDFGenerator::generateLowStockPDF(const std::vector<Component>& components,
                                       const std::string& filename,
                                       int threshold,
                                       bool landscape,
                                       bool colorMode) {
    // Filtrar componentes con stock bajo
    std::vector<Component> lowStockComponents;
    std::copy_if(components.begin(), components.end(),
                 std::back_inserter(lowStockComponents),
                 [threshold](const Component& c) { 
                     return c.getQuantity() <= threshold; 
                 });
    
    if (lowStockComponents.empty()) {
        // Si no hay stock bajo, generar reporte normal con nota
        return generatePDFReport(components, filename, landscape, colorMode);
    }
    
    // Configurar impresora
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(QString::fromStdString(filename));
    printer.setPageSize(QPageSize(QPageSize::A4));
    printer.setPageOrientation(landscape ? QPageLayout::Landscape : QPageLayout::Portrait);
    printer.setPageMargins(QMarginsF(15, 15, 15, 15), QPageLayout::Millimeter);
    
    QPainter painter;
    if (!painter.begin(&printer)) {
        return false;
    }
    
    QRect pageRect = printer.pageRect(QPrinter::DevicePixel).toRect();
    int pageWidth = pageRect.width();
    int pageHeight = pageRect.height();
    
    // ===== ENCABEZADO DE ALERTA =====
    QFont titleFont("Arial", 18, QFont::Bold);
    QFont headerFont("Arial", 12, QFont::Bold);
    QFont normalFont("Arial", 10);
    
    // Fondo de alerta
    QRect alertRect(0, 0, pageWidth, 120);
    QLinearGradient gradient(0, 0, 0, 120);
    if (colorMode) {
        gradient.setColorAt(0, QColor(231, 76, 60));
        gradient.setColorAt(1, QColor(192, 57, 43));
    } else {
        gradient.setColorAt(0, Qt::lightGray);
        gradient.setColorAt(1, Qt::darkGray);
    }
    painter.fillRect(alertRect, gradient);
    
    // Título de alerta
    painter.setFont(titleFont);
    painter.setPen(Qt::white);
    painter.drawText(alertRect, Qt::AlignCenter, "⚠️  ALERTA: STOCK BAJO  ⚠️");
    
    // Subtítulo
    painter.setFont(normalFont);
    painter.drawText(50, 90, QString("Umbral: %1 unidades | Componentes afectados: %2 | Generado: %3")
                     .arg(threshold)
                     .arg(lowStockComponents.size())
                     .arg(getCurrentDateTime()));
    
    painter.setPen(colorMode ? QColor(50, 50, 50) : Qt::black);
    
    // ===== RESUMEN URGENTE =====
    int currentY = 140;
    painter.setFont(headerFont);
    painter.drawText(50, currentY, "ACCIONES RECOMENDADAS:");
    currentY += 30;
    
    painter.setFont(normalFont);
    QStringList recommendations = {
        "1. Verificar niveles de inventario de inmediato",
        "2. Realizar pedido de reposición urgente",
        "3. Revisar políticas de reorden",
        "4. Notificar al responsable de compras"
    };
    
    for (const QString& rec : recommendations) {
        painter.drawText(70, currentY, rec);
        currentY += 25;
    }
    
    currentY += 20;
    
    // ===== TABLA DE COMPONENTES CON STOCK BAJO =====
    painter.setFont(headerFont);
    painter.drawText(50, currentY, "COMPONENTES AFECTADOS:");
    currentY += 30;
    
    QStringList alertHeaders = {"ID", "Nombre", "Tipo", "Cantidad", "Ubicación", "Estado"};
    QVector<QStringList> alertRows;
    
    for (const auto& component : lowStockComponents) {
        QStringList row;
        row << QString::number(component.getId());
        row << QString::fromStdString(component.getName());
        row << QString::fromStdString(component.getType());
        row << QString::number(component.getQuantity());
        row << QString::fromStdString(component.getLocation());
        row << "⚠️ URGENTE";
        alertRows.append(row);
    }
    
    drawTable(painter, currentY, pageHeight, alertHeaders, alertRows, colorMode);
    
    // ===== PIE DE PÁGINA =====
    drawFooter(painter, pageRect, 1, 1, colorMode);
    
    painter.end();
    return true;
}

// ===== MÉTODOS AUXILIARES PRIVADOS =====

void PDFGenerator::drawHeader(QPainter& painter, const QRect& pageRect, 
                             const QString& title, const QString& subtitle,
                             bool colorMode) {
    int pageWidth = pageRect.width();
    
    // Fondo del encabezado
    QRect headerRect(0, 0, pageWidth, 80);
    QLinearGradient gradient(0, 0, 0, 80);
    
    if (colorMode) {
        gradient.setColorAt(0, getHeaderColor(true));
        gradient.setColorAt(1, QColor(41, 128, 185));
    } else {
        gradient.setColorAt(0, Qt::lightGray);
        gradient.setColorAt(1, Qt::darkGray);
    }
    
    painter.fillRect(headerRect, gradient);
    
    // Título
    QFont titleFont("Arial", 14, QFont::Bold);
    painter.setFont(titleFont);
    painter.setPen(Qt::white);
    
    QFontMetrics titleMetrics(titleFont);
    int titleWidth = titleMetrics.horizontalAdvance(title);
    painter.drawText((pageWidth - titleWidth) / 2, 35, title);
    
    // Subtítulo
    QFont subFont("Arial", 9);
    painter.setFont(subFont);
    
    QFontMetrics subMetrics(subFont);
    int subWidth = subMetrics.horizontalAdvance(subtitle);
    painter.drawText((pageWidth - subWidth) / 2, 55, subtitle);
    
    // Información
    painter.setFont(QFont("Arial", 8));
    painter.drawText(50, 70, "Generado: " + getCurrentDateTime());
    painter.drawText(pageWidth - 200, 70, "Sistema de Gestión de Inventario");
    
    // Restaurar color negro para el contenido
    painter.setPen(colorMode ? QColor(50, 50, 50) : Qt::black);
}

void PDFGenerator::drawFooter(QPainter& painter, const QRect& pageRect,
                             int pageNumber, int totalPages,
                             bool colorMode) {
    int pageWidth = pageRect.width();
    int pageHeight = pageRect.height();
    
    // Línea separadora
    painter.setPen(QPen(colorMode ? QColor(200, 200, 200) : Qt::gray, 1));
    painter.drawLine(50, pageHeight - 40, pageWidth - 50, pageHeight - 40);
    
    // Número de página
    painter.setFont(QFont("Arial", 9));
    painter.setPen(colorMode ? QColor(100, 100, 100) : Qt::darkGray);
    
    QString pageInfo = QString("Página %1 de %2").arg(pageNumber).arg(totalPages);
    QFontMetrics fm(painter.font());
    int textWidth = fm.horizontalAdvance(pageInfo);
    
    painter.drawText((pageWidth - textWidth) / 2, pageHeight - 20, pageInfo);
    
    // Información del sistema
    painter.drawText(50, pageHeight - 20, "© 2024 - Gestor de Inventario");
    painter.drawText(pageWidth - 250, pageHeight - 20, "Documento de carácter informativo");
}

int PDFGenerator::drawTable(QPainter& painter, int startY, int pageHeight,
                           const QStringList& headers, 
                           const QVector<QStringList>& rows,
                           bool colorMode) {
    int pageWidth = painter.window().width();
    int tableWidth = pageWidth - 100;
    int columnCount = headers.size();
    
    // Calcular anchos de columnas
    QVector<int> columnWidths;
    if (columnCount == 6) {
        columnWidths << 50 << 200 << 100 << 80 << 150 << 100;
    } else {
        int colWidth = (tableWidth - (columnCount * 5)) / columnCount;
        for (int i = 0; i < columnCount; i++) {
            columnWidths << colWidth;
        }
    }
    
    // ENCABEZADO DE LA TABLA
    int currentX = 50;
    int rowHeight = 25;
    int headerHeight = 30;
    
    // Fondo del encabezado
    QRect headerRect(50, startY, tableWidth, headerHeight);
    drawRoundedRect(painter, headerRect, 5, 
                   getHeaderColor(colorMode),
                   colorMode ? QColor(41, 128, 185) : Qt::darkGray);
    
    // Texto del encabezado
    painter.setFont(QFont("Arial", 10, QFont::Bold));
    painter.setPen(Qt::white);
    
    currentX = 55;
    for (int i = 0; i < columnCount; i++) {
        QString headerText = truncateText(headers[i], QFontMetrics(painter.font()), 
                                         columnWidths[i] - 10);
        painter.drawText(currentX, startY + 20, headerText);
        currentX += columnWidths[i] + 5;
    }
    
    // FILAS DE DATOS
    painter.setFont(QFont("Arial", 9));
    
    int currentRowY = startY + headerHeight + 5;
    int rowIndex = 0;
    
    for (const QStringList& row : rows) {
        // Determinar si es stock bajo (buscar símbolo ⚠️ o cantidad baja)
        bool isLowStock = false;
        if (row.size() > 3) {
            QString quantityStr = row[3];
            isLowStock = quantityStr.contains("⚠️") || 
                        quantityStr.contains("BAJO") ||
                        quantityStr.contains("URGENTE");
        }
        
        // Color de fondo de la fila
        QColor rowColor = getRowColor(rowIndex, isLowStock, colorMode);
        painter.fillRect(50, currentRowY, tableWidth, rowHeight, rowColor);
        
        // Borde de la fila
        painter.setPen(QPen(colorMode ? QColor(220, 220, 220) : Qt::gray, 1));
        painter.drawRect(50, currentRowY, tableWidth, rowHeight);
        
        // Texto de la fila
        painter.setPen(getTextColor(isLowStock, colorMode));
        currentX = 55;
        
        for (int colIdx = 0; colIdx < std::min(row.size(), columnCount); colIdx++) {
            QString cellText = truncateText(row[colIdx], QFontMetrics(painter.font()), 
                                           columnWidths[colIdx] - 10);
            painter.drawText(currentX, currentRowY + 18, cellText);
            currentX += columnWidths[colIdx] + 5;
        }
        
        currentRowY += rowHeight;
        rowIndex++;
        
        // Verificar si queda espacio en la página
        if (currentRowY + rowHeight > pageHeight - 60) {
            break; // Dejar para siguiente página
        }
    }
    
    return currentRowY + 20;
}

void PDFGenerator::drawSummaryBox(QPainter& painter, int startY, int pageWidth,
                                 const std::vector<Component>& components,
                                 bool colorMode) {
    // Calcular estadísticas
    int totalComponents = components.size();
    int totalQuantity = 0;
    int lowStockCount = 0;
    std::map<std::string, int> typeDistribution;
    
    for (const auto& comp : components) {
        totalQuantity += comp.getQuantity();
        if (comp.isLowStock()) lowStockCount++;
        typeDistribution[comp.getType()]++;
    }
    
    // Dibujar recuadro de resumen
    QRect summaryRect(50, startY, pageWidth - 100, 130);
    drawRoundedRect(painter, summaryRect, 10,
                   colorMode ? QColor(240, 248, 255) : QColor(250, 250, 250),
                   colorMode ? QColor(200, 220, 240) : Qt::gray);
    
    // Título del resumen
    painter.setFont(QFont("Arial", 12, QFont::Bold));
    painter.setPen(colorMode ? QColor(50, 50, 50) : Qt::black);
    painter.drawText(70, startY + 25, "RESUMEN ESTADÍSTICO");
    
    // Estadísticas
    painter.setFont(QFont("Arial", 10));
    
    int textY = startY + 50;
    painter.drawText(70, textY, QString("• Total de componentes: %1").arg(totalComponents));
    painter.drawText(70, textY + 20, QString("• Cantidad total: %1 unidades").arg(totalQuantity));
    
    QColor lowStockColor = colorMode ? QColor(231, 76, 60) : Qt::red;
    painter.setPen(lowStockCount > 0 ? lowStockColor : painter.pen().color());
    painter.drawText(70, textY + 40, QString("• Componentes con stock bajo: %1").arg(lowStockCount));
    
    // Restaurar color
    painter.setPen(colorMode ? QColor(50, 50, 50) : Qt::black);
    
    // Distribución por tipos
    QString typesText = "• Distribución por tipos: ";
    int typeCount = 0;
    for (const auto& [type, count] : typeDistribution) {
        if (typeCount > 0) typesText += ", ";
        typesText += QString("%1 (%2)").arg(QString::fromStdString(type)).arg(count);
        typeCount++;
        if (typeCount >= 4) { // Limitar a 4 tipos por línea
            typesText += "...";
            break;
        }
    }
    
    // Dibujar distribución (con manejo de texto largo)
    QFontMetrics fm(painter.font());
    int maxTextWidth = pageWidth - 140;
    
    if (fm.horizontalAdvance(typesText) <= maxTextWidth) {
        painter.drawText(70, textY + 60, typesText);
    } else {
        // Dividir en dos líneas
        QString line1 = "• Distribución por tipos: ";
        QString line2 = "";
        
        for (const auto& [type, count] : typeDistribution) {
            QString typeStr = QString("%1 (%2)").arg(QString::fromStdString(type)).arg(count);
            if (fm.horizontalAdvance(line1 + typeStr + ", ") <= maxTextWidth) {
                line1 += typeStr + ", ";
            } else {
                line2 += typeStr + ", ";
            }
        }
        
        // Remover última coma y espacio
        if (line1.endsWith(", ")) line1.chop(2);
        if (line2.endsWith(", ")) line2.chop(2);
        
        painter.drawText(70, textY + 60, line1);
        if (!line2.isEmpty()) {
            painter.drawText(70, textY + 80, line2);
        }
    }
}

// ===== MÉTODOS DE UTILIDAD =====

QString PDFGenerator::getCurrentDateTime() {
    return QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm:ss");
}

QString PDFGenerator::formatQuantity(int quantity, bool isLowStock) {
    if (isLowStock) {
        return QString("%1 ⚠️").arg(quantity);
    }
    return QString::number(quantity);
}

void PDFGenerator::drawRoundedRect(QPainter& painter, const QRect& rect,
                                  int radius, const QColor& fillColor,
                                  const QColor& borderColor, int borderWidth) {
    painter.save();
    
    painter.setBrush(QBrush(fillColor));
    painter.setPen(QPen(borderColor, borderWidth));
    painter.drawRoundedRect(rect, radius, radius);
    
    painter.restore();
}

QString PDFGenerator::truncateText(const QString& text, 
                                  const QFontMetrics& metrics,
                                  int maxWidth) {
    if (metrics.horizontalAdvance(text) <= maxWidth) {
        return text;
    }
    
    QString truncated = text;
    QString ellipsis = "...";
    int ellipsisWidth = metrics.horizontalAdvance(ellipsis);
    
    while (!truncated.isEmpty() && 
           metrics.horizontalAdvance(truncated) + ellipsisWidth > maxWidth) {
        truncated.chop(1);
    }
    
    return truncated + ellipsis;
}

// ===== MÉTODOS PARA MANEJO DE COLORES =====

QColor PDFGenerator::getHeaderColor(bool colorMode) {
    return colorMode ? QColor(52, 152, 219) : Qt::darkGray;
}

QColor PDFGenerator::getRowColor(int rowIndex, bool isLowStock, bool colorMode) {
    if (isLowStock) {
        return colorMode ? QColor(255, 230, 230) : QColor(240, 240, 240);
    }
    
    if (rowIndex % 2 == 0) {
        return colorMode ? QColor(255, 255, 255) : QColor(255, 255, 255);
    } else {
        return colorMode ? QColor(245, 245, 245) : QColor(250, 250, 250);
    }
}

QColor PDFGenerator::getTextColor(bool isLowStock, bool colorMode) {
    if (isLowStock) {
        return colorMode ? QColor(231, 76, 60) : Qt::red;
    }
    return colorMode ? QColor(50, 50, 50) : Qt::black;
}

bool PDFGenerator::generateStatisticsPDF(const std::vector<Component>& components,
                                         const std::string& filename) {
    // Implementación similar pero enfocada en gráficos estadísticos
    // Puedes implementar esto más adelante si lo necesitas
    return generatePDFReport(components, filename, true, true);
}