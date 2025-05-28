# Anotaciones sobre las testMetodos
## Clase `IORedirect`
Esta clase es la que se encarga de redirigir temporalmente los flujos de entrada y salida estandar `std::cin` y `std::cout` (E/S). Para simular la interacción del usuario y capturar resultados.

- Componentes clave como `std::streambuf* orig_cin;`: Son punteros a los buffers de flujo originales
- Su respectivo `std::stringstream input;`: Son flujos en memoria que reemplazan a cin/cout.
### Una definicion mas detallada seria asi:
```c++
// Constructor: Guarda y reemplaza buffers
IORedirect() {
    orig_cin = std::cin.rdbuf();   // Respalda buffer original de cin
    orig_cout = std::cout.rdbuf(); // Respalda buffer original de cout
    
    std::cin.rdbuf(input.rdbuf());   // Redirige cin a nuestro stringstream
    std::cout.rdbuf(output.rdbuf()); // Redirige cout a nuestro stringstream
}

// Destructor: Restaura buffers originales
~IORedirect() {
    std::cin.rdbuf(orig_cin);
    std::cout.rdbuf(orig_cout);
}
```

### ¿Como funciona?
1. Durante la prueba, redirecciona cualquier operacion de:
- std::cin -> Lee nuestro `input` (stringstream).
- std::cout -> Escribe en nuestro `output` (stringstream)
2. Resturación automatica:
- El destructor nos garantiza que al finalizar la prueba los flujos estandar vuelven a su estado original.

---

## Metodos clave de IORedirect
```cpp
void setInput(const std::string &in) {
    input.str(in);      // Establece el contenido de entrada
    input.clear();      // Limpia flags de estado (error/eof)
}

std::string getOutput() {
    return output.str(); // Devuelve todo lo capturado en cout
}
```
- `input.str()`: Prepara la entrada del usuario simulada.
    - Por ejemplo: **setInput("-2\n4\n")** simula que el usuario ingreso `-2` + `ENTER (\n)` + `4` + `ENTER`.
- `input.clear()`: Resetea el estado del stream despues de cada uso.
---
## Verificación de salidas
```cpp
EXPECT_TRUE(io.getOutput().find("texto buscado") != std::string::npos);
```
- `find()`: Busca una subcadena en el texto capturado.
- `npos`: de -> `'no position'` que indica que no fue encontrado.
- Verificación (cadena): Confirma que el texto esperado existe en la salida, si no coincide la prueba falla.

- Ejemplo practico:
```cpp
std::string output = "Ingresa un numero: 42";
bool encontrado = (output.find("Ingresa") != std::string::npos); // true
```

---

## Estructura de las pruebas unitarias
Cada prueba sigue un patron predeterminado:
```cpp
TEST(NombreSuite, NombrePrueba) {
    // 1. Preparación
    Calculadora calc;
    IORedirect io;           // Activa redirección
    
    // 2. Simular entrada usuario
    io.setInput("entradas\nsimuladas\n");
    
    // 3. Ejecutar funcionalidad a probar
    calc.metodoProbado();
    
    // 4. Verificaciones
    // - Valores de retorno
    EXPECT_EQ(resultado_esperado, valor_real);
    
    // - Contenido de salida
    std::string salida = io.getOutput();
    EXPECT_TRUE(salida.find("texto esperado") != std::string::npos);
} // Aquí se destruye 'io' y se restauran cin/cout automáticamente
```

### Ejemplo de caso de prueba implementado
- ObtenerNumero: Verifica lectura basica de numeros.
```cpp
TEST(CalculadoraTest, ObtenerNumero)
{
    // 1. Preparacion
    Calculadora calc;
    IORedirect io;

    // 2. Simulamos la entrada el usuario en 42
    io.setInput("42\n");
    double resultado = calc.obtenerNumero(); // Del metodo guardamos el numero en la variable resultado

    // 4.1 Prueba que verifica el comportamiento esperado del metodo
    EXPECT_EQ(resultado, 42);
    // 4.2 Contenido de salida
    EXPECT_TRUE(io.getOutput().find("Ingresa un numero:") != std::string::npos);
}
```
- `io.setInput("42\n");`: Esto carga el string "42\n" en el buffer de entrada (input stringstream). Equivale a un usuario tecleando 42.

