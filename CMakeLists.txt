cmake_minimum_required(VERSION 3.10)
project(GestorInventario LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Buscar paquetes necesarios
find_package(Qt5 COMPONENTS Widgets REQUIRED)

# Buscar SQLite3 de forma correcta
find_package(SQLite3 REQUIRED)

# Mensaje de depuración
message(STATUS "SQLite3 found: ${SQLite3_FOUND}")
message(STATUS "SQLite3 include dir: ${SQLite3_INCLUDE_DIRS}")
message(STATUS "SQLite3 libraries: ${SQLite3_LIBRARIES}")

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

# Enlazar bibliotecas - FORMA CORRECTA
target_link_libraries(GestorInventario PRIVATE 
    Qt5::Widgets
    ${SQLite3_LIBRARIES}  # Usar la variable correcta
)

# Incluir directorios
target_include_directories(GestorInventario PRIVATE 
    ${SQLite3_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Configuración para macOS
if(APPLE)
    set_target_properties(GestorInventario PROPERTIES
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_GUI_IDENTIFIER "com.gestor.inventario"
        MACOSX_BUNDLE_BUNDLE_NAME "Gestor de Inventario"
    )
endif()