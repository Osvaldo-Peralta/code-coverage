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
    - Si tests/CMakeLists.txt está en /proyecto/tests/, PROJECT_ROOT será /proyecto/.