#include "ElGamalUtilities.h"
using namespace std;

// Функция модульного возведения в степень
// reslt = baseG^expnt mod modP
void modPow(mpz_t reslt, const mpz_t baseG, const mpz_t expnt, const mpz_t modP)
{
    mpz_powm(reslt, baseG, expnt, modP);
}

// Функция поиска примитивного корня (генератора)
void findGenerator(mpz_t baseG, const mpz_t modP)
{
    gmp_randstate_t state;
    gmp_randinit_default(state);

    random_device rd;
    gmp_randseed_ui(state, rd());

    mpz_t temp, modPm1, half;
    mpz_init(temp);
    mpz_init(modPm1);
    mpz_init(half);

    mpz_sub_ui(modPm1, modP, 1);
    mpz_div_ui(half, modPm1, 2);

    // Ищем подходящий генератор
    for(int attempt = 0; attempt < 100; attempt++)
    {
        mpz_urandomm(baseG, state, modPm1);
        if(mpz_cmp_ui(baseG, 2) < 0) continue;

        // Проверяем что baseG^2 != 1 и baseG^((modP-1)/2) != 1
        mpz_powm_ui(temp, baseG, 2, modP);
        if(mpz_cmp_ui(temp, 1) == 0) continue;

        mpz_powm(temp, baseG, half, modP);
        if(mpz_cmp_ui(temp, 1) == 0) continue;

        mpz_clear(temp);
        mpz_clear(modPm1);
        mpz_clear(half);
        gmp_randclear(state);
        return;
    }

    // Запасной вариант
    mpz_set_ui(baseG, 2);

    mpz_clear(temp);
    mpz_clear(modPm1);
    mpz_clear(half);
    gmp_randclear(state);
}

// Функция генерации простого числа
void generatePrime(mpz_t prime, int bits)
{
    gmp_randstate_t state;
    gmp_randinit_default(state);

    // Инициализация генератора случайных чисел
    random_device gen;
    gmp_randseed_ui(state, gen());

    // Генерация простого числа заданной битности
    mpz_urandomb(prime, state, bits);
    mpz_nextprime(prime, prime);

    gmp_randclear(state);
}

// Функция добавления байтов в конец сообщения
void Padding(vector<uint8_t>& data)
{
    // Padding до кратности 64 байтам (512 бит)
    uint8_t padSz = 64 - (data.size() % 64);
    if (padSz == 0) {padSz = 64;}
    for (int i = 0; i < padSz; i++)
    {
        data.push_back(padSz); // Добавление байтов в конец сообщения
    }
}

// Функция удаления добавленных байтов в конец сообщения
void deletePadding(vector<uint8_t>& data)
{
    if (data.empty()) return; // Если вводные данные пусты выходим из функции, удалять нечего
    // Получаем последний байт наших данных, это как раз добавочное значение
    uint8_t padSz = data.back();
    if (padSz == 0 || padSz > 64) return; // Проверка границ
    data.resize(data.size() - padSz); // Удаляем добавочные байты
}

// Читаем файл
bool readFile(const string& path, vector<uint8_t>& data)
{
    // Открытие файла в бинарном режиме
    ifstream file(path, ios::binary);
    // Проверка открытия файла
    if (!file) return false;
    // Перемещение в конец файла для получения размера файла
    // Буквальный перевод строки: переместись на 0 байт относительно конца файла
    file.seekg(0, ios::end);
    // Получение и запись размера файла
    // Принцип: размер == последней позиции в файле
    //tellg == tell get position
    size_t size = (size_t)file.tellg();
    // Перемещение обратно в начало
    file.seekg(0, ios::beg);
    // Увеличиваем размер вектора data
    data.resize(size);
    // Запись данных в файл
    file.read(reinterpret_cast<char*>(&data[0]), size);
    // Закрытие файла
    file.close();
    return true;
}

// Запись в файл
bool writeFile(const string& path, const vector<uint8_t>& data)
{
    // Открытие файла в бинарном режиме
    ofstream file(path, ios::binary);
    // Проверка открытия
    if (!file) return false;
    // Запись данных в файл
    file.write(reinterpret_cast<const char*>(&data[0]), data.size());
    // Закрытие файла
    file.close();
    return true;
}

// Перевод байт в hex-строку для вывода в консоль
string hexDisplay(const vector<uint8_t>& data)
{
    ostringstream oss; // Строковый поток для вывода в строку
    for (size_t i = 0; i < data.size(); i++)
    {
        // Перевод вывода в режим hex, и перевод из uint8_t в int
        oss << hex << setw(2) << setfill('0') << (int)data[i];
        if (i + 1 < data.size()) oss << " ";
    }
    return oss.str(); // Возвращаем полученную строку из потока
}

// Функция генерации ключей
// Параметры: modP, baseG, openY - открытый ключ, secX - закрытый ключ
void generateKeys(mpz_t modP, mpz_t baseG, mpz_t openY, mpz_t secX)
{
    // Генерируем простое число modP (1024 бита для криптостойкости)
    generatePrime(modP, 1024);

    // Находим генератор baseG
    findGenerator(baseG, modP);

    // Генерируем секретный ключ secX
    gmp_randstate_t state;
    gmp_randinit_default(state);
    random_device gen;
    gmp_randseed_ui(state, gen());

    mpz_t modPm2;
    mpz_init(modPm2);
    mpz_sub_ui(modPm2, modP, 2);

    mpz_urandomm(secX, state, modPm2);
    mpz_add_ui(secX, secX, 2);

    mpz_clear(modPm2);

    // Вычисляем открытый ключ openY = baseG^secX mod modP
    modPow(openY, baseG, secX, modP);

    gmp_randclear(state);
}

// Запись ключей в файл
bool saveKey(const string& path, const mpz_t modP, const mpz_t baseG, const mpz_t openY, const mpz_t secX)
{
    // Открыть файл как бинарник
    ofstream file(path, ios::binary);
    // Проверка открытия
    if (!file) return false;

    // Сохраняем каждое число
    // Формат: размер (4 байта) + данные

    // Сохраняем modP
    size_t modPSz = (mpz_sizeinbase(modP, 2) + 7) / 8;
    file.write(reinterpret_cast<const char*>(&modPSz), sizeof(size_t));
    vector<uint8_t> modPBy(modPSz);
    mpz_export(&modPBy[0], NULL, 1, sizeof(uint8_t), 0, 0, modP);
    file.write(reinterpret_cast<const char*>(&modPBy[0]), modPSz);

    // Сохраняем baseG
    size_t baseGSz = (mpz_sizeinbase(baseG, 2) + 7) / 8;
    file.write(reinterpret_cast<const char*>(&baseGSz), sizeof(size_t));
    vector<uint8_t> baseGBy(baseGSz);
    mpz_export(&baseGBy[0], NULL, 1, sizeof(uint8_t), 0, 0, baseG);
    file.write(reinterpret_cast<const char*>(&baseGBy[0]), baseGSz);

    // Сохраняем openY
    size_t openYSz = (mpz_sizeinbase(openY, 2) + 7) / 8;
    file.write(reinterpret_cast<const char*>(&openYSz), sizeof(size_t));
    vector<uint8_t> openYBy(openYSz);
    mpz_export(&openYBy[0], NULL, 1, sizeof(uint8_t), 0, 0, openY);
    file.write(reinterpret_cast<const char*>(&openYBy[0]), openYSz);

    // Сохраняем secX
    size_t secXSz = (mpz_sizeinbase(secX, 2) + 7) / 8;
    file.write(reinterpret_cast<const char*>(&secXSz), sizeof(size_t));
    vector<uint8_t> secXBy(secXSz);
    mpz_export(&secXBy[0], NULL, 1, sizeof(uint8_t), 0, 0, secX);
    file.write(reinterpret_cast<const char*>(&secXBy[0]), secXSz);

    // Закрытие файла
    file.close();
    return true;
}

// Загружаем ключи из файла
bool loadKey(const string& path, mpz_t modP, mpz_t baseG, mpz_t openY, mpz_t secX)
{
    // Открываем файл как бинарник
    ifstream file(path, ios::binary);
    // Проверка открытия
    if (!file) return false;

    // Загружаем modP
    size_t modPSz;
    file.read(reinterpret_cast<char*>(&modPSz), sizeof(size_t));
    vector<uint8_t> modPBy(modPSz);
    file.read(reinterpret_cast<char*>(&modPBy[0]), modPSz);
    mpz_import(modP, modPSz, 1, sizeof(uint8_t), 0, 0, &modPBy[0]);

    // Загружаем baseG
    size_t baseGSz;
    file.read(reinterpret_cast<char*>(&baseGSz), sizeof(size_t));
    vector<uint8_t> baseGBy(baseGSz);
    file.read(reinterpret_cast<char*>(&baseGBy[0]), baseGSz);
    mpz_import(baseG, baseGSz, 1, sizeof(uint8_t), 0, 0, &baseGBy[0]);

    // Загружаем openY
    size_t openYSz;
    file.read(reinterpret_cast<char*>(&openYSz), sizeof(size_t));
    vector<uint8_t> openYBy(openYSz);
    file.read(reinterpret_cast<char*>(&openYBy[0]), openYSz);
    mpz_import(openY, openYSz, 1, sizeof(uint8_t), 0, 0, &openYBy[0]);

    // Загружаем secX
    size_t secXSz;
    file.read(reinterpret_cast<char*>(&secXSz), sizeof(size_t));
    vector<uint8_t> secXBy(secXSz);
    file.read(reinterpret_cast<char*>(&secXBy[0]), secXSz);
    mpz_import(secX, secXSz, 1, sizeof(uint8_t), 0, 0, &secXBy[0]);

    // Закрываем файл
    file.close();
    return true;
}
