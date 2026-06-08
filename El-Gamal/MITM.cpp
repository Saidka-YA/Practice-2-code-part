// Эмуляция атаки "Человек посередине" на шифрование Эль-Гамаля
// Сценарий: Алиса хочет отправить сообщение Бобу, Ева перехватывает и подменяет ключи
#include <iostream>
#include "ElGamalblock.h"
#include "ElGamalUtilities.h"

using namespace std;

//вывод первых N байт в hex
string hexPreview(const vector<uint8_t>& data, size_t maxBytes = 32)
{
    ostringstream oss;
    size_t limit = min(data.size(), maxBytes);
    for (size_t i = 0; i < limit; i++)
    {
        oss << hex << setw(2) << setfill('0') << (int)data[i];
        if (i + 1 < limit) oss << " ";
    }
    if (data.size() > maxBytes) oss << " ...";
    return oss.str();
}

//  Шаг 1: Алиса генерирует свои ключи и "публикует" открытый ключ
void aliceGenerateKeys(mpz_t p, mpz_t g, mpz_t y, mpz_t x)
{
    cout << "[Алиса] Генерация пары ключей...\n";
    generateKeys(p, g, y, x);

    char* pStr = mpz_get_str(NULL, 16, p);
    char* gStr = mpz_get_str(NULL, 16, g);
    char* yStr = mpz_get_str(NULL, 16, y);
    cout << "[Алиса] Открытый ключ сгенерирован:\n";
    cout << "  p (hex, первые 32 символа): " << string(pStr).substr(0, 32) << "...\n";
    cout << "  g (hex): " << gStr << "\n";
    cout << "  y (hex, первые 32 символа): " << string(yStr).substr(0, 32) << "...\n";
    free(pStr); free(gStr); free(yStr);
}

//  Шаг 2: Ева перехватывает открытый ключ Алисы и подменяет
//  его своим собственным ключом (используя те же p и g)
void eveSubstituteKey(const mpz_t p, const mpz_t g, mpz_t eveY, mpz_t eveX)
{
    cout << "\n[Ева]   Перехват открытого ключа Алисы!\n";
    cout << "[Ева]   Генерация собственной пары ключей на тех же p и g...\n";

    // Генерируем секретный ключ Евы xEve
    gmp_randstate_t state;
    gmp_randinit_default(state);
    random_device gen;
    gmp_randseed_ui(state, gen());

    mpz_t pMinus2;
    mpz_init(pMinus2);
    mpz_sub_ui(pMinus2, p, 2);

    // xEve in [2, p-2]
    mpz_urandomm(eveX, state, pMinus2);
    mpz_add_ui(eveX, eveX, 2);

    mpz_clear(pMinus2);
    gmp_randclear(state);

    // y_eve = g^xEve mod p
    modPow(eveY, g, eveX, p);

    char* yStr = mpz_get_str(NULL, 16, eveY);
    cout << "[Ева]   Поддельный открытый ключ y_eve (первые 32 символа): "
         << string(yStr).substr(0, 32) << "...\n";
    cout << "[Ева]   Подменённый ключ отправлен Бобу вместо ключа Алисы!\n";
    free(yStr);
}

//  Шаг 3: Боб шифрует сообщение ПОДДЕЛЬНЫМ ключом Евы (думая, что это ключ Алисы)
void bobEncryptMessage(const string& message,
                       vector<uint8_t>& ciphertext,
                       const mpz_t p, const mpz_t g, const mpz_t eveY)
{
    cout << "\n[Боб]   Шифрование сообщения открытым ключом (подменённым Евой)...\n";
    cout << "[Боб]   Исходное сообщение: \"" << message << "\"\n";

    // Текст -> байты
    vector<uint8_t> data(message.begin(), message.end());

    // Padding до кратности 64 байтам
    Padding(data);

    // Шифруем поддельным ключом Евы
    encryptData(data, ciphertext, p, g, eveY);

    cout << "[Боб]   Шифрование завершено. Размер шифртекста: "
         << ciphertext.size() << " байт\n";
    cout << "[Боб]   Шифртекст (hex, первые 32 байта): "
         << hexPreview(ciphertext) << "\n";
    cout << "[Боб]   Шифртекст отправлен... (перехвачен Евой!)\n";
}

//  Шаг 4: Ева расшифровывает сообщение своим закрытым ключом,
// читает его, затем перешифровывает настоящим ключом Алисы
// и пересылает дальше
void eveInterceptAndReencrypt(const vector<uint8_t>& interceptedCiphertext,
                              vector<uint8_t>& forwardedCiphertext,
                              const mpz_t p, const mpz_t g,
                              const mpz_t eveX, const mpz_t aliceY)
{
    cout << "\n[Ева]   Перехват зашифрованного сообщения!\n";

    // Расшифровываем своим закрытым ключом
    vector<uint8_t> plaintext;
    decryptData(interceptedCiphertext, plaintext, p, eveX);
    deletePadding(plaintext);

    string decryptedMessage(plaintext.begin(), plaintext.end());
    cout << "[Ева]   Сообщение успешно расшифровано!\n";
    cout << "[Ева]   Прочитанное сообщение: \"" << decryptedMessage << "\"\n";

    // Модифицируем сообщение (опционально — демонстрирует полный контроль)
    string modifiedMessage = decryptedMessage + " [изменено Евой]";
    cout << "[Ева]   Подмена содержимого: \"" << modifiedMessage << "\"\n";

    // Перешифровываем настоящим открытым ключом Алисы
    vector<uint8_t> modifiedData(modifiedMessage.begin(), modifiedMessage.end());
    Padding(modifiedData);
    encryptData(modifiedData, forwardedCiphertext, p, g, aliceY);

    cout << "[Ева]   Перешифрованное сообщение отправлено Алисе!\n";
    cout << "[Ева]   Размер перешифрованного шифртекста: "
         << forwardedCiphertext.size() << " байт\n";
}

//  Шаг 5: Алиса расшифровывает своим закрытым ключом и получает подменённое сообщение
void aliceDecryptMessage(const vector<uint8_t>& ciphertext,
                         const mpz_t p, const mpz_t x)
{
    cout << "\n[Алиса] Расшифрование полученного сообщения...\n";

    vector<uint8_t> plaintext;
    decryptData(ciphertext, plaintext, p, x);
    deletePadding(plaintext);

    string decryptedMessage(plaintext.begin(), plaintext.end());
    cout << "[Алиса] Расшифрованное сообщение: \"" << decryptedMessage << "\"\n";
    cout << "[Алиса] (Алиса не подозревает, что сообщение было изменено!)\n";
}

int main()
{
    cout << "       Эмуляция атаки \"Человек посередине\" (MITM)\n";
    cout << "            на шифрование Эль-Гамаля\n";

    cout << "Сценарий:\n";
    cout << "  Боб хочет отправить зашифрованное сообщение Алисе.\n";
    cout << "  Ева находится между ними и перехватывает все пакеты.\n\n";

    // Инициализация ключей всех участников
    // Ключи Алисы (настоящие)
    mpz_t aliceP, aliceG, aliceY, aliceX;
    mpz_init(aliceP);
    mpz_init(aliceG);
    mpz_init(aliceY);
    mpz_init(aliceX);

    // Ключи Евы (поддельные)
    mpz_t eveY, eveX;
    mpz_init(eveY);
    mpz_init(eveX);

    // Шаг 1: Алиса генерирует ключи и публикует открытый ключ
    cout << "ШАГ 1: Алиса публикует открытый ключ\n";
    aliceGenerateKeys(aliceP, aliceG, aliceY, aliceX);

    // Шаг 2: Ева подменяет открытый ключ Алисы
    cout << "\nШАГ 2: Атака — Ева подменяет открытый ключ\n";
    eveSubstituteKey(aliceP, aliceG, eveY, eveX);

    // Шаг 3: Боб шифрует сообщение (поддельным ключом Евы)
    cout << "\nШАГ 3: Боб отправляет зашифрованное сообщение\n";
    string originalMessage = "Привет, Алиса! Встретимся в 18:00.";
    vector<uint8_t> bobCiphertext;
    bobEncryptMessage(originalMessage, bobCiphertext, aliceP, aliceG, eveY);

    // Шаг 4: Ева перехватывает, читает и пересылает
    cout << "\nШАГ 4: Ева перехватывает и модифицирует сообщение\n";
    vector<uint8_t> forwardedCiphertext;
    eveInterceptAndReencrypt(bobCiphertext, forwardedCiphertext, aliceP, aliceG, eveX, aliceY);

    // Шаг 5: Алиса расшифровывает и получает подменённое сообщение
    cout << "\nШАГ 5: Алиса расшифровывает полученное сообщение\n";
    aliceDecryptMessage(forwardedCiphertext, aliceP, aliceX);

    // Итог
    cout << "                        ИТОГ АТАКИ\n";
    cout << "  Боб думал, что шифрует ключом Алисы — шифровал ключом Евы   \n";
    cout << "  Ева прочитала и изменила сообщение Боба                     \n";
    cout << "  Алиса получила изменённое сообщение и ничего не заметила    \n";
    cout << "  Защита: использование сертификатов / цифровых подписей      \n";

    // Очистка
    mpz_clear(aliceP);
    mpz_clear(aliceG);
    mpz_clear(aliceY);
    mpz_clear(aliceX);
    mpz_clear(eveY);
    mpz_clear(eveX);

    return 0;
}