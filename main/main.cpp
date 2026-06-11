#include "mainUtilities.h"
using namespace std;

int main()
{
    int rawChoice;

    // Бесконечный цикл главного меню
    while (true)
    {
        printMenu();

        if (!(cin >> rawChoice))
        {
            cerr << "Ошибка: введите число!\n";
            cin.clear();
            cin.ignore(1000, '\n');
            continue;
        }

        MenuOption menuChoice = static_cast<MenuOption>(rawChoice);

        switch (menuChoice)
        {
            case MenuOption::ModularExponentiation:
                runModularExponentiation();
                break;

            case MenuOption::InverseViaCandD:
                runInverseViaCandD();
                break;

            case MenuOption::InverseCmod:
                runInverseCmod();
                break;

            case MenuOption::ElGamalCipher:
                runElGamalCipher();
                break;

            case MenuOption::MitmAttack:
                runMitmAttack();
                break;

            case MenuOption::Exit:
                cout << "\nВыход из программы.\n";
                return 0;

            default:
                cerr << "Неверный выбор. Введите число от 0 до 5.\n";
                break;
        }
    }
    return 0;
}
