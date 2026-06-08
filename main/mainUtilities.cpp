#include "mainUtilities.h"
#include "funcktions.h"
#include "extendedGCD.h"
#include "funck.h"
#include "ElGamalblock.h"
#include "ElGamalUtilities.h"
#include "mitmUtilities.h"
using namespace std;

// вывод hex-превью первых N байт
string hexPreview(const vector<uint8_t>& data, size_t maxBytes)
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

// чтение int с проверкой
int readInt(const string& prompt)
{
    int value;
    cout << prompt;
    while (!(cin >> value))
    {
        cerr << "Ошибка: введите целое число!\n";
        cin.clear();
        cin.ignore(1000, '\n');
        cout << prompt;
    }
    return value;
}

// чтение uint с проверкой
uint readUint(const string& prompt)
{
    uint value;
    cout << prompt;
    while (!(cin >> value))
    {
        cerr << "Ошибка: введите положительное число!\n";
        cin.clear();
        cin.ignore(1000, '\n');
        cout << prompt;
    }
    return value;
}

// Главное меню
void printMenu()
{
    cout << "\n========================================\n";
    cout << "           Практическая работа №2\n";
    cout << "========================================\n";
    cout << "  1 - Модульное возведение в степень (a^x mod p)\n";
    cout << "  2 - Расширенный алгоритм Евклида (c*d mod m = 1)\n";
    cout << "  3 - Расширенный алгоритм Евклида (c^-1 mod m = d)\n";
    cout << "  4 - Шифрование Эль-Гамаля\n";
    cout << "  5 - Атака MITM на шифр Эль-Гамаля\n";
    cout << "  6 - Цепные дроби и диофантово уравнение\n";
    cout << "  0 - Выход\n";
    cout << "==========================================\n";
    cout << "Выбор: ";
}

//===========Дальше блок функций в которых собрана реализация заданий==========

// Задание 1: Модульное возведение в степень (теорема Ферма + бинарный алгоритм)
void runModularExponentiation()
{
    cout << "\n--- Задание 1: a^x mod p ---\n\n";

    uint base = readUint("Введите основание a: ");
    uint exponent = readUint("Введите степень x: ");

    uint modul;
    cout << "Введите модуль p: ";
    while (!(cin >> modul) || modul == 0)
    {
        cerr << "Ошибка: введите число не равное нулю!\n";
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Введите модуль p: ";
    }

    cout << "\nВычисляем: " << base << "^" << exponent << " mod " << modul << "\n\n";

    // Метод теоремы Ферма
    bool fermaFlag = false;
    uint fermaResult = fermaMethod(base, exponent, modul, fermaFlag);

    if (fermaFlag)
    {
        cout << "Метод 1 (теорема Ферма): " << base << "^" << exponent
             << " mod " << modul << " = " << fermaResult << "\n";
        cout << "(степень уменьшена: " << exponent << " mod " << (modul - 1)
             << " = " << (exponent % (modul - 1)) << ")\n";
    }
    else
    {
        cout << "Метод 1 (теорема Ферма): не работает\n";
        if (!isPrime(modul))
        {
            cout << "\tПричина: " << modul << " не является простым числом\n";
        }
        else if (base % modul == 0)
        {
            cout << "\tПричина: " << base << " делится на " << modul << "\n";
        }
        else
        {
            cout << "\tПричина: НОД(" << base << ", " << modul << ") != 1\n";
        }
    }

    // Бинарный метод
    uint binaryResult = binaryModPow(base, exponent, modul);
    cout << "Метод 2 (бинарный алгоритм): " << base << "^" << exponent
         << " mod " << modul << " = " << binaryResult << "\n\n";

    // Сравнение результатов
    if (fermaFlag)
    {
        if (fermaResult == binaryResult)
        {
            cout << "Результаты совпадают: " << fermaResult << "\n";
        }
        else
        {
            cout << "Ошибка: результаты различаются!\n";
            cout << "  Ферма:    " << fermaResult << "\n";
            cout << "  Бинарный: " << binaryResult << "\n";
        }
    }
    else
    {
        cout << "Результат вычисления: " << binaryResult << "\n";
    }

    // Дополнительная проверка теоремы Ферма
    if (isPrime(modul) && base % modul != 0)
    {
        uint fermaCheck = binaryModPow(base, modul - 1, modul);
        cout << "\nПроверка теоремы Ферма: " << base << "^" << (modul - 1)
             << " mod " << modul << " = " << fermaCheck;
        if (fermaCheck == 1)
        {
            cout << "\t(теорема работает)\n";
        }
        else
        {
            cout << "\t(ошибка проверки)\n";
        }
    }
}

// Задание 2: Расширенный алгоритм Евклида — c * d mod m = 1
void runInverseViaCandD()
{
    cout << "\n--- Задание 2: c * d mod m = 1 (расширенный алгоритм Евклида) ---\n\n";

    int m = readInt("Введите число m: ");
    int c = readInt("Введите число c: ");

    int x, y, d;
    gcdExtended(m, c, x, y, d);

    cout << "Введённое число c: " << c << "\n";
    cout << "Введённое число m: " << m << "\n";

    if (d != 1)
    {
        cout << "Обратный элемент не существует, так как НОД(" 
             << c << ", " << m << ") != 1\n";
    }
    else
    {
        int inverse = (y % m + m) % m;
        cout << "Найденное число d: " << inverse << "\n";
        cout << "Проверка условия c * d mod m = 1: " << (c * inverse) % m << "\n";
    }
}

// Задание 3: Расширенный алгоритм Евклида — c^-1 mod m = d
void runInverseCmod()
{
    cout << "\n--- Задание 3: c^-1 mod m = d (расширенный алгоритм Евклида) ---\n\n";

    int c = readInt("Введите число c: ");
    int m = readInt("Введите число m: ");

    int x, y, d;
    gcdExtended(c, m, x, y, d);

    cout << "Введённое число c: " << c << "\n";
    cout << "Введённое число m: " << m << "\n";
    cout << "Найденный обратный элемент: " << x << "\n";
    cout << "Проверка условия c^-1 mod m = d: " << y % m << "\n";
}

// Задание 4: Шифр Эль-Гамаля
void runElGamalCipher()
{
    cout << "\n--- Задание 4: Шифрование Эль-Гамаля ---\n\n";
    cout << "1 - Зашифровать файл\n";
    cout << "2 - Расшифровать файл\n";
    cout << "3 - Зашифровать текст (консоль)\n";
    cout << "Выбор: ";

    int cipherChoice;
    cin >> cipherChoice;
    cin.ignore();

    string keyPath = "elgamalKey.bin";

    mpz_t p, g, y, x;
    mpz_init(p);
    mpz_init(g);
    mpz_init(y);
    mpz_init(x);

    if (cipherChoice == 1)
    {
        string inputPath, outputPath;
        cout << "Введите путь к файлу: ";
        getline(cin, inputPath);
        cout << "Введите путь для сохранения результата: ";
        getline(cin, outputPath);

        vector<uint8_t> data;
        if (!readFile(inputPath, data))
        {
            cerr << "Ошибка чтения файла\n";
            mpz_clear(p); mpz_clear(g); mpz_clear(y); mpz_clear(x);
            return;
        }
        cout << "Файл прочитан. Размер: " << data.size() << " байт\n";

        cout << "Генерация ключей...\n";
        generateKeys(p, g, y, x);

        char* pStr = mpz_get_str(NULL, 16, p);
        char* gStr = mpz_get_str(NULL, 16, g);
        char* yStr = mpz_get_str(NULL, 16, y);
        char* xStr = mpz_get_str(NULL, 16, x);
        cout << "  p (hex): " << pStr << "\n";
        cout << "  g (hex): " << gStr << "\n";
        cout << "  y (hex): " << yStr << "\n";
        cout << "  x (hex): " << xStr << "\n";
        free(pStr); free(gStr); free(yStr); free(xStr);

        if (!saveKey(keyPath, p, g, y, x))
        {
            cerr << "Ошибка сохранения ключа\n";
            mpz_clear(p); mpz_clear(g); mpz_clear(y); mpz_clear(x);
            return;
        }
        cout << "Ключи сохранены в " << keyPath << "\n";

        Padding(data);
        cout << "Padding применён. Размер после padding: " << data.size() << " байт\n";

        cout << "Шифрование...\n";
        vector<uint8_t> ciphertext;
        encryptData(data, ciphertext, p, g, y);
        cout << "Шифрование завершено. Размер шифртекста: " << ciphertext.size() << " байт\n";

        if (!writeFile(outputPath, ciphertext))
        {
            cerr << "Ошибка записи файла\n";
        }
        else
        {
            cout << "Файл зашифрован. Ключ сохранён в " << keyPath << "\n";
        }
    }
    else if (cipherChoice == 2)
    {
        string inputPath, outputPath;
        cout << "Введите путь к файлу: ";
        getline(cin, inputPath);
        cout << "Введите путь для сохранения результата: ";
        getline(cin, outputPath);

        if (!loadKey(keyPath, p, g, y, x))
        {
            cerr << "Ошибка загрузки ключа\n";
            mpz_clear(p); mpz_clear(g); mpz_clear(y); mpz_clear(x);
            return;
        }

        char* pStr = mpz_get_str(NULL, 16, p);
        char* gStr = mpz_get_str(NULL, 16, g);
        char* yStr = mpz_get_str(NULL, 16, y);
        char* xStr = mpz_get_str(NULL, 16, x);
        cout << "Ключи загружены из " << keyPath << "\n";
        cout << "  p (hex): " << pStr << "\n";
        cout << "  g (hex): " << gStr << "\n";
        cout << "  y (hex): " << yStr << "\n";
        cout << "  x (hex): " << xStr << "\n";
        free(pStr); free(gStr); free(yStr); free(xStr);

        vector<uint8_t> ciphertext;
        if (!readFile(inputPath, ciphertext))
        {
            cerr << "Ошибка чтения файла\n";
            mpz_clear(p); mpz_clear(g); mpz_clear(y); mpz_clear(x);
            return;
        }
        cout << "Файл прочитан. Размер шифртекста: " << ciphertext.size() << " байт\n";

        cout << "Расшифрование...\n";
        vector<uint8_t> plaintext;
        decryptData(ciphertext, plaintext, p, x);
        deletePadding(plaintext);
        cout << "Расшифрование завершено. Размер открытого текста: " 
             << plaintext.size() << " байт\n";

        if (!writeFile(outputPath, plaintext))
        {
            cerr << "Ошибка записи файла\n";
        }
        else
        {
            cout << "Файл расшифрован.\n";
        }
    }
    else if (cipherChoice == 3)
    {
        cout << "Ввод текста с переносами строк будет обработан неверно!\n";
        cout << "Для расшифрования/дешифрования большого текста "
             << "используйте режим расшифрования файлов\n";
        cout << "Введите текст для шифрования: ";
        string text;
        getline(cin, text);

        vector<uint8_t> data(text.begin(), text.end());
        cout << "Размер входных данных: " << data.size() << " байт\n";

        cout << "Генерация ключей...\n";
        generateKeys(p, g, y, x);

        char* pStr = mpz_get_str(NULL, 16, p);
        char* gStr = mpz_get_str(NULL, 16, g);
        char* yStr = mpz_get_str(NULL, 16, y);
        char* xStr = mpz_get_str(NULL, 16, x);
        cout << "  p (hex): " << pStr << "\n";
        cout << "  g (hex): " << gStr << "\n";
        cout << "  y (hex): " << yStr << "\n";
        cout << "  x (hex): " << xStr << "\n";
        free(pStr); free(gStr); free(yStr); free(xStr);

        saveKey(keyPath, p, g, y, x);
        cout << "Ключи сохранены в " << keyPath << "\n";

        Padding(data);
        cout << "Padding применён. Размер после padding: " << data.size() << " байт\n";

        cout << "Шифрование...\n";
        vector<uint8_t> ciphertext;
        encryptData(data, ciphertext, p, g, y);
        cout << "Шифрование завершено. Размер шифртекста: " << ciphertext.size() << " байт\n";

        cout << "\nЗашифрованный текст (hex):\n";
        cout << hexDisplay(ciphertext) << "\n";

        cout << "\nРасшифрование...\n";
        vector<uint8_t> plaintext;
        decryptData(ciphertext, plaintext, p, x);
        deletePadding(plaintext);
        cout << "Расшифрование завершено. Размер открытого текста: "
             << plaintext.size() << " байт\n";

        string decrypted(plaintext.begin(), plaintext.end());
        cout << "\nРасшифрованный текст:\n" << decrypted << "\n";
    }
    else
    {
        cerr << "Неверный выбор\n";
    }

    mpz_clear(p); mpz_clear(g); mpz_clear(y); mpz_clear(x);
}

// Задание 5: Эмуляция атаки "Человек посередине" на шифр Эль-Гамаля
void runMitmAttack()
{
    cout << "\n--- Задание 5: Атака \"Человек посередине\" (MITM) ---\n\n";
    cout << "Сценарий:\n";
    cout << "  Боб хочет отправить зашифрованное сообщение Алисе.\n";
    cout << "  Ева находится между ними и перехватывает все пакеты.\n\n";

    mpz_t aliceP, aliceG, aliceY, aliceX;
    mpz_init(aliceP); mpz_init(aliceG);
    mpz_init(aliceY); mpz_init(aliceX);

    mpz_t eveY, eveX;
    mpz_init(eveY); mpz_init(eveX);

    cout << "ШАГ 1: Алиса публикует открытый ключ\n";
    mitmAliceGenerateKeys(aliceP, aliceG, aliceY, aliceX);

    cout << "\nШАГ 2: Атака — Ева подменяет открытый ключ\n";
    mitmEveSubstituteKey(aliceP, aliceG, eveY, eveX);

    cout << "\nШАГ 3: Боб отправляет зашифрованное сообщение\n";
    string originalMessage = "Привет, Алиса! Встретимся в 18:00.";
    vector<uint8_t> bobCiphertext;
    mitmBobEncryptMessage(originalMessage, bobCiphertext, aliceP, aliceG, eveY);

    cout << "\nШАГ 4: Ева перехватывает и модифицирует сообщение\n";
    vector<uint8_t> forwardedCiphertext;
    mitmEveInterceptAndReencrypt(bobCiphertext, forwardedCiphertext,
                                  aliceP, aliceG, eveX, aliceY);

    cout << "\nШАГ 5: Алиса расшифровывает полученное сообщение\n";
    mitmAliceDecryptMessage(forwardedCiphertext, aliceP, aliceX);

    cout << "\n                        ИТОГ АТАКИ\n";
    cout << "  Боб думал, что шифрует ключом Алисы — шифровал ключом Евы   \n";
    cout << "  Ева прочитала и изменила сообщение Боба                     \n";
    cout << "  Алиса получила изменённое сообщение и ничего не заметила    \n";
    cout << "  Защита: использование сертификатов / цифровых подписей      \n";

    mpz_clear(aliceP); mpz_clear(aliceG);
    mpz_clear(aliceY); mpz_clear(aliceX);
    mpz_clear(eveY);   mpz_clear(eveX);
}

// Задание 6: Цепные дроби и решение уравнения 1256a + 847b = 119
void runContinuedFraction()
{
    cout << "\n--- Задание 6: Цепные дроби и диофантово уравнение ---\n\n";

    int A = 1256;
    int B = 847;
    int D = 119;

    // Цепная дробь
    Fraction(A, B);

    int x, y, gcd;

    // Расширенный алгоритм Евклида
    gcdExtended(A, B, x, y, gcd);

    cout << "\nНОД = " << gcd << "\n";

    // Проверка существования решения
    if (D % gcd != 0)
    {
        cout << "Решений нет.\n";
        return;
    }

    // Масштабируем решение
    int k = D / gcd;

    int a0 = x * k;
    int b0 = y * k;

    cout << "\nЧастное решение уравнения:\n";
    cout << "a = " << a0 << "\n";
    cout << "b = " << b0 << "\n";

    cout << "\nПроверка:\n";
    cout << A << " * (" << a0 << ") + " << B << " * (" << b0 << ") = "
         << A * a0 + B * b0 << "\n";

    cout << "\nОбщее решение:\n";
    cout << "a = " << a0 << " + " << B / gcd << " * t\n";
    cout << "b = " << b0 << " - " << A / gcd << " * t\n";
}
