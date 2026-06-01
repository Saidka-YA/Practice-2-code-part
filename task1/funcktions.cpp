#include "funcktions.h"

bool isPrime(uint n) 
{
    if (n <= 1) 
    {
        return false;
    }
    if (n <= 3) 
    {
        return true;
    }
    if (n % 2 == 0 || n % 3 == 0) 
    {
        return false;
    }
    
    //Проверяем делители вида 6k +- 1 до sqrt(n)
    //Все простые числа > 3 имеют вид 6k +- 1
    for (uint i = 5; i * i <= n; i += 6) 
    {
        if (n % i == 0 || n % (i + 2) == 0) 
        {
            return false;
        }
    }
    return true;
}

// Алгоритм Евклида
uint gcd(uint a, uint b) 
{
    while (b != 0) 
    {
        uint temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

// Бинарное возведение в степень
uint binaryModPow(uint base, uint exponent, uint mod) 
{
    uint result = 1;
    // Берём основание по модулю чтобы не работать с большими числами
    base = base % mod;
    // Ходим пока степень не равна 0
    while (exponent > 0) 
    {
        // Проверка младшего бита степени, если равно значит входит в разложение
        if (exponent % 2 == 1) 
        {
            // Умножаем результат на текущее основание по модулю
            result = (result * base) % mod;
        }
        // Возводим основание в квадрат для следующего бита
        base = (base * base) % mod;
        // Переход к следующему биту
        exponent = exponent / 2;
    }
    return result;
}

uint fermaMethod(uint a, uint x, uint p, bool &flag) 
{
    // Проверка 1: p должно быть простым числом
    if (!isPrime(p)) 
    {
        flag = false;
        return 0;
    }
    // Проверка 2: a не должно делиться на p
    if (a % p == 0) 
    {
        flag = false;
        return 0;
    }
    // Проверка 3: НОД(a, p) должен быть равен 1
    if (gcd(a, p) != 1) 
    {
        flag = false;
        return 0;
    }
    // Уменьшаем степень по теореме Ферма
    x = x % (p - 1);
    
    // Простое возведение в степень циклом
    uint result = 1;
    for (uint i = 0; i < x; i++) 
    {
        result = (result * a) % p;
    }
    
    flag = true;
    return result;;
}
