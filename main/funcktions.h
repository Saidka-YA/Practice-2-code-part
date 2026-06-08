#ifndef FUNCKTIONS_H
#define FUNCKTIONS_H
#pragma once
#include <cstdlib>

bool isPrime(uint n);
uint gcd(uint a, uint b);
uint binaryModPow(uint base, uint exponent, uint mod);
uint fermaMethod(uint a, uint x, uint p, bool &flag);

#endif