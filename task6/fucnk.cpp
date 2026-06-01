#include "funck.h"

using namespace std;

// Построение цепной дроби
void Fraction(int a, int b)
{
    cout << "\nЦепная дробь для " << a << "/" << b << ":\n";

    cout << "[";

    bool first = true;

    while (b != 0)
    {
        // Очередной элемент цепной дроби
        int q = a / b;

        if (!first)
        {
            cout << ";";
        }
        else
        {
            first = false;
        }
        cout << q;
        // Следующий шаг алгоритма Евклида
        int tmp = a % b;
        a = b;
        b = tmp;    
    }

    cout << "]\n";
}