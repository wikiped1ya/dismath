#include "BigCalculator.h"
#include <iostream>
#include <string>
#include <vector>

#ifdef _WIN32
#include <windows.h>
#endif

void printNumber(const std::vector<char>& number) {
   if (number.empty()) {
      std::cout << "empty";
      return;
   }

   for (char value : number) {
      std::cout << value;
   }
}

int main() {
#ifdef _WIN32
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
#endif

   BigCalculator calculator;

   std::cout << "Калькулятор готов\n";

   while (true) {
      std::cout << "1 - сложение\n";
      std::cout << "2 - вычитание\n";
      std::cout << "3 - умножение\n";
      std::cout << "4 - деление\n";
      std::cout << "0 - выход\n";
      std::cout << "Выбор операции: ";

      std::string operation;
      std::cin >> operation;

      if (!std::cin) {
         return 0;
      }

      if (operation == "0") {
         std::cout << "Программа завершена\n";
         break;
      }

      if (operation != "1" and operation != "2" and operation != "3" and operation != "4") {
         std::cout << "Выберите число от 0 до 4\n";
         continue;
      }

      std::string firstText;
      std::string secondText;

      std::cout << "Введите первый символ: ";
      std::cin >> firstText;

      std::cout << "Введите второй символ: ";
      std::cin >> secondText;

      std::vector<char> first(firstText.begin(), firstText.end());
      std::vector<char> second(secondText.begin(), secondText.end());

      if (operation == "1") {
         std::vector<char> result = calculator.add(first, second);

         std::cout << "Результат: ";

         if (!calculator.normalSize) {
            std::cout << "overflow";
         } else {
            printNumber(result);
         }

         std::cout << "\n";
      }

      if (operation == "2") {
         std::vector<char> result = calculator.subtract(first, second);

         std::cout << "Результат: ";
         printNumber(result);
         std::cout << "\n";
      }

      if (operation == "3") {
         std::vector<char> result = calculator.multiply(first, second);

         std::cout << "Результат: ";

         if (!calculator.normalSize) {
            std::cout << "overflow";
         } else {
            printNumber(result);
         }

         std::cout << "\n";
      }

      if (operation == "4") {
         std::cout << "Результат: ";
         std::cout << calculator.division(first, second);
         std::cout << "\n";
      }
   }

   return 0;
}