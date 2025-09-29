#include <iostream>
#include <cmath>
#include <iomanip>

using namespace std;

int main() {


    cout << "\nLab2_var1 \n 1. Процентная ставка по вкладу составляет P процентов годовых, которые прибавл"
            "яются к сумме вклада в конце года. Вклад составляет X рублей Y копеек. Напишите программу, которая "
            "по введённым X, Y, P выведет размер вклада через год. \n\n";

    int init_rubles, init_kopeek;
    double interest;
    

    while (true) {
        cout << "Enter the rubles: ";
        cin >> init_rubles;

        if ( cin.fail()) {
             cin.clear(); // Clear error flag
             cin.ignore( numeric_limits< streamsize>::max(), '\n'); // Discard bad input
             cout << "Invalid input. Please enter a valid number.\n";
        } else {
            break; // Exit loop on valid input
        }
    }

    while (true) {
        cout << "Enter the kopeiki: ";
        cin >> init_kopeek;
        if ( cin.fail()) {
             cin.clear(); // Clear error flag
             cin.ignore( numeric_limits< streamsize>::max(), '\n'); // Discard bad input
             cout << "Invalid input. Please enter a valid number.\n";
        } else {
            break; // Exit loop on valid input
        }
    }

    while (true) {
        cout << "Enter the interest: ";
        cin >> interest;

        if ( cin.fail()) {
             cin.clear(); // Clear error flag
             cin.ignore( numeric_limits< streamsize>::max(), '\n'); // Discard bad input
             cout << "Invalid input. Please enter a valid number.\n";
        } else {
            break; // Exit loop on valid input
        }
    }
    
    // Переводим всю сумму в копейки
    long long total_kopeek = init_rubles * 100LL + init_kopeek;

    // Вычисляем сумму с процентами и округляем до ближайшего целого
    double total_with_interest = total_kopeek * (100 + interest) / 100.0;
    long long new_total = round(total_with_interest);

    // Переводим обратно в рубли и копейки
    long long rubles = new_total / 100;
    long long kopeek = new_total % 100;

    cout  << " You will have: " << rubles << "." << kopeek << "rub in just a year" <<  endl;




    cout << "\n"
            " 2. По двум вводимым катетам A и B вычислить и вывести гипотенузу. 1 ≤ A, B ≤ 100: \n";

    int A, B;
    while (true) {
        cout << "Enter A: ";
        cin >> A;

        if ( cin.fail() or A<1 or A>100) {
            cin.clear(); // Clear error flag
            cin.ignore( numeric_limits< streamsize>::max(), '\n'); // Discard bad input
            cout << "Invalid input. Please enter a valid number.\n";
        } else {
            break; // Exit loop on valid input
        }
    }
    while (true) {
        cout << "Enter B: ";
        cin >> B;

        if ( cin.fail() or B>100 or B<1) {
            cin.clear(); // Clear error flag
            cin.ignore( numeric_limits< streamsize>::max(), '\n'); // Discard bad input
            cout << "Invalid input. Please enter a valid number.\n";
        } else {
            break; // Exit loop on valid input
        }
    }

    // Вычисляем гипотенузу по теореме Пифагора
    double hypotenuse = sqrt(A * A + B * B);

    // Выводим результат с точностью до 6 знаков после запятой
    cout << fixed << setprecision(6) << hypotenuse << endl;

    
    return 0;
}