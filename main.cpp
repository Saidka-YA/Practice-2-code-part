#include "extendedGCD.h"
using namespace std;

int main()
{
    cout << "Введите число m: ";
    int m;
    while (!(cin >> m))
    {
        cerr << "Ошибка: введите число!\n";
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Введите число m: ";
    }
    cout << "Введите число c: ";
    int c;
    while (!(cin >> c))
    {
        cerr << "Ошибка: введите число!\n";
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Введите число c: ";
    }
    int x, y, d;
    gcdExtended(m, c, x, y, d);
    cout << "Введённое число c: " << c << endl;
    cout << "Введённое число m: " << m << endl;
    cout << "Найденное число d: " << x << endl;
    cout << "Проверка условия c * d mod m = 1: " << (c * x) % m << endl;
    return 0;
}
