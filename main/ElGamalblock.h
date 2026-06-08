#ifndef ELGAMALBLOCK_H
#define ELGAMALBLOCK_H
#pragma once
#include <vector>
#include <random>
#include <gmp.h>

void encryptBlock(const uint8_t* input, int blockSize, mpz_t c1, mpz_t c2, 
                  const mpz_t p, const mpz_t g, const mpz_t y);
void decryptBlock(mpz_t message, const mpz_t c1, const mpz_t c2, 
                  const mpz_t p, const mpz_t x);
void encryptData(const std::vector<uint8_t>& plaintext, std::vector<uint8_t>& ciphertext, 
                 const mpz_t p, const mpz_t g, const mpz_t y);
void decryptData(const std::vector<uint8_t>& ciphertext, std::vector<uint8_t>& plaintext, 
                 const mpz_t p, const mpz_t x);
  
#endif