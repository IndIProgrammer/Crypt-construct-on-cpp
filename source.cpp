#include "head.h"
#include <algorithm>
#include <iostream>
#include <cstdlib>  // для srand, rand
#include <ctime>    // для time
#include <sstream>
#include <cctype>
// Глобальная переменная для хранения текущего ключа
std::vector<int> currentKey;

//Проверка правильности ввода
int getValidatedInt(const std::string& prompt, int minVal, int maxVal) {
    std::string input;
    int value;

    while (true) {
        std::cout << prompt;
        std::getline(std::cin, input);

        std::stringstream ss(input);
        if (ss >> value) { // извлекаем число из ввода
            // Проверяем, что не осталось лишних символов
            char remaining;
            if (ss >> remaining) {
                std::cout << "Ошибка! Введите только число.\n";
            } else if (value < minVal || value > maxVal) {
                std::cout << "Ошибка! Число должно быть от " << minVal
                          << " до " << maxVal << ".\n";
            } else {
                return value;
            }
        } else {
            std::cout << "Ошибка! Введите корректное число.\n";
        }
    }
}

std::vector<int> generatePermutationKey(int size) {
    std::vector<int> key(size);
    for (int i = 0; i < size; i++) {
        key[i] = i + 1;
    }
    // Инициализируем генератор случайных чисел текущим временем
    std::srand(static_cast<unsigned int>(std::time(0)));
    // Алгоритм Фишера-Йетса (тасование Кнута) для перемешивания
    for (int i = size - 1; i > 0; i--) {
        // Генерируем случайный индекс от 0 до i
        int j = std::rand() % (i + 1);

        // Меняем местами элементы i и j
        int temp = key[i];
        key[i] = key[j];
        key[j] = temp;
    }
    return key;
}

bool validateKey(const std::vector<int>& key) {
    if (key.empty()) {
        return false;
    }

    int n = key.size();
    std::vector<bool> present(n, false);

    for (int num : key) {
        if (num < 1 || num > n) {
            return false;
        }
        // Для проверки уникальности чисел
        if (present[num - 1]) {
            return false;
        }
        present[num - 1] = true;
    }
    return true;
}

std::string encryptSimplePermutation(const std::string& text, const std::vector<int>& key) {
    // Текст записывается в таблицу по строкам а считывается по столбцам в порядке заданном ключом-перестановкой
    int n = key.size(); // длину ключа = кол-во столбцов
    int len = text.length();

    if (len == 0) {
        return "";
    }

    // Определяем количество строк в матрице
    int rows = (len + n - 1) / n;

    // Создаем матрицу rows x n
    std::vector<std::vector<char>> matrix(rows, std::vector<char>(n, ' '));

    // Заполняем матрицу по строкам (полностью, включая пробелы)
    int index = 0;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < n; j++) {
            if (index < len) {
                matrix[i][j] = text[index];
            } else {
                matrix[i][j] = ' '; // Заполняем пробелами пустые ячейки
            }
            index++;
        }
    }

    // Шифруем: считываем столбцы в порядке ключа
    std::string result;

    for (int i = 0; i < n; i++) {
        int col = key[i] - 1;

        // Считываем весь столбец сверху вниз, включая пробелы
        for (int row = 0; row < rows; row++) {
            result += matrix[row][col];
        }
    }

    return result;
}
std::string decryptSimplePermutation(const std::string& text, const std::vector<int>& key) {
    // Восстанавливает исходную таблицу, записав шифртекст в столбцы в порядке ключа, и затем читает таблицу по строкам
    int n = key.size();
    int len = text.length();

    if (len == 0) {
        return "";
    }

    // Длина текста должна быть кратна n для простого шифра с пробелами
    if (len % n != 0) {
        std::cout << "Ошибка: длина зашифрованного текста должна быть кратна размеру ключа!" << std::endl;
        return "";
    }

    // Определяем количество строк в матрице
    int rows = len / n;

    // Создаем матрицу rows x n
    std::vector<std::vector<char>> matrix(rows, std::vector<char>(n, ' '));

    // Заполняем матрицу из шифртекста по столбцам в порядке ключа
    int index = 0;
    for (int i = 0; i < n; i++) {
        int col = key[i] - 1;

        // Записываем весь столбец сверху вниз, включая пробелы
        for (int row = 0; row < rows; row++) {
            if (index < len) {
                matrix[row][col] = text[index];
                index++;
            }
        }
    }

    // Считываем по строкам, включая пробелы
    std::string result;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < n; j++) {
            result += matrix[i][j];
        }
    }

    return result;
}

std::string encryptBlockPermutation(const std::string& text, const std::vector<int>& key, int blockSize) {
    //Текст разбивается на блоки фиксированного размера, и внутри каждого блока символы переставляются согласно ключу
    std::string result;

    if (key.size() != blockSize) {
        std::cout << "Ошибка: размер ключа должен быть равен размеру блока!" << std::endl;
        return "";
    }

    // Разбиваем текст на блоки
    for (size_t i = 0; i < text.length(); i += blockSize) {
        std::string block = text.substr(i, blockSize);

        // Дополняем последний блок пробелами
        if (block.length() < blockSize) {
            block.append(blockSize - block.length(), ' ');
        }

        // Переставляем символы в блоке согласно ключу
        std::string encryptedBlock;
        for (int j = 0; j < blockSize; j++) {
            int srcIndex = key[j] - 1;
            encryptedBlock += block[srcIndex];
        }
        result += encryptedBlock;
    }
    return result;
}
std::string decryptBlockPermutation(const std::string& text, const std::vector<int>& key, int blockSize) {
    //Восстанавливает исходный текст, применив к каждому блоку шифртекста обратный ключ-перестановку
    std::string result;

    if (key.size() != blockSize) {
        std::cout << "Ошибка: размер ключа должен быть равен размеру блока!" << std::endl;
        return "";
    }

    // Создаем обратный ключ
    std::vector<int> reverseKey(blockSize);
    for (int i = 0; i < blockSize; i++) {
        // Ищем, куда попадает символ из позиции i исходного блока
        for (int j = 0; j < blockSize; j++) {
            if (key[j] == i + 1) {
                reverseKey[i] = j + 1;
                break;
            }
        }
    }
    // Разбиваем на блоки
    for (size_t i = 0; i < text.length(); i += blockSize) {
        std::string block = text.substr(i, blockSize);
        // Дополняем пробелами, если нужно
        if (block.length() < blockSize) {
            block.append(blockSize - block.length(), ' ');
        }

        // Восстанавливаем исходный порядок
        std::string decryptedBlock;
        for (int j = 0; j < blockSize; j++) {
            int srcIndex = reverseKey[j] - 1;
            decryptedBlock += block[srcIndex];
        }
        result += decryptedBlock;
    }

    // Убираем пробелы в конце
    size_t lastPos = result.find_last_not_of(' '); // находит последний символ "не пробел"
    if (lastPos != std::string::npos) {
        result = result.substr(0, lastPos + 1); // удаляет лишние пробелы
    } else {
        result = "";
    }

    return result;
}

void displayMenu() {
    std::cout << "\n=== КОНСТРУКТОР КРИПТОГРАММ ===\n";
    std::cout << "1. Сгенерировать новый ключ\n";
    std::cout << "2. Ввести ключ вручную\n";
    std::cout << "3. Показать текущий ключ\n";
    std::cout << "4. Проверка корректности ключа\n";
    std::cout << "5. Шифрование строки (простая перестановка)\n";
    std::cout << "6. Расшифровка строки (простая перестановка)\n";
    std::cout << "7. Шифрование строки (блочная перестановка)\n";
    std::cout << "8. Расшифровка строки (блочная перестановка)\n";
    std::cout << "0. Выход\n";
    std::cout << "Выберите действие (0-8): ";
}
void displayKeyInputMenu() {
    std::cout << "\n=== ВВОД КЛЮЧА ===\n";
    std::cout << "1. Ввести весь ключ полностью\n";
    std::cout << "2. Ввести по одному числу\n";
    std::cout << "3. Назад\n";
}
void generateNewKey() {
    int size = getValidatedInt("Введите размер ключа: ", 1, 100);
    currentKey = generatePermutationKey(size);
    std::cout << "Новый ключ сгенерирован: ";
    for (int num : currentKey) {
        std::cout << num << " ";
    }
    std::cout << std::endl;
}

void enterKeyManually() {
    int choice;
    do {
        displayKeyInputMenu();
        choice = getValidatedInt("Выберите способ ввода (1-3): ", 1, 3);
        if (choice == 3) {
            break;
        }
        if (choice == 1) {
            int keySize = getValidatedInt("Введите размер ключа: ", 1, 100);
            currentKey.clear();
            std::cout << "Введите ключ (числа через пробел): ";
            std::string input;
            std::getline(std::cin, input);

            std::stringstream ss(input);
            int num;
            int count = 0;

            while (ss >> num && count < keySize) {
                currentKey.push_back(num);
                count++;
            }
            // число введенных чисел = размеру
            if (count != keySize) {
                std::cout << "Ошибка: введено " << count << " чисел вместо " << keySize << "!\n";
                currentKey.clear();
                continue;
            }
            // Проверяем, что не осталось лишних чисел
            if (ss >> num) {
                std::cout << "Ошибка: введено больше чисел, чем указанный размер!\n";
                currentKey.clear();
                continue;
            }
        }
        else if (choice == 2) {
            currentKey.clear();
            std::cout << "Вводите числа ключа по одному (пустая строка для завершения):\n";

            int count = 1;
            while (true) {
                std::cout << "Число " << count << ": ";
                std::string input;
                std::getline(std::cin, input);

                if (input.empty()) {
                    if (currentKey.empty()) {
                        std::cout << "Ключ не должен быть пустым! Продолжайте ввод.\n";
                        continue;
                    }
                    break;
                }

                std::stringstream ss(input);
                int num;
                if (ss >> num) {
                    // Проверяем, что в строке только число
                    char remaining;
                    if (ss >> remaining) {
                        std::cout << "Ошибка! Введите только число.\n";
                        continue;
                    }
                    currentKey.push_back(num);
                    count++;
                } else {
                    std::cout << "Ошибка! Введите корректное число.\n";
                }
            }
        }
        if (validateKey(currentKey)) {
            std::cout << "Ключ успешно установлен!" << std::endl;
        }
        else {
            std::cout << "Ошибка: ключ некорректен!" << std::endl;
            currentKey.clear();
        }
    } while (choice != 3);
}

void showCurrentKey() {
    if (currentKey.empty()) {
        std::cout << "Ключ не установлен!" << std::endl;
    }
    else {
        std::cout << "Текущий ключ (размер " << currentKey.size() << "): ";
        for (int num : currentKey) {
            std::cout << num << " ";
        }
        std::cout << std::endl;
    }
}

void handleSimplePermutation(bool encrypt) {
    if (currentKey.empty()) {
        std::cout << "Ошибка: сначала установите ключ!" << std::endl;
        return;
    }

    std::string text;
    if (encrypt) {
        std::cout << "Введите текст для шифрования: ";
    }
    else {
        std::cout << "Введите текст для расшифровки: ";
    }
    std::getline(std::cin, text);

    std::string result;
    if (encrypt) {
        result = encryptSimplePermutation(text, currentKey);
        std::cout << "Зашифрованный текст: " << result << std::endl;
    }
    else {
        result = decryptSimplePermutation(text, currentKey);
        std::cout << "Расшифрованный текст: " << result << std::endl;
    }
}

void handleBlockPermutation(bool encrypt) {
    if (currentKey.empty()) {
        std::cout << "Ошибка: сначала установите ключ!" << std::endl;
        return;
    }

    int blockSize = getValidatedInt("Введите размер блока: ", 1, 100);

    if (currentKey.size() > blockSize) {
        std::cout << "Ошибка: размер ключа (" << currentKey.size()
                  << ") больше размера блока (" << blockSize << ")!" << std::endl;
        return;
    }

    std::string text;
    if (!encrypt) {
        std::cout << "Введите текст для шифрования: ";
    }
    else {
        std::cout << "Введите текст для расшифровки: ";
    }
    std::getline(std::cin, text);

    std::string result;
    if (encrypt) {
        result = encryptBlockPermutation(text, currentKey, blockSize);
        std::cout << "Зашифрованный текст: " << result << std::endl;
    }
    else {
        result = decryptBlockPermutation(text, currentKey, blockSize);
        std::cout << "Расшифрованный текст: " << result << std::endl;
    }
}

void validateCurrentKey() {
    if (currentKey.empty()) {
        std::cout << "Ключ не установлен!" << std::endl;
    }
    else {
        if (validateKey(currentKey)) {
            std::cout << "Текущий ключ корректен!" << std::endl;
        }
        else {
            std::cout << "Текущий ключ некорректен!" << std::endl;
        }
    }
}
