# Version Minima de CMake
cmake_minimum_required(VERSION 3.18)
# Nombre del proyecto
project(Calculadora LANGUAGES CXX)

# ========== [1] Flags para Code Coverage ==========
option(ENABLE_COVERAGE "Habilita generación de code coverage" OFF)

# Configuracion de flags de coverage si esta activado
if(ENABLE_COVERAGE)
    if(NOT CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        message(FATAL_ERROR "Coverage solo compatible con GCC/Clang")
    endif()
    # Flags aplicados solo a la biblioteca (no globales)
    set(COVERAGE_FLAGS -fprofile-arcs -ftest-coverage --coverage)
endif()

# ========== [2] Configuración principal ==========
# Configuración del estándar de C++ (opcional pero recomendado)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Crear una biblioteca (paquete/librerias) con el codigo de Calculadora (Para poder reutilizarlo en los unitest).
add_library(calculadora_sources STATIC
    src/Calculadora.cpp
    include/Calculadora.h
)

# Incluir directorios de cabeceras
target_include_directories(calculadora_sources PUBLIC include)

# Aplicar flags de cobertura solo a esta biblioteca
if(ENABLE_COVERAGE)
    target_compile_options(calculadora_sources PRIVATE ${COVERAGE_FLAGS})
    target_link_libraries(calculadora_sources PRIVATE ${COVERAGE_FLAGS})
endif()

# Ejecutable principal
add_executable(main_app src/main.cpp)
target_link_libraries(main_app calculadora_sources)

# ========== [3] Configuración de pruebas unitarias ==========
# Incluir el directorio de pruebas
enable_testing()
add_subdirectory(tests)

# ========== [4] Configuración de Coverage Automática ==========
# Targets para generacion de informes (si coverage esta activado)
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
            DEPENDS tests  # 👈 ¡Posición correcta!
            COMMENT "Generando informe..."
        )
    else()
        message(WARNING "Instala gcovr: pip install gcovr")
    endif()
endif()