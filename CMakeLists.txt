cmake_minimum_required(VERSION 3.16)
project(InventoryManager)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Encontrar Qt5
find_package(Qt5 REQUIRED Core Widgets)

# Encontrar SQLite3
find_package(PkgConfig REQUIRED)
pkg_check_modules(SQLITE3 REQUIRED sqlite3)

# Archivos fuente
set(SOURCES
    src/main.cpp
    src/MainWindow.cpp
    src/Component.cpp
    src/InventoryManager.cpp
    src/DatabaseManager.cpp
    src/ReportGenerator.cpp
)

# Archivos de cabecera
set(HEADERS
    src/MainWindow.h
    src/Component.h
    src/InventoryManager.h
    src/DatabaseManager.h
    src/ReportGenerator.h
)

# Crear ejecutable
add_executable(InventoryManager ${SOURCES} ${HEADERS})

# Enlazar librerías
target_link_libraries(InventoryManager 
    Qt5::Core 
    Qt5::Widgets
    ${SQLITE3_LIBRARIES}
)

# Incluir directorios
target_include_directories(InventoryManager PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${SQLITE3_INCLUDE_DIRS}
)

# Configuración para release y reducir tamaño
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_options(InventoryManager PRIVATE -Os)
endif()