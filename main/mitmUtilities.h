#ifndef MITMUTILITIES_H
#define MITMUTILITIES_H
#pragma once
#include <vector>
#include "gmp.h"
#include <cstdint>

void mitmAliceGenerateKeys(mpz_t p, mpz_t g, mpz_t y, mpz_t x);
void mitmEveSubstituteKey(const mpz_t p, const mpz_t g, mpz_t eveY, mpz_t eveX);
void mitmBobEncryptMessage(const std::string& message, std::vector<uint8_t>& ciphertext,
                            const mpz_t p, const mpz_t g, const mpz_t eveY);
void mitmEveInterceptAndReencrypt(const std::vector<uint8_t>& interceptedCiphertext,
                                   std::vector<uint8_t>& forwardedCiphertext,
                                   const mpz_t p, const mpz_t g,
                                   const mpz_t eveX, const mpz_t aliceY);
void mitmAliceDecryptMessage(const std::vector<uint8_t>& ciphertext,
                             const mpz_t p, const mpz_t x);

#endif