#include "mitmUtilities.h"
#include "ElGamalblock.h"
#include "ElGamalUtilities.h"
#include "mainUtilities.h"
using namespace std;

// Шаг 1: Алиса генерирует ключи и публикует открытый
void mitmAliceGenerateKeys(mpz_t modP, mpz_t baseG, mpz_t openY, mpz_t secX)
{
    cout << "[Алиса] Генерация пары ключей...\n";
    generateKeys(modP, baseG, openY, secX);

    char* pStr = mpz_get_str(NULL, 16, modP);
    char* gStr = mpz_get_str(NULL, 16, baseG);
    char* yStr = mpz_get_str(NULL, 16, openY);
    cout << "[Алиса] Открытый ключ сгенерирован:\n";
    cout << "  modP (hex, первые 32 символа): " << string(pStr).substr(0, 32) << "...\n";
    cout << "  baseG (hex): " << gStr << "\n";
    cout << "  openY (hex, первые 32 символа): " << string(yStr).substr(0, 32) << "...\n";
    free(pStr); free(gStr); free(yStr);
}

// Шаг 2: Ева перехватывает открытый ключ Алисы и подменяет своим
void mitmEveSubstituteKey(const mpz_t modP, const mpz_t baseG, mpz_t eveY, mpz_t eveX)
{
    cout << "\n[Ева]   Перехват открытого ключа Алисы!\n";
    cout << "[Ева]   Генерация собственной пары ключей на тех же modP и baseG...\n";

    gmp_randstate_t state;
    gmp_randinit_default(state);
    random_device gen;
    gmp_randseed_ui(state, gen());

    mpz_t modPm2;
    mpz_init(modPm2);
    mpz_sub_ui(modPm2, modP, 2);

    mpz_urandomm(eveX, state, modPm2);
    mpz_add_ui(eveX, eveX, 2);

    mpz_clear(modPm2);
    gmp_randclear(state);

    modPow(eveY, baseG, eveX, modP);

    char* yStr = mpz_get_str(NULL, 16, eveY);
    cout << "[Ева]   Поддельный открытый ключ eveY (первые 32 символа): "
         << string(yStr).substr(0, 32) << "...\n";
    cout << "[Ева]   Подменённый ключ отправлен Бобу вместо ключа Алисы!\n";
    free(yStr);
}

// Шаг 3: Боб шифрует сообщение поддельным ключом Евы
void mitmBobEncryptMessage(const string& message,
                            vector<uint8_t>& ciphr,
                            const mpz_t modP, const mpz_t baseG, const mpz_t eveY)
{
    cout << "\n[Боб]   Шифрование сообщения открытым ключом (подменённым Евой)...\n";
    cout << "[Боб]   Исходное сообщение: \"" << message << "\"\n";

    vector<uint8_t> data(message.begin(), message.end());
    Padding(data);
    encryptData(data, ciphr, modP, baseG, eveY);

    cout << "[Боб]   Шифрование завершено. Размер шифртекста: "
         << ciphr.size() << " байт\n";
    cout << "[Боб]   Шифртекст (hex, первые 32 байта): "
         << hexPreview(ciphr) << "\n";
    cout << "[Боб]   Шифртекст отправлен... (перехвачен Евой!)\n";
}

// Шаг 4: Ева расшифровывает, читает, модифицирует и перешифровывает
void mitmEveInterceptAndReencrypt(const vector<uint8_t>& intCiph,
                                   vector<uint8_t>& fwdCiph,
                                   const mpz_t modP, const mpz_t baseG,
                                   const mpz_t eveX, const mpz_t alceY)
{
    cout << "\n[Ева]   Перехват зашифрованного сообщения!\n";

    vector<uint8_t> plain;
    decryptData(intCiph, plain, modP, eveX);
    deletePadding(plain);

    string decMsg(plain.begin(), plain.end());
    cout << "[Ева]   Сообщение успешно расшифровано!\n";
    cout << "[Ева]   Прочитанное сообщение: \"" << decMsg << "\"\n";

    string modMsg = decMsg + " [изменено Евой]";
    cout << "[Ева]   Подмена содержимого: \"" << modMsg << "\"\n";

    vector<uint8_t> modData(modMsg.begin(), modMsg.end());
    Padding(modData);
    encryptData(modData, fwdCiph, modP, baseG, alceY);

    cout << "[Ева]   Перешифрованное сообщение отправлено Алисе!\n";
    cout << "[Ева]   Размер перешифрованного шифртекста: "
         << fwdCiph.size() << " байт\n";
}

// Шаг 5: Алиса расшифровывает и получает подменённое сообщение
void mitmAliceDecryptMessage(const vector<uint8_t>& ciphr,
                              const mpz_t modP, const mpz_t secX)
{
    cout << "\n[Алиса] Расшифрование полученного сообщения...\n";

    vector<uint8_t> plain;
    decryptData(ciphr, plain, modP, secX);
    deletePadding(plain);

    string decMsg(plain.begin(), plain.end());
    cout << "[Алиса] Расшифрованное сообщение: \"" << decMsg << "\"\n";
    cout << "[Алиса] (Алиса не подозревает, что сообщение было изменено!)\n";
}
