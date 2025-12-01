cmake_minimum_required(VERSION 3.10)
project(InventoryManager)

# Configurar estándar C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Modo Release para evitar warnings
set(CMAKE_BUILD_TYPE Release)

# Encontrar SQLite3 de manera simple
find_library(SQLITE3_LIB sqlite3)

# Archivos fuente básicos (ajusta según tus archivos reales)
set(SOURCES
    src/main.cpp
    src/Component.cpp
    src/InventoryManager.cpp
    src/DatabaseManager.cpp
)

# Archivos de cabecera
set(HEADERS
    src/Component.h
    src/InventoryManager.h  
    src/DatabaseManager.h
)

# Crear ejecutable
add_executable(InventoryManager ${SOURCES} ${HEADERS})

# Incluir directorios
target_include_directories(InventoryManager PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Enlazar SQLite3 si se encontró
if(SQLITE3_LIB)
    target_link_libraries(InventoryManager ${SQLITE3_LIB})
    message(STATUS "SQLite3 encontrado: ${SQLITE3_LIB}")
else()
    message(WARNING "SQLite3 no encontrado, compilando sin soporte de base de datos")
endif()

message(STATUS "Configuración completada para ${PROJECT_NAME}")