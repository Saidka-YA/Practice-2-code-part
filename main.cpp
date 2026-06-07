#include "extendedGCD.h"
using namespace std;

int main()
{
    // ввод первого числа
    cout << "Введите число m: ";
    int m;
    while (!(cin >> m))
    {
        cerr << "Ошибка: введите число!\n";
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Введите число m: ";
    }

    // ввод второго числа
    cout << "Введите число c: ";
    int c;
    while (!(cin >> c))
    {
        cerr << "Ошибка: введите число!\n";
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Введите число c: ";
    }

    // переменные для результатов алгоритма
    int x, y, d;

    // вызов расширенного алгоритма евклида
    gcdExtended(m, c, x, y, d);

    // вывод исходных данных
    cout << "Введённое число c: " << c << endl;
    cout << "Введённое число m: " << m << endl;
    if (d != 1)
    {
        cout << "Обратный элемент не существует, так как НОД("
                << c << ", " << m << ") != 1" << endl;
    }
    else
    {
        int inverse = (y % m + m) % m;
        cout << "Найденное число d: " << inverse << endl;
        cout << "Проверка условия c * d mod m = 1: " << (c * inverse) % m << endl;
    }
    return 0;
}
