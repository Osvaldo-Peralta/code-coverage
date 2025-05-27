#include <iostream>
#include <string>
#include "Calculadora.h"

const std::string menu = "1. Sumar\n2. Restar\n3. Multiplicar\n4. Dividir\n5. Salir\n";

Calculadora::Calculadora() {}

double Calculadora::sumar(double numero, double numero2)
{
    return numero + numero2;
}

double Calculadora::restar(double numero, double numero2)
{
    return numero - numero2;
}

double Calculadora::multiplica(double numero, double numero2)
{
    return numero * numero2;
}

double Calculadora::divide(double numero, double numero2)
{
    if (numero2 == 0)
    {
        throw std::invalid_argument("No se puede dividir entre 0");
    }
    return numero / numero2;
}

double Calculadora::validarDivicion()
{
    using std::cin, std::cout, std::endl;
    int numeroValido;
    while (true)
    {
        cout << "Ingresa el numero: ";
        cin >> numeroValido;

        if (numeroValido == 0)
        {
            cout << "[Advertencia]: El numero no puede ser 0" << endl;
            return numeroValido;
        }
    }
}

int Calculadora::obtenerOpcion()
{
    using std::cin, std::cout, std::endl;
    int numeroOpcion;
    while (true)
    {
        cout << "Que operacion quieres realizar: ";
        cin >> numeroOpcion;
        if (numeroOpcion > 0)
        {
            return numeroOpcion;
        }
        else
        {
            cout << "Por favor ingresa un numero mayor a 0\n"
                 << endl;
        }
    }
}

double Calculadora::obtenerNumero()
{
    using std::cin, std::cout, std::endl;
    double cantidad;
    while (true)
    {
        cout << "Ingresa el un numero: ";
        cin >> cantidad;

        return cantidad;
    }
}

void Calculadora::showMenu()
{
    using std::cout, std::endl;
    int opcion = 0;
    double numero = 0;
    double numero2 = 0;

    cout << menu << endl;
    opcion = obtenerOpcion();

    switch (opcion)
    {
    case 1:
        numero = obtenerNumero();
        numero2 = obtenerNumero();
        cout << "El resultado de " << numero << " + " << numero2 << " es: " << sumar(numero, numero2) << endl;
        break;
    case 2:
        numero = obtenerNumero();
        numero2 = obtenerNumero();
        cout << "El resultado de " << numero << " - " << numero2 << " es: " << restar(numero, numero2) << endl;
        break;
    case 3:
        numero = obtenerNumero();
        numero2 = obtenerNumero();
        cout << "El resultado de " << numero << " * " << numero2 << " es: " << multiplica(numero, numero2) << endl;
        break;
    case 4:
        numero = obtenerNumero();
        numero2 = validarDivicion();
        cout << "El resultado de " << numero << " / " << numero2 << " es: " << divide(numero, numero2) << endl;
        break;
    case 5:
        cout << "Hasta la proxima...." << endl;
        break;
    default:
        cout << "Opcion no valida. Intente otra vez" << endl;
    }
}