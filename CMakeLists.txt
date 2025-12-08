cmake_minimum_required(VERSION 3.10)
project(GestorInventario LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Buscar paquetes necesarios - ¡AGREGAR PrintSupport!
find_package(Qt5 COMPONENTS Widgets PrintSupport REQUIRED)
find_package(SQLite3 REQUIRED)

# Incluir directorios
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

# Archivos del proyecto
set(SOURCES
    src/main.cpp
    src/MainWindow.cpp
    src/Component.cpp
    src/DatabaseManager.cpp
    src/InventoryManager.cpp
    src/ReportGenerator.cpp
    src/PDFGenerator.cpp  # ¡Nuevo archivo!
)

set(HEADERS
    src/MainWindow.h
    src/Component.h
    src/DatabaseManager.h
    src/InventoryManager.h
    src/ReportGenerator.h
    src/PDFGenerator.h    # ¡Nuevo archivo!
)

# Crear ejecutable
add_executable(GestorInventario ${SOURCES} ${HEADERS})

# Enlazar bibliotecas - ¡AGREGAR PrintSupport!
target_link_libraries(GestorInventario PRIVATE 
    Qt5::Widgets
    Qt5::PrintSupport      # ¡Importante!
    ${SQLITE3_LIBRARY}
)

# Incluir directorios de SQLite3
target_include_directories(GestorInventario PRIVATE ${SQLITE3_INCLUDE_DIR})