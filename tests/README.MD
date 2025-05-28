# Configuración de CMake de tests
- Descarga GoogleTest desde GitHub si no está disponible localmente.
```cmake
include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.12.1
)
FetchContent_MakeAvailable(googletest)
```
Si ya está descargado: No se vuelve a descargar.

- `set(PROJECT_ROOT ${CMAKE_SOURCE_DIR}/..)`: Define la ruta raíz del proyecto principal (un nivel arriba del directorio tests/).
    - Si tests/CMakeLists.txt está en `/proyecto/tests/`, `PROJECT_ROOT` será `/proyecto/`.
---
- Ejecutable de pruebas
```cmake
# Ejecutable de pruebas
add_executable(${PROJECT_NAME} test_calculadora.cpp)
```
Compila el archivo test_calculadora.cpp en un ejecutable llamado `unitTests`.
---
- Incluir cabeceras
```cmake
# Incluir cabeceras de la calculadora
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_ROOT}/include)
```
Permite al ejecutable de pruebas acceder a las cabeceras del proyecto principal.
> ¿Porque **PRIVATE**?
> Porque las cabeceras solo son necesarias para este objetivo.
---
- Enlace con las bibliotecas
```cmake
# Enlazar con la biblioteca(Librerias)  de la calculadora y googletest 
target_link_libraries(${PROJECT_NAME} PRIVATE calculadora_sources gtest gtest_main)
```
- `calculadora_sources`: es la biblioteca de programa principal que contiene el codigo a probar.
- ***`gtest`*** y ***`gtest_main`***: 
    - `gtest`: es el Framework de Googletest.
    - `gtest_main`: Proporciona una función `main()` predefinida (asi evitamos crearla manualmente).
---
- Registro de pruebas en CTest
```cmake
# Registrar pruebas
add_test(NAME test_calculadora COMMAND ${PROJECT_NAME} WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
```
- Se registra la prueba en CTest para ejecutarla con ctest
- `NAME test_calculadora`: Nombre que identifica la prueba.
- `COMMAND ${PROJECT_NAME}`: ejecuta el binario unitTests.
- `WORKING_DIRECTORY`: Directorio desde donde se ejecuta la prueba -> `build/`.