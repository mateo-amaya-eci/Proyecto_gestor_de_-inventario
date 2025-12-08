# Proyecto de Algoritmos en sistemas electrónicos
### *Participantes:*
- *Mateo Felipe Amaya Novoa*
- *Jesus Estiven Vargas Calderon*
- *Daniel Andres Valderrama Lopez*


## Proyecto 3.3  Gestor de Inventario para Hogar o Laboratorio
### Descripción General
Sistema de inventario para administrar componentes electr´onicos o dispositivos del hogar/laboratorio. Implementado en C++ con Qt y base de datos SQLite, orientado a dispositivos de bajo consumo como Raspberry Pi
### Características Principales
- Ingreso de art´ıculos: nombre, tipo, cantidad, ubicaci´on, fecha de adquisición.
- Búsqueda y filtrado en la lista de componentes.
- Alerta de stock bajo.
- Generación de reportes en CSV o PDF.
### Diseño Orientado a Objetos
- class Component
- class InventoryManager
- class DatabaseManager
- class ReportGenerator}
### Base de Datos (Modelo ER)
- **Tabla components:** id, name, type, quantity, location, purchase date
### Interfaz Qt
- Ventana principal con:
  - Tabla de componentes.
  - Formulario para agregar/editar artículos
  - Filtro de búsqueda rápida.
