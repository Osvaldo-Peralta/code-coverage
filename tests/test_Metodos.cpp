#include <gtest/gtest.h>
#include <sstream>
#include "Calculadora.h"

class IORedirect
{
public:
    // Se desarrolla toda la logica en el constructor
    IORedirect()
    {
        // respaldo de buffers originales
        orig_cin = std::cin.rdbuf();
        orig_cout = std::cout.rdbuf();

        // Redirigir a nuestros streams
        std::cin.rdbuf(input.rdbuf());
        std::cout.rdbuf(output.rdbuf());
    }

    ~IORedirect()
    {
        // Restaurar buffers originales
        std::cin.rdbuf(orig_cin);
        std::cout.rdbuf(orig_cout);
    }

    // Metodo para simular entrada
    void setInput(const std::string &in)
    {
        input.str(in);
        input.clear(); // Limpiamos flags de error
    }

    // Metodo para devolver la salida
    std::string getOutput()
    {
        return output.str();
    }

private:
    std::streambuf *orig_cin;
    std::streambuf *orig_cout;
    std::stringstream input;
    std::stringstream output;
};

TEST(CalculadoraTest, ObtenerNumero)
{
    Calculadora calc;
    IORedirect io;

    io.setInput("42\n"); // Simula entrada del usuario
    double resultado = calc.obtenerNumero();

    // Pruebas
    EXPECT_EQ(resultado, 42);
    EXPECT_TRUE(io.getOutput().find("Ingresa un numero:") != std::string::npos);
}

TEST(CalculadoraTest, DivicionValida)
{
    Calculadora calc;
    IORedirect io;

    io.setInput("0\n5\n");
    // El primero 0 es invalido, el segundo si es valido
    double resultado = calc.validarDivicion();

    // Pruebas
    EXPECT_EQ(resultado, 5);
    std::string output = io.getOutput();
    EXPECT_TRUE(output.find("El numero no puede ser 0") != std::string::npos);
    EXPECT_TRUE(output.find("Ingresa el numero:") != std::string::npos);
}

TEST(CalculadoraTest, ObtenerOpcionValida)
{
    Calculadora calc;
    IORedirect io;

    io.setInput("3\n");
    int result = calc.obtenerOpcion();

    EXPECT_EQ(result, 3);
}

TEST(CalculadoraTest, ObtenerOpcionInvalida)
{
    Calculadora calc;
    IORedirect io;

    io.setInput("-2\n4\n"); // Inválido luego válido
    int result = calc.obtenerOpcion();

    EXPECT_EQ(result, 4);
    EXPECT_TRUE(io.getOutput().find("Por favor ingresa un numero mayor a 0") != std::string::npos);
}

TEST(CalculadoraTest, MenuSuma)
{
    Calculadora calc;
    IORedirect io;

    // Secuencia: Opción 1, num1=5, num2=3
    io.setInput("1\n5\n3\n");
    calc.showMenu();

    std::string output = io.getOutput();
    EXPECT_TRUE(output.find("El resultado de 5 + 3 es: 8") != std::string::npos);
}

TEST(CalculadoraTest, MenuResta)
{
    Calculadora calc;
    IORedirect io;

    // Secuencia: Opcion 2, num1=20, num2=8
    io.setInput("2\n20\n8\n");
    calc.showMenu();

    std::string output = io.getOutput();
    EXPECT_TRUE(output.find("El resultado de 20 - 8 es: 12") != std::string::npos);
}

TEST(CalculadoraTest, MenuMultiplica)
{
    Calculadora calc;
    IORedirect io;

    // Secuencia: Opcion 3, num1=8, num2=3
    io.setInput("3\n8\n3\n");
    calc.showMenu();
    std::string output = io.getOutput();
    EXPECT_TRUE(output.find("El resultado de 8 * 3 es: 24") != std::string::npos);
}

TEST(CalculadoraTest, MenuDivisionInvalida)
{
    Calculadora calc;
    IORedirect io;

    // Opción 4, num1=10, num2=0 (inválido), luego num2=2
    io.setInput("4\n10\n0\n2\n");
    calc.showMenu();

    std::string output = io.getOutput();
    EXPECT_TRUE(output.find("El numero no puede ser 0") != std::string::npos);
    EXPECT_TRUE(output.find("El resultado de 10 / 2 es: 5") != std::string::npos);
}

TEST(CalculadoraTest, CasoSalida)
{
    Calculadora calc;
    IORedirect io;

    // Secuencia salida: Opcion 5
    io.setInput("5\n");
    calc.showMenu();

    std::string output = io.getOutput();
    EXPECT_TRUE(output.find("Hasta la proxima....") != std::string::npos);
}

TEST(CalculadoraTest, OpcionFueraDeRango)
{
    Calculadora calc;
    IORedirect io;

    // Secuencia salida: Opcion 6 -> No valida
    io.setInput("6\n");
    calc.showMenu();

    std::string output = io.getOutput();
    EXPECT_TRUE(output.find("Opcion no valida. Intente otra vez") != std::string::npos);
}