#include "extendedGCD.h"
using namespace std;

int main()
{
    // ввод первого числа
    cout << "Введите число c: ";
    int c;
    while (!(cin >> c))
    {
        cerr << "Ошибка: введите число!\n";
        cin.clear();
        cin.ignore(1000, '\n');
    }

    // ввод второго числа
    cout << "Введите число m: ";
    int m;
    while (!(cin >> m))
    {
        cerr << "Ошибка: введите число!\n";
        cin.clear();
        cin.ignore(1000, '\n');
    }

    // переменные для результатов алгоритма
    int x, y, d;

    // вызов расширенного алгоритма евклида
    gcdExtended(c, m, x, y, d);

    // вывод исходных данных
    cout << "Введёное число с: " << c << endl;
    cout << "Введённое число m: " << m << endl;
    if (d != 1)
    {
        cout << "Обратный элемент не существует, так как НОД("
             << c << ", " << m << ") != 1" << endl;
    }
    else
    {
        int inverse = (x % m + m) % m;

        cout << "Найденный обратный элемент: " << inverse << endl;
        cout << "Проверка условия c^-1 mod m = d: " << (c * inverse) % m << endl;
    }
    return 0;
}