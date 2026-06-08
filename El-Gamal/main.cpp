#include <iostream>
#include "ElGamalblock.h"
#include "ElGamalUtilities.h"
using namespace std;

int main()
{
    cout << "1 - Зашифровать файл\n";
    cout << "2 - Расшифровать файл\n";
    cout << "3 - Зашифровать текст (консоль)\n";
    cout << "Выбор: ";

    int choice;
    cin >> choice;
    cin.ignore();

    string keyPath = "elgamalKey.bin";
    
    // Инициализация mpz переменных для ключей
    mpz_t p, g, y, x;
    mpz_init(p);
    mpz_init(g);
    mpz_init(y);
    mpz_init(x);

    if (choice == 1)
    {
        // Получение адресов файлов для считывания и сохранения данных
        string inputPath, outputPath;
        cout << "Введите путь к файлу: ";
        getline(cin, inputPath);
        cout << "Введите путь для сохранения результата: ";
        getline(cin, outputPath);
        // Читаем и записываем данные в вектор data
        vector<uint8_t> data;
        if (!readFile(inputPath, data))
        {
            cerr << "Ошибка чтения файла\n";
            mpz_clear(p);
            mpz_clear(g);
            mpz_clear(y);
            mpz_clear(x);
            return 1;
        }
        cout << "Файл прочитан. Размер: " << data.size() << " байт\n";

        // Генерируем ключи и сохраняем их в файл
        cout << "Генерация ключей...\n";
        generateKeys(p, g, y, x);

        // Вывод сгенерированных ключей
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
            mpz_clear(p);
            mpz_clear(g);
            mpz_clear(y);
            mpz_clear(x);
            return 1;
        }
        cout << "Ключи сохранены в " << keyPath << "\n";

        // Добавляем байты в конец данных чтобы были кратны 64 байтам
        Padding(data);
        cout << "Padding применён. Размер после padding: " << data.size() << " байт\n";

        // Шифруем данные и записываем в ciphertext
        cout << "Шифрование...\n";
        vector<uint8_t> ciphertext;
        encryptData(data, ciphertext, p, g, y);
        cout << "Шифрование завершено. Размер шифртекста: " << ciphertext.size() << " байт\n";

        if (!writeFile(outputPath, ciphertext))
        {
            cerr << "Ошибка записи файла\n";
            mpz_clear(p);
            mpz_clear(g);
            mpz_clear(y);
            mpz_clear(x);
            return 1;
        }

        cout << "Файл зашифрован. Ключ сохранён в " << keyPath << endl;
    }

    else if (choice == 2)
    {
        // Получаем адреса файлов откуда читать и куда записывать
        string inputPath, outputPath;
        cout << "Введите путь к файлу: ";
        getline(cin, inputPath);
        cout << "Введите путь для сохранения результата: ";
        getline(cin, outputPath);

        if (!loadKey(keyPath, p, g, y, x))
        {
            cerr << "Ошибка загрузки ключа\n";
            mpz_clear(p);
            mpz_clear(g);
            mpz_clear(y);
            mpz_clear(x);
            return 1;
        }

        // Вывод загруженных ключей
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
            mpz_clear(p);
            mpz_clear(g);
            mpz_clear(y);
            mpz_clear(x);
            return 1;
        }
        cout << "Файл прочитан. Размер шифртекста: " << ciphertext.size() << " байт\n";

        cout << "Расшифрование...\n";
        vector<uint8_t> plaintext;
        decryptData(ciphertext, plaintext, p, x);
        deletePadding(plaintext);
        cout << "Расшифрование завершено. Размер открытого текста: " << plaintext.size() << " байт\n";

        if (!writeFile(outputPath, plaintext))
        {
            cerr << "Ошибка записи файла\n";
            mpz_clear(p);
            mpz_clear(g);
            mpz_clear(y);
            mpz_clear(x);
            return 1;
        }
        cout << "Файл расшифрован.\n";
    }

    else if (choice == 3)
    {
        cout << "Ввод текста с переносами строк будет обработан неверно!\n";
        cout << "Для расшифрования/дешифрования большого текста "
                    << "используйте режим расшифрования/дефишрования файлов\n";
        cout << "Введите текст для шифрования: ";
        string text;
        getline(cin, text);

        // Текст -> байты
        vector<uint8_t> data(text.begin(), text.end());
        cout << "Размер входных данных: " << data.size() << " байт\n";

        // Генерируем ключи, сохраняем
        cout << "Генерация ключей...\n";
        generateKeys(p, g, y, x);

        // Вывод сгенерированных ключей
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

        // Шифруем
        cout << "Шифрование...\n";
        vector<uint8_t> ciphertext;
        encryptData(data, ciphertext, p, g, y);
        cout << "Шифрование завершено. Размер шифртекста: " << ciphertext.size() << " байт\n";

        cout << "\nЗашифрованный текст (hex):\n";
        cout << hexDisplay(ciphertext) << endl;

        // Расшифруем сразу для проверки
        cout << "\nРасшифрование...\n";
        vector<uint8_t> plaintext;
        decryptData(ciphertext, plaintext, p, x);
        deletePadding(plaintext);
        cout << "Расшифрование завершено. Размер открытого текста: " << plaintext.size() << " байт\n";

        string decrypted(plaintext.begin(), plaintext.end());
        cout << "\nРасшифрованный текст:\n";
        cout << decrypted << endl;
    }

    else
    {
        cerr << "Неверный выбор\n";
        mpz_clear(p);
        mpz_clear(g);
        mpz_clear(y);
        mpz_clear(x);
        return 1;
    }

    // Очистка mpz переменных
    mpz_clear(p);
    mpz_clear(g);
    mpz_clear(y);
    mpz_clear(x);

    return 0;
}