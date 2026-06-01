#include <iostream>
#include <string>
#include <cstdlib>

using namespace std;

bool isPrime(uint n) {
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


int main() 
{   
    // Ввод параметров
    cout << "Введите основание a: ";
    uint a;
    while(!(cin >> a))
    {
        cerr << "Ошибка: введите число!\n";
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Введите основание a: ";
    }

    cout << "Введите степень x: ";  
    uint x;
    while(!(cin >> x))
    {
        cerr << "Ошибка: введите число!\n";
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Введите степень x: "; 
    }
    
    cout << "Введите модуль p: "; 
    uint p;
    while(!(cin >> p) || p == 0)
    {
        cerr << "Ошибка: введите число не равное нулю!\n";
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Введите модуль p: "; 
    }
    
    cout << "\nВычисляем: " << a << "^" << x << " mod " << p << endl;
    cout << endl;
    
    // Вычисление первым методом (Ферма)
    bool fermaFlag = false;
    uint fermaResult = fermaMethod(a, x, p, fermaFlag);
    
    if (fermaFlag) 
    {
        cout << "Метод 1 (теорема Ферма): " << a << "^" << x << " mod " << p 
            << " = " << fermaResult << endl;
        cout << "(степень уменьшена: " << x << " mod " << (p-1) 
            << " = " << (x % (p-1)) << ")\n";
    }
    else 
    {
        cout << "Метод 1 (теорема Ферма): не работает\n";
        if (!isPrime(p)) 
        {
            cout << "\tПричина: " << p << " не является простым числом\n";
        } 
        else if (a % p == 0) 
        {
            cout << "\tПричина: " << a << " делится на " << p << endl;
        } 
        else 
        {
            cout << "\tПричина: НОД(" << a << ", " << p << ") != 1\n";
        }
    }
    
    // Вычисление вторым методом (бинарный)
    uint binaryResult = binaryModPow(a, x, p);
    cout << "Метод 2 (бинарный алгоритм): " << a << "^" << x << " mod " << p 
         << " = " << binaryResult << endl;
    
    // Сравнение результатов
    cout << endl;
    if (fermaFlag) 
    {
        if (fermaResult == binaryResult) 
        {
            cout << "Результаты совпадают: " << fermaResult << endl;
        } 
        else 
        {
            cout << "Ошибка: результаты различаются!\n";
            cout << "  Ферма: " << fermaResult << endl;
            cout << "  Бинарный: " << binaryResult << endl;
        }
    } 
    else 
    {
        cout << "Результат вычисления: " << binaryResult << endl;
    }
    
    // Дополнительная проверка теоремы Ферма
    if (isPrime(p) && a % p != 0) 
    {
        uint fermaCheck = binaryModPow(a, p - 1, p);
        cout << endl;
        cout << "Проверка теоремы Ферма: " << a << "^" << (p-1) << " mod " << p 
             << " = " << fermaCheck;
        if (fermaCheck == 1) 
        {
            cout << "\t(теорема работает)\n";
        } 
        else 
        {
            cout << "\t(ошибка проверки)\n";
        }
    }
    cout << endl;
    
    return 0;
}