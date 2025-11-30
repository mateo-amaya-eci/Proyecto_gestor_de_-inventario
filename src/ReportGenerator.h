#ifndef REPORTGENERATOR_H
#define REPORTGENERATOR_H

#include "Component.h"
#include <vector>
#include <string>

class ReportGenerator {
public:
    static bool generateCSVReport(const std::vector<Component>& components, 
                                 const std::string& filename);
    static bool generateHTMLReport(const std::vector<Component>& components,
                                  const std::string& filename);
    static bool generateLowStockReport(const std::vector<Component>& components,
                                      const std::string& filename, int threshold = 5);
};

#endif