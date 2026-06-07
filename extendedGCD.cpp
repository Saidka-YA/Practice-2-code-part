#include "extendedGCD.h"
using namespace std;

void gcdExtended(int a, int b, int& x, int& y, int& d)
{
    // Блок У
    int x1 = a;
    int x2 = 1;
    int x3 = 0;
    // Блок Х
    int y1 = b;
    int y2 = 0;
    int y3 = 1;

    cout << "|" << setw(7) << "Q" << "|" << setw(7) << "x" << "|" << setw(7) 
         << "y" << "|" << setw(7) << "d" << "|" << setw(7) << "b" << "|" << endl;

    cout << string(41, '-') << endl;

    while (y1 != 0)
    {
        int Q = x1 / y1;

        int t1 = x1 % y1;
        int t2 = x2 - Q * y2;
        int t3 = x3 - Q * y3;
        
        cout << "|" << setw(7) << Q << "|" << setw(7) << t2 << "|" << setw(7) << t3  
             << "|" << setw(7) << x1 << "|" << setw(7) << y1 << "|" << endl;
        // Обновление переменных
        x1 = y1;
        x2 = y2;
        x3 = y3;
        y1 = t1;
        y2 = t2;
        y3 = t3;
    }
    // Сохранение коэффициэнтов Безу
    x = x2;
    y = x3;
    // Вывод НОД
    cout << "НОД(" << a << "," << b << "): " << x1 << endl;
}