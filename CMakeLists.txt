cmake_minimum_required(VERSION 3.10)
project(GestorInventario LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Solo necesitamos Widgets, no PrintSupport
find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(SQLite3 REQUIRED)

# Incluir directorios
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

# Archivos del proyecto SIN PDFGenerator
set(SOURCES
    src/main.cpp
    src/MainWindow.cpp
    src/Component.cpp
    src/DatabaseManager.cpp
    src/InventoryManager.cpp
    src/ReportGenerator.cpp
)

set(HEADERS
    src/MainWindow.h
    src/Component.h
    src/DatabaseManager.h
    src/InventoryManager.h
    src/ReportGenerator.h
)

# Crear ejecutable
add_executable(GestorInventario ${SOURCES} ${HEADERS})

# Enlazar bibliotecas SIN PrintSupport
target_link_libraries(GestorInventario PRIVATE 
    Qt5::Widgets
    ${SQLITE3_LIBRARY}
)

# Incluir directorios de SQLite3
target_include_directories(GestorInventario PRIVATE ${SQLITE3_INCLUDE_DIR})

# Configuración para macOS
if(APPLE)
    set_target_properties(GestorInventario PROPERTIES
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_GUI_IDENTIFIER "com.gestor.inventario"
        MACOSX_BUNDLE_BUNDLE_NAME "Gestor de Inventario"
    )
endif()