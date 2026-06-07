#include <fstream>
#include <random>
#include <sstream>
#include <iomanip>
#include <gmp.h>

using namespace std;

// Функция модульного возведения в степень
// result = base^exp mod mod
void modPow(mpz_t result, const mpz_t base, const mpz_t exp, const mpz_t mod)
{
    mpz_powm(result, base, exp, mod);
}

// Функция поиска примитивного корня (генератора)
void findGenerator(mpz_t g, const mpz_t p)
{
    gmp_randstate_t state;
    gmp_randinit_default(state);
    
    random_device rd;
    gmp_randseed_ui(state, rd());
    
    mpz_t temp, p_minus_1, half;
    mpz_init(temp);
    mpz_init(p_minus_1);
    mpz_init(half);
    
    mpz_sub_ui(p_minus_1, p, 1);
    mpz_div_ui(half, p_minus_1, 2);
    
    // Ищем подходящий генератор
    for(int attempt = 0; attempt < 100; attempt++)
    {
        mpz_urandomm(g, state, p_minus_1);
        if(mpz_cmp_ui(g, 2) < 0) continue;
        
        // Проверяем что g^2 != 1 и g^((p-1)/2) != 1
        mpz_powm_ui(temp, g, 2, p);
        if(mpz_cmp_ui(temp, 1) == 0) continue;
        
        mpz_powm(temp, g, half, p);
        if(mpz_cmp_ui(temp, 1) == 0) continue;
        
        mpz_clear(temp);
        mpz_clear(p_minus_1);
        mpz_clear(half);
        gmp_randclear(state);
        return;
    }
    
    // Запасной вариант
    mpz_set_ui(g, 2);
    
    mpz_clear(temp);
    mpz_clear(p_minus_1);
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
    uint8_t padding = 64 - (data.size() % 64);
    if (padding == 0) {padding = 64;} 
    for (int i = 0; i < padding; i++)
    {
        data.push_back(padding); // Добавление байтов в конец сообщения
    }    
}

// Функция удаления добавленных байтов в конец сообщения
void deletePadding(vector<uint8_t>& data)
{
    if (data.empty()) return; // Если вводные данные пусты выходим из функции, удалять нечего
    // Получаем последний байт наших данных, это как раз добавочное значение
    uint8_t padding = data.back(); 
    if (padding == 0 || padding > 64) return; // Проверка границ
    data.resize(data.size() - padding); // Удаляем добавочные байты
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
    // Перемешение обратно в начало
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