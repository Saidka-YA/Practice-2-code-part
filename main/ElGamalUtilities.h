#ifndef ELGAMALUTILITIES_H
#define ELGAMALUTILITIES_H
#pragma once
#include <fstream>
#include <random>
#include <sstream>
#include <iomanip>
#include <gmp.h>

void modPow(mpz_t reslt, const mpz_t baseG, const mpz_t expnt, const mpz_t modP);
void findGenerator(mpz_t baseG, const mpz_t modP);
void generatePrime(mpz_t prime, int bits);
void Padding(std::vector<uint8_t>& data);
void deletePadding(std::vector<uint8_t>& data);
bool readFile(const std::string& path, std::vector<uint8_t>& data);
bool writeFile(const std::string& path, const std::vector<uint8_t>& data);
std::string hexDisplay(const std::vector<uint8_t>& data);
void generateKeys(mpz_t modP, mpz_t baseG, mpz_t openY, mpz_t secX);
bool saveKey(const std::string& path, const mpz_t modP, const mpz_t baseG, const mpz_t openY, const mpz_t secX);
bool loadKey(const std::string& path, mpz_t modP, mpz_t baseG, mpz_t openY, mpz_t secX);

#endif
