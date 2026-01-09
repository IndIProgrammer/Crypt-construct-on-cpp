#ifndef UNTITLED_HEAD_H
#define UNTITLED_HEAD_H
#include <windows.h>
#include <vector>
#include <string>
//Генерация рандомного ключа-перестановки
std::vector<int> generatePermutationKey(int size);

//Проверка корректности ключа
bool validateKey(const std::vector<int>& key);

//Простая перестановка
std::string encryptSimplePermutation(const std::string& text, const std::vector<int>& key);
std::string decryptSimplePermutation(const std::string& text, const std::vector<int>& key);

//Блочная перестановка
std::string encryptBlockPermutation(const std::string& text, const std::vector<int>& key, int blockSize);
std::string decryptBlockPermutation(const std::string& text, const std::vector<int>& key, int blockSize);

//Пользовательский интерфейс
void displayMenu();
void displayKeyInputMenu();
void generateNewKey();

//Ввод ключа вручную с клавиатуры
void enterKeyManually();

//Показать текущий ключ
void showCurrentKey();

//Интерфейс для шифрования/расшифрования простой перестановки
void handleSimplePermutation(bool encrypt);

//Интерфейс для шифрования/расшифрования блочной перестановки
void handleBlockPermutation(bool encrypt);

//Корректность текущего ключа
void validateCurrentKey();
#endif
