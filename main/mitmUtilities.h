#ifndef MITMUTILITIES_H
#define MITMUTILITIES_H
#pragma once
#include <vector>
#include "gmp.h"
#include <cstdint>

void mitmAliceGenerateKeys(mpz_t modP, mpz_t baseG, mpz_t openY, mpz_t secX);
void mitmEveSubstituteKey(const mpz_t modP, const mpz_t baseG, mpz_t eveY, mpz_t eveX);
void mitmBobEncryptMessage(const std::string& message, std::vector<uint8_t>& ciphr,
                            const mpz_t modP, const mpz_t baseG, const mpz_t eveY);
void mitmEveInterceptAndReencrypt(const std::vector<uint8_t>& intCiph,
                                   std::vector<uint8_t>& fwdCiph,
                                   const mpz_t modP, const mpz_t baseG,
                                   const mpz_t eveX, const mpz_t alceY);
void mitmAliceDecryptMessage(const std::vector<uint8_t>& ciphr,
                             const mpz_t modP, const mpz_t secX);

#endif
