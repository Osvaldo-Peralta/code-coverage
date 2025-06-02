# Compilacion Estandar para Code Coverage

## Estructura de Directorios
- `src/`: Código fuente principal (.cpp).  
- `include/`: Cabeceras públicas (.h).  
- `tests/`: Pruebas unitarias.  
- `build/`: Generado por CMake (no versionado).

## Prerrequisitos
- CMake ≥ 3.18  
- Compilador GCC o Clang  
- Python (para gcovr)  
- GoogleTest (se descarga automáticamente)

## Explicación de la Configuracion del CMakeLists principal:

- Definimos la version minima de CMake
```cmake
cmake_minimum_required(VERSION 3.18)
```
- Se establece el nombre del proyecto y el lenguage de programación
```cmake
project(Calculadora LANGUAGES CXX)
```
- Configuración del estándar de C++ (opcional pero recomendado)
```cmake
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
```
- Opcion para habilitar el code coverage (OFF por defecto)
```cmake
option(ENABLE_COVERAGE "Habilita generación de code coverage" OFF)
```
> **Explicación**:
>
> `option()` crea una opción de configuración `ENABLE_COVERAGE` que el usuario puede activar con `-DENABLE_COVERAGE=ON`

- Si code coverage esta activo entonces
```cmake
if(ENABLE_COVERAGE)
    if(NOT CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        message(FATAL_ERROR "Coverage solo compatible con GCC/Clang")
    endif()
    # Flags aplicados solo a la biblioteca (no globales)
    set(COVERAGE_FLAGS -fprofile-arcs -ftest-coverage --coverage)
endif()
```
> - Verifica que el compilador sea GCC o Clang (los únicos que soportan coverage).
> - Añade flags de compilación (`-fprofile-arcs` `-ftest-coverage`) para instrumentar el código.
> - Añade flags de enlazado (`--coverage`) para generar datos de cobertura.
---
### Ahora definimos la biblioteca de la calculadora
```cmake
# Crear una biblioteca (paquete/librerias) con el codigo de Calculadora (Para poder reutilizarlo en los unitest).
add_library(calculadora_sources STATIC 
src/Calculadora.cpp
include/Calculadora.h
)
```
> *¿Por qué una biblioteca?*
Para encapsular el código reusable (tanto para el ejecutable principal como para las pruebas).
---
- Aqui es donde se crea la libreria instrumentada `calculadora_sources`:
```cmake
# Aplicar flags de cobertura solo a esta biblioteca
if(ENABLE_COVERAGE)
    target_compile_options(calculadora_sources PRIVATE ${COVERAGE_FLAGS})
    target_link_libraries(calculadora_sources PRIVATE ${COVERAGE_FLAGS})
endif()
```
- Le agregamos el directorio de cabeceras
```cmake
# Incluir directorios de cabeceras
target_include_directories(calculadora_sources PUBLIC include)
# Todas las cabeceras que estan dentro de include/
```
> **PUBLIC** tiene un efecto dual:

- Para la biblioteca: Las cabeceras en include/ son necesarias para compilar Calculadora.cpp (uso interno).
- Cualquier objetivo que use calculadora_sources (como main_app)
> ¿Porque **PUBLIC**?

Porque las cabeceras son necesarias para compilar la biblioteca **y** para cualquier objetivo que la use.
Si `main_app` incluye `Calculadora.h`, usa `PUBLIC`.
- Diagrama de dependencias:

![diagrama](/dependencias.svg)

> **Beneficios Clave de Este Enfoque**:
- *Escalabilidad*:
Si añades más directorios de cabeceras, solo los modificas en un lugar.
- *Mantenibilidad*:
Si cambias la estructura de directorios, solo actualizas la ruta en `target_include_directories`.
- *Preparación para Pruebas Unitarias*:
Las pruebas también pueden enlazar con `calculadora_sources` y heredarán automáticamente todas las configuraciones necesarias.
---

- Creamos el ejecutable principal llamado `main_app`
```cmake
add_executable(main_app src/main.cpp)
```
- Enlacamos el ejecutable principal `main_app` con la biblioteca `calculadora_sources`
```cmake
target_link_libraries(main_app calculadora_sources)
```
- Flujo de trabajo recomendado considerando pruebas unitarias.

| Paso | Acción de CMake |
|----------|----------|
| 1. Definir biblioteca    | `add_library(calculadora_sources ...)` + `target_include_directories(PUBLIC ...)`   |
| 2. Crear ejecutable principal    | `add_executable(main_app ...)` + `target_link_libraries(calculadora_sources)`  |
| 3. Añadir pruebas (opcional)    | `add_executable(tests ...)` + `target_link_libraries(calculadora_sources gtest)` |

---

### Información a considerar
Llegados a este punto de la configuracion del CMake principal, es importante repasar algunas cosas.

1. Las banderas para el coverage: `-fprofile-arcs` `-ftest-coverage`:
Instrumentan el código para rastrear qué líneas se ejecutan durante las pruebas.
2. `--coverage` en el linker:
Asegura que los archivos **`.gcno`** (información de cobertura) se generen correctamente.

## Configuración de Coverage
- Si coverage esta habilitado entonces:
```cmake
if(ENABLE_COVERAGE)
    find_program(GCOVR gcovr)
    if(GCOVR)
        message(STATUS "--> I Coverage")
        add_custom_target(coverage
            COMMAND ${GCOVR} 
                --root ${CMAKE_SOURCE_DIR} 
                --object-directory ${CMAKE_BINARY_DIR}/CMakeFiles/calculadora_sources.dir/src 
                --filter "${CMAKE_SOURCE_DIR}/src/.*\\.cpp"
                --exclude "tests/.*"      # 👈 Patrón relativo
                --html-details ${CMAKE_BINARY_DIR}/coverage-report.html
                --print-summary  # 👈 Ver resumen en terminal
            COMMAND ${CMAKE_COMMAND} -E echo "Informe: file://${CMAKE_BINARY_DIR}/coverage-report.html"
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
            DEPENDS tests  # Se deben ejecutar los test primero
            COMMENT "Generando informe..."
        )
    else()
        message(WARNING "Instala gcovr: pip install gcovr")
    endif()
endif()
```
- Explicacion de las flags:
+ Ejecuta `gcovr` para analizar los archivos `.gcda` `.gcno`.
+ `--root`: Ruta raíz del proyecto para mapear correctamente los archivos fuente.
    - Si tu proyecto está en `/home/usuario/proyecto`, gcovr buscará archivos fuente en `/home/usuario/proyecto/src/`
+ `--object-directory`: Indica a `gcovr` dónde buscar los archivos `.gcda`(Datos de ejecución)/`.gcno` (Información estatica).
+ `--filter`: Incluye solo los archivos `.cpp` del directorio `src/` en el informe.
+ `--exclude`: Excluye los archivos de pruebas del informe. `(tests/test_calculadora.cpp)`
+ `--html-details`: Genera un informe HTML detallado.
+ `--print-summary`: Muestra un resumen en la terminal después de generar el informe.
+ `DEPENDS tests`: Asegura que las pruebas se ejecuten antes de generar el informe.
    - Los archivos .gcda se generan solo después de ejecutar el código.
+ `WORKING_DIRECTORY ${CMAKE_BINARY_DIR}`: Ejecuta `gcovr` desde el directorio `build/`.
    - Evita conflictos con rutas relativas/absolutas.

### Consideraciones
- Verificar si el archivo tasks.json esta configurado para eleminar la carpeta build/ junto con la compilación