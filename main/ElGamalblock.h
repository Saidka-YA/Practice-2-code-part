#ifndef ELGAMALBLOCK_H
#define ELGAMALBLOCK_H
#pragma once
#include <vector>
#include <random>
#include <gmp.h>

void encryptBlock(const uint8_t* inBuf, int blkSz, mpz_t ciph1, mpz_t ciph2,
                  const mpz_t modP, const mpz_t baseG, const mpz_t openY);
void decryptBlock(mpz_t msgOut, const mpz_t ciph1, const mpz_t ciph2,
                  const mpz_t modP, const mpz_t secX);
void encryptData(const std::vector<uint8_t>& plain, std::vector<uint8_t>& ciphr,
                 const mpz_t modP, const mpz_t baseG, const mpz_t openY);
void decryptData(const std::vector<uint8_t>& ciphr, std::vector<uint8_t>& plain,
                 const mpz_t modP, const mpz_t secX);

#endif
