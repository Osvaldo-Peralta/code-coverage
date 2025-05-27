# Version Minima
cmake_minimum_required(VERSION 3.28)
project ("unitTests" LANGUAGES CXX)

# Agregar un mensaje, opcional
message(STATUS "--> I CMaking: ${PROJECT_NAME}")

# Configurar ruta relativa donde se guardara el ejecutable de test
set(PROJECT_ROOT ${CMAKE_SOURCE_DIR}/..) # Ajustar segun lo necesitemos

# Ejecutable de pruebas
add_executable(${PROJECT_NAME} test_calculadora.cpp)

# Incluir cabeceras de la calculadora
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_ROOT}/include)

# Enlazar con la biblioteca(Librerias)  de la calculadora y googletest 
target_link_libraries(${PROJECT_NAME} PRIVATE calculadora_sources gtest gtest_main)

# Registrar pruebas
add_test(NAME test_calculadora COMMAND ${PROJECT_NAME} WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
