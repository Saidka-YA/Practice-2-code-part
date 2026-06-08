#ifndef MAINUTILITIES_H
#define MAINUTILITIES_H
#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <cstdint>

// Перечисление пунктов главного меню
enum class MenuOption
{
    ModularExponentiation = 1,  // Задание 1: a^x mod p
    InverseViaCandD       = 2,  // Задание 2: c*d mod m = 1
    InverseCmod           = 3,  // Задание 3: c^-1 mod m = d
    ElGamalCipher         = 4,  // Задание 4: Шифр Эль-Гамаля
    MitmAttack            = 5,  // Задание 5: Атака "Человек посередине"
    ContinuedFraction     = 6,  // Задание 6: Цепные дроби и уравнение
    Exit                  = 0   // Выход
};

std::string hexPreview(const std::vector<uint8_t>& data, size_t maxBytes = 32);
int readInt(const std::string& prompt);
uint readUint(const std::string& prompt);
void printMenu();
void runModularExponentiation();
void runInverseViaCandD();
void runInverseCmod();
void runElGamalCipher();
void runMitmAttack();
void runContinuedFraction();

#endif