#include "extendedGCD.h"
using namespace std;

int main()
{
    cout << "Введите число c: ";
    int c;
    while (!(cin >> c))
    {
        cerr << "Ошибка: введите число!\n";
        cin.clear();
        cin.ignore(1000, '\n');
    }
    cout << "Введите число m: ";
    int m;
    while (!(cin >> m))
    {
        cerr << "Ошибка: введите число!\n";
        cin.clear();
        cin.ignore(1000, '\n');
    }
    int x, y, d;
    gcdExtended(c, m, x, y, d);
    cout << "Введёное число с: " << c << endl;
    cout << "Введённое число m: " << m << endl;
    cout << "Найденный обратный элемент: " << x << endl;
    cout << "Проверка условия c^-1 mod m = d: " << y % m << endl;;
}