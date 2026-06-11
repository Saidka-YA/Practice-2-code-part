#include "ElGamalUtilities.h"
using namespace std;

// Функция шифрования блока
// inBuf - массив байт для шифрования
// ciphr - массив для зашифрованных данных (ciph1 и ciph2)
// blkSz - размер блока в байтах
void encryptBlock(const uint8_t* inBuf, int blkSz, mpz_t ciph1, mpz_t ciph2,
                  const mpz_t modP, const mpz_t baseG, const mpz_t openY)
{
    mpz_t msgIn, randK, temp;
    mpz_init(msgIn);
    mpz_init(randK);
    mpz_init(temp);

    // Преобразуем байты в mpz_t
    mpz_import(msgIn, blkSz, 1, sizeof(uint8_t), 0, 0, inBuf);

    // Приводим сообщение в допустимый диапазон
    mpz_mod(msgIn, msgIn, modP);

    // Генерируем случайное randK
    gmp_randstate_t state;
    gmp_randinit_default(state);
    random_device gen;
    gmp_randseed_ui(state, gen());

    mpz_t modPm2;
    mpz_init(modPm2);
    mpz_sub_ui(modPm2, modP, 2);

    mpz_urandomm(randK, state, modPm2);
    mpz_add_ui(randK, randK, 2);

    mpz_clear(modPm2);

    // Вычисляем ciph1 = baseG^randK mod modP
    modPow(ciph1, baseG, randK, modP);

    // Вычисляем ciph2 = msgIn * openY^randK mod modP
    modPow(temp, openY, randK, modP);
    mpz_mul(ciph2, msgIn, temp);
    mpz_mod(ciph2, ciph2, modP);

    mpz_clear(msgIn);
    mpz_clear(randK);
    mpz_clear(temp);
    gmp_randclear(state);
}

// Функция расшифрования блока
void decryptBlock(mpz_t msgOut, const mpz_t ciph1, const mpz_t ciph2,
                  const mpz_t modP, const mpz_t secX)
{
    mpz_t shareS, invS, modPm2;
    mpz_init(shareS);
    mpz_init(invS);
    mpz_init(modPm2);

    // Вычисляем shareS = ciph1^secX mod modP
    modPow(shareS, ciph1, secX, modP);

    // Вычисляем обратный элемент invS = shareS^(-1) mod modP
    // Используем теорему Ферма: shareS^(-1) = shareS^(modP-2) mod modP
    mpz_sub_ui(modPm2, modP, 2);
    modPow(invS, shareS, modPm2, modP);

    // Вычисляем msgOut = ciph2 * invS mod modP
    mpz_mul(msgOut, ciph2, invS);
    mpz_mod(msgOut, msgOut, modP);

    mpz_clear(shareS);
    mpz_clear(invS);
    mpz_clear(modPm2);
}

// Функция шифрования данных
void encryptData(const vector<uint8_t>& plain, vector<uint8_t>& ciphr,
                 const mpz_t modP, const mpz_t baseG, const mpz_t openY)
{
    size_t size = plain.size();
    int blkSz = 64; // Размер блока 64 байта

    mpz_t ciph1, ciph2;
    mpz_init(ciph1);
    mpz_init(ciph2);

    // Ходим блоками по 64 байта
    for(size_t i = 0; i < size; i += blkSz)
    {
        // Шифруем блок
        encryptBlock(&plain[i], blkSz, ciph1, ciph2, modP, baseG, openY);

        // Получаем размер ciph1 и ciph2 в байтах
        size_t c1Sz = (mpz_sizeinbase(ciph1, 2) + 7) / 8;
        size_t c2Sz = (mpz_sizeinbase(ciph2, 2) + 7) / 8;

        // Записываем размер ciph1 (2 байта)
        ciphr.push_back((c1Sz >> 8) & 0xFF);
        ciphr.push_back(c1Sz & 0xFF);

        // Записываем ciph1
        vector<uint8_t> c1By(c1Sz);
        mpz_export(&c1By[0], NULL, 1, sizeof(uint8_t), 0, 0, ciph1);
        for(size_t j = 0; j < c1Sz; j++)
        {
            ciphr.push_back(c1By[j]);
        }

        // Записываем размер ciph2 (2 байта)
        ciphr.push_back((c2Sz >> 8) & 0xFF);
        ciphr.push_back(c2Sz & 0xFF);

        // Записываем ciph2
        vector<uint8_t> c2By(c2Sz);
        mpz_export(&c2By[0], NULL, 1, sizeof(uint8_t), 0, 0, ciph2);
        for(size_t j = 0; j < c2Sz; j++)
        {
            ciphr.push_back(c2By[j]);
        }
    }

    mpz_clear(ciph1);
    mpz_clear(ciph2);
}

// Функция расшифрования данных
void decryptData(const vector<uint8_t>& ciphr, vector<uint8_t>& plain,
                 const mpz_t modP, const mpz_t secX)
{
    size_t pos = 0;
    int blkSz = 64; // Размер расшифрованного блока

    mpz_t ciph1, ciph2, msgOut;
    mpz_init(ciph1);
    mpz_init(ciph2);
    mpz_init(msgOut);

    while(pos < ciphr.size())
    {
        // Защита от выхода за границу массива
        if(pos + 2 > ciphr.size()) break;

        // Читаем размер ciph1
        size_t c1Sz = ((size_t)ciphr[pos] << 8) | ciphr[pos + 1];
        pos += 2;

        if(pos + c1Sz > ciphr.size()) break;

        // Читаем ciph1
        mpz_import(ciph1, c1Sz, 1, sizeof(uint8_t), 0, 0, &ciphr[pos]);
        pos += c1Sz;

        if(pos + 2 > ciphr.size()) break;

        // Читаем размер ciph2
        size_t c2Sz = ((size_t)ciphr[pos] << 8) | ciphr[pos + 1];
        pos += 2;

        if(pos + c2Sz > ciphr.size()) break;

        // Читаем ciph2
        mpz_import(ciph2, c2Sz, 1, sizeof(uint8_t), 0, 0, &ciphr[pos]);
        pos += c2Sz;

        // Расшифровываем блок
        decryptBlock(msgOut, ciph1, ciph2, modP, secX);

        // Преобразуем обратно в байты с выравниванием вправо (сохраняем ведущие нули)
        vector<uint8_t> blkBy(blkSz, 0); // Инициализируем нулями
        size_t count;
        vector<uint8_t> expBuf(blkSz, 0);
        mpz_export(&expBuf[0], &count, 1, sizeof(uint8_t), 0, 0, msgOut);

        // Копируем в конец блока, ведущие нули остаются в начале
        for(size_t j = 0; j < count; j++)
        {
            blkBy[blkSz - count + j] = expBuf[j];
        }

        // Записываем расшифрованный блок
        for(int j = 0; j < blkSz; j++)
        {
            plain.push_back(blkBy[j]);
        }
    }

    mpz_clear(ciph1);
    mpz_clear(ciph2);
    mpz_clear(msgOut);
}
