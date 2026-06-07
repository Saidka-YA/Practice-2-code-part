#ifndef ELGAMALUTILITIES_H
#define ELGAMALUTILITIES_H
#pragma once
#include <fstream>
#include <random>
#include <sstream>
#include <iomanip>
#include <gmp.h>

void modPow(mpz_t result, const mpz_t base, const mpz_t exp, const mpz_t mod);
void findGenerator(mpz_t g, const mpz_t p);
void generatePrime(mpz_t prime, int bits);
void Padding(std::vector<uint8_t>& data);
void deletePadding(std::vector<uint8_t>& data);
bool readFile(const std::string& path, std::vector<uint8_t>& data);
bool writeFile(const std::string& path, const std::vector<uint8_t>& data);
std::string hexDisplay(const std::vector<uint8_t>& data);
void generateKeys(mpz_t p, mpz_t g, mpz_t y, mpz_t x);
bool saveKey(const string& path, const mpz_t p, const mpz_t g, const mpz_t y, const mpz_t x);
bool loadKey(const string& path, mpz_t p, mpz_t g, mpz_t y, mpz_t x);

#endif