#include <iostream>
#include <string>
#include <sstream>
#include <windows.h>
#include "head.h"

int main() {
    SetConsoleCP(1251); // Для русского языка
    SetConsoleOutputCP(1251);

    int choice;
    std::string input;

    do {
        displayMenu();

        // Ввод с проверкой
        while (true) {
            std::getline(std::cin, input);
            std::stringstream ss(input);
            if (ss >> choice) {
                // Проверяем, что не осталось лишних символов
                char remaining;
                if (ss >> remaining) {
                    std::cout << "Ошибка! Введите только число.\n";
                } else if (choice < 0 || choice > 8) {
                    std::cout << "Ошибка! Число должно быть от 0 до 8.\n";
                } else {
                    break; // Успешный ввод
                }
            } else {
                std::cout << "Ошибка! Введите корректное число.\n";
            }
        }

        switch (choice) {
            case 1: generateNewKey(); break;
            case 2: enterKeyManually(); break;
            case 3: showCurrentKey(); break;
            case 4: validateCurrentKey(); break;
            case 5: handleSimplePermutation(true); break;
            case 6: handleSimplePermutation(false); break;
            case 7: handleBlockPermutation(false); break;
            case 8: handleBlockPermutation(true); break;
            case 0: std::cout << "Выход из программы...\n"; break;
        }
    } while (choice != 0);

    return 0;
}
