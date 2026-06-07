#include <iostream>
#include "funcktions.h"

using namespace std;

int main() 
{   
    // ввод исходных данных
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
    // модуль не должен быть равен нулю
    while(!(cin >> p) || p == 0)
    {
        cerr << "Ошибка: введите число не равное нулю!\n";
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Введите модуль p: "; 
    }
    
    // вывод выражения для вычисления
    cout << "\nВычисляем: " << a << "^" << x << " mod " << p << endl;
    cout << endl;
    
    // вычисление по теореме ферма
    bool fermaFlag = false;
    uint fermaResult = fermaMethod(a, x, p, fermaFlag);
    
    if (fermaFlag) 
    {
        cout << "Метод 1 (теорема Ферма): " << a << "^" << x << " mod " << p 
            << " = " << fermaResult << endl;

        // вывод сокращенной степени
        cout << "(степень уменьшена: " << x << " mod " << (p-1) 
            << " = " << (x % (p-1)) << ")\n";
    }
    else 
    {
        // обработка случая когда метод неприменим
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
    
    // вычисление бинарным методом
    uint binaryResult = binaryModPow(a, x, p);

    cout << "Метод 2 (бинарный алгоритм): " << a << "^" << x << " mod " << p 
         << " = " << binaryResult << endl;
    
    // сравнение полученных результатов
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
    
    // дополнительная проверка теоремы ферма
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