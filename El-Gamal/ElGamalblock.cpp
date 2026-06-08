#include "ElGamalUtilities.h"
using namespace std;

// Функция шифрования блока
// input - массив байт для шифрования
// output - массив для зашифрованных данных (c1 и c2)
// blockSize - размер блока в байтах
void encryptBlock(const uint8_t* input, int blockSize, mpz_t c1, mpz_t c2, 
                  const mpz_t p, const mpz_t g, const mpz_t y)
{
    mpz_t message, k, temp;
    mpz_init(message);
    mpz_init(k);
    mpz_init(temp);
    
    // Преобразуем байты в mpz_t
    mpz_import(message, blockSize, 1, sizeof(uint8_t), 0, 0, input);
    
    // Приводим сообщение в допустимый диапазон
    mpz_mod(message, message, p);
    
    // Генерируем случайное k
    gmp_randstate_t state;
    gmp_randinit_default(state);
    random_device gen;
    gmp_randseed_ui(state, gen());
    
    mpz_t pMinus2;
    mpz_init(pMinus2);
    mpz_sub_ui(pMinus2, p, 2);
    
    mpz_urandomm(k, state, pMinus2);
    mpz_add_ui(k, k, 2);
    
    mpz_clear(pMinus2);
    
    // Вычисляем c1 = g^k mod p
    modPow(c1, g, k, p);
    
    // Вычисляем c2 = m * y^k mod p
    modPow(temp, y, k, p);
    mpz_mul(c2, message, temp);
    mpz_mod(c2, c2, p);
    
    mpz_clear(message);
    mpz_clear(k);
    mpz_clear(temp);
    gmp_randclear(state);
}

// Функция расшифрования блока
void decryptBlock(mpz_t message, const mpz_t c1, const mpz_t c2, 
                  const mpz_t p, const mpz_t x)
{
    mpz_t s, sInv, pMinus2;
    mpz_init(s);
    mpz_init(sInv);
    mpz_init(pMinus2);
    
    // Вычисляем s = c1^x mod p
    modPow(s, c1, x, p);
    
    // Вычисляем обратный элемент s^(-1) mod p
    // Используем теорему Ферма: s^(-1) = s^(p-2) mod p
    mpz_sub_ui(pMinus2, p, 2);
    modPow(sInv, s, pMinus2, p);
    
    // Вычисляем m = c2 * s^(-1) mod p
    mpz_mul(message, c2, sInv);
    mpz_mod(message, message, p);
    
    mpz_clear(s);
    mpz_clear(sInv);
    mpz_clear(pMinus2);
}

// Функция шифрования данных
void encryptData(const vector<uint8_t>& plaintext, vector<uint8_t>& ciphertext, 
                 const mpz_t p, const mpz_t g, const mpz_t y)
{
    size_t size = plaintext.size();
    int blockSize = 64; // Размер блока 64 байта
    
    mpz_t c1, c2;
    mpz_init(c1);
    mpz_init(c2);
    
    // Ходим блоками по 64 байта
    for(size_t i = 0; i < size; i += blockSize)
    {
        // Шифруем блок
        encryptBlock(&plaintext[i], blockSize, c1, c2, p, g, y);
        
        // Получаем размер c1 и c2 в байтах
        size_t c1Size = (mpz_sizeinbase(c1, 2) + 7) / 8;
        size_t c2Size = (mpz_sizeinbase(c2, 2) + 7) / 8;
        
        // Записываем размер c1 (2 байта)
        ciphertext.push_back((c1Size >> 8) & 0xFF);
        ciphertext.push_back(c1Size & 0xFF);
        
        // Записываем c1
        vector<uint8_t> c1Bytes(c1Size);
        mpz_export(&c1Bytes[0], NULL, 1, sizeof(uint8_t), 0, 0, c1);
        for(size_t j = 0; j < c1Size; j++)
        {
            ciphertext.push_back(c1Bytes[j]);
        }
        
        // Записываем размер c2 (2 байта)
        ciphertext.push_back((c2Size >> 8) & 0xFF);
        ciphertext.push_back(c2Size & 0xFF);
        
        // Записываем c2
        vector<uint8_t> c2Bytes(c2Size);
        mpz_export(&c2Bytes[0], NULL, 1, sizeof(uint8_t), 0, 0, c2);
        for(size_t j = 0; j < c2Size; j++)
        {
            ciphertext.push_back(c2Bytes[j]);
        }
    }
    
    mpz_clear(c1);
    mpz_clear(c2);
}

// Функция расшифрования данных
void decryptData(const vector<uint8_t>& ciphertext, vector<uint8_t>& plaintext, 
                 const mpz_t p, const mpz_t x)
{
    size_t pos = 0;
    int blockSize = 64; // Размер расшифрованного блока
    
    mpz_t c1, c2, message;
    mpz_init(c1);
    mpz_init(c2);
    mpz_init(message);
    
    while(pos < ciphertext.size())
    {
        // Защита от выхода за границу массива
        if(pos + 2 > ciphertext.size()) break;
        
        // Читаем размер c1
        size_t c1Size = ((size_t)ciphertext[pos] << 8) | ciphertext[pos + 1];
        pos += 2;
        
        if(pos + c1Size > ciphertext.size()) break;
        
        // Читаем c1
        mpz_import(c1, c1Size, 1, sizeof(uint8_t), 0, 0, &ciphertext[pos]);
        pos += c1Size;
        
        if(pos + 2 > ciphertext.size()) break;
        
        // Читаем размер c2
        size_t c2Size = ((size_t)ciphertext[pos] << 8) | ciphertext[pos + 1];
        pos += 2;
        
        if(pos + c2Size > ciphertext.size()) break;
        
        // Читаем c2
        mpz_import(c2, c2Size, 1, sizeof(uint8_t), 0, 0, &ciphertext[pos]);
        pos += c2Size;
        
        // Расшифровываем блок
        decryptBlock(message, c1, c2, p, x);
        
        // Преобразуем обратно в байты с выравниванием вправо (сохраняем ведущие нули)
        vector<uint8_t> blockBytes(blockSize, 0); // Инициализируем нулями
        size_t count;
        vector<uint8_t> exportBuf(blockSize, 0);
        mpz_export(&exportBuf[0], &count, 1, sizeof(uint8_t), 0, 0, message);
        
        // Копируем в конец блока, ведущие нули остаются в начале
        for(size_t j = 0; j < count; j++)
        {
            blockBytes[blockSize - count + j] = exportBuf[j];
        }
        
        // Записываем расшифрованный блок
        for(int j = 0; j < blockSize; j++)
        {
            plaintext.push_back(blockBytes[j]);
        }
    }
    
    mpz_clear(c1);
    mpz_clear(c2);
    mpz_clear(message);
}

