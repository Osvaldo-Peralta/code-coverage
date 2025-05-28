# Version Minima
cmake_minimum_required(VERSION 3.28)
project ("unitTests" LANGUAGES CXX)

# Agregar un mensaje, opcional
message(STATUS "--> I CMaking: ${PROJECT_NAME} Configurando pruebas")

# Descargar GoogleTest automáticamente
include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.12.1
)
FetchContent_MakeAvailable(googletest)

# Configurar ruta relativa donde se guardara el ejecutable de test
set(PROJECT_ROOT ${CMAKE_SOURCE_DIR}/..) # Ajustar segun lo necesitemos

# Ejecutable de pruebas
add_executable(${PROJECT_NAME} test_calculadora.cpp test_Metodos.cpp) # Actualizado: Incluye las pruebas de simulaciones de entradas

# Incluir cabeceras de la calculadora
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_ROOT}/include)

# Enlazar con la biblioteca(Librerias)  de la calculadora y googletest 
target_link_libraries(${PROJECT_NAME} PRIVATE calculadora_sources gtest gtest_main)

# Registrar pruebas
add_test(NAME test_calculadora COMMAND ${PROJECT_NAME} WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
