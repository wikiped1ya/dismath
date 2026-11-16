#pragma once
#include <map>
#include <utility>
#include <vector>
#include <string>

class BigCalculator
{
public:
    BigCalculator();
    bool normalSize; //true, когда нет превышения 8 разрядов 

    std::vector<char> add(const std::vector<char>& num1, const std::vector<char>& num2); //+
    std::vector<char> subtract(const std::vector<char>& num1, const std::vector<char>& num2); //-
    std::vector<char> multiply(const std::vector<char>& num1, const std::vector<char>& num2); //*
    std::string division(const std::vector<char>& num1, const std::vector<char>& num2);//:
//    std::string pow(const std::vector<char>& num1, const std::vector<char>& num2);
//    std::vector<char> gcd(const std::vector<char>& num1, const std::vector<char>& num2);
//    std::vector<char> lcm(const std::vector<char>& num1, const std::vector<char>& num2);
private:
    const std::vector<char> alphabet = {'a', 'b', 'h', 'g', 'd', 'f', 'c', 'k', 'i', 'e', 'j'};
    std::map<char, char> nextTable; //Таблица перехода
    std::map<char, std::map<char, char>> addition; //Таблица сложения
    std::map<char, std::map<char, char>> multiplication; //Таблица умножения

    char next(char value) const; //Следующий элемент 
    char previous(char value) const; //Предыдущий

    void buildSmallTables(); //Построение таблиц

    char addSymbol(char left, char right) const; //Сложение символов
    char multiplySymbol(char left, char right) const; //Умножение символов
    std::pair<char, char> addDigits(char left, char right, char carry) const; //Обработка одного разряда
    std::pair<char, char> subtractDigits(char left, char right, char borrow) const; //Обработка при вычитании
    bool validNumber(const std::vector<char>& number) const; //Проверка числа
    bool lessOrEqual(const std::vector<char>& left, const std::vector<char>& right) const; //Сранение чисел для вычитания
    std::vector<char> normalize(std::vector<char> number) const; //Без ведущих нулей
    std::vector<char> addNumbers(const std::vector<char>& left, const std::vector<char>& right); //Внутреннее сложение
    std::vector<char> subtractNumbers(const std::vector<char>& left, const std::vector<char>& right); //Внутреннее вычитание
    std::vector<char> multiplyBySymbol(const std::vector<char>& number, char multiplier); //Умножает большое число на один символ
    std::vector<char> shiftLeft(std::vector<char> number, const std::vector<char>& shift) const; //Добавляет справа нули
    std::vector<char> appendDigit(std::vector<char> number, char digit) const; //Дописывает символ справа(для деления в столбик)
    std::vector<char> multiplyNumbers(const std::vector<char>& left, const std::vector<char>& right); //Умножение чисел
    std::pair<std::vector<char>, std::vector<char>> divideNumbers(const std::vector<char>& dividend, const std::vector<char>& divisor); //Деление
};