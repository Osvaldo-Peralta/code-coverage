#include <gtest/gtest.h>

#include "Calculadora.h"
TEST(myCalculadoraTest, SumaTest)
{
    Calculadora calculadorcita;
    EXPECT_DOUBLE_EQ(calculadorcita.sumar(20, 10), 30);
    EXPECT_DOUBLE_EQ(calculadorcita.sumar(-20, 14), -6);
    EXPECT_DOUBLE_EQ(calculadorcita.sumar(0, 3), 3);
}

TEST(myCalculadoraTest, RestaTest)
{
    Calculadora calculadorcita;
    EXPECT_DOUBLE_EQ(calculadorcita.restar(30, 2), 28);
}

TEST(myCalculadoraTest, MultiplicaTest)
{
    Calculadora calculadorcita;
    EXPECT_DOUBLE_EQ(calculadorcita.multiplica(5, 3), 15);
    EXPECT_DOUBLE_EQ(calculadorcita.multiplica(5, 0), 0);
    EXPECT_DOUBLE_EQ(calculadorcita.multiplica(70, 85), 5950);
}

TEST(myCalculadoraTest, DivideTest)
{
    Calculadora calculadorcita;
    EXPECT_DOUBLE_EQ(calculadorcita.divide(20, -5), -4);
    EXPECT_THROW(calculadorcita.divide(20, 0), std::invalid_argument);
    EXPECT_DOUBLE_EQ(calculadorcita.divide(10, 2), 5);
}

/*
TEST_F(test_calculadora, MetodosVacios) {

}
*/