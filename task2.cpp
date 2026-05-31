#include <iostream>

using namespace std;

int gcdExtended(int a, int b, int& x, int& y)
{
    // Блок У
    int x1 = a;
    int x2 = 1;
    int x3 = 0;
    // Блок Х
    int y1 = b;
    int y2 = 0;
    int y3 = 1;
    while (y1 != 0)
    {
        int Q = x1 / y1;

        int t1 = Q;
        int t2 = x2 - Q * y2;
        int t3 = x3 - Q * y3;
        
        x1 = y1;
        x2 = y3;
        x3 = y3;
        y1 = t1;
        y2 = t2;
        y3 = t3;
    }
    x = x2;
    y = x3;
    return x1;
}

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
}