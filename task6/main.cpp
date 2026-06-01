#include "extendedGCD.h"
#include "funck.h"
using namespace std;

int main()
{
    int A = 1256;
    int B = 847;
    int D = 119;

    // Цепная дробь
    Fraction(A, B);

    int x, y, gcd;

    // Расширенный алгоритм Евклида
    gcdExtended(A, B, x, y, gcd);

    cout << "\nНОД = " << gcd << endl;
    // Проверка существования решения
    if (D % gcd != 0)
    {
        cout << "Решений нет." << endl;
        return 0;
    }
    // Масштабируем решение
    int k = D / gcd;

    int a0 = x * k;
    int b0 = y * k;

    cout << "\nЧастное решение уравнения:\n";
    cout << "a = " << a0 << endl;
    cout << "b = " << b0 << endl;

    cout << "\nПроверка:\n";
    cout << A << " * (" << a0 << ") + " << B << " * (" << b0 << ") = "
         << A * a0 + B * b0 << endl;

    cout << "\nОбщее решение:\n";
    cout << "a = " << a0 << " + " << B / gcd << " * t\n";
    cout << "b = " << b0 << " - " << A / gcd << " * t\n";

    return 0;
}