// #ifndef CALCULADORA_H
// #define CALCULADORA_H
#pragma once
#include <iostream>

class Calculadora
{
public:
    Calculadora();
    double sumar(double numero, double numero2);
    double restar(double numero, double numero2);
    double multiplica(double numero, double numero2);
    double divide(double numero, double numero2);

    double obtenerNumero();
    int obtenerOpcion();
    double validarDivicion();

    // Mostrar el menu de opciones
    void showMenu();
};
// #endif