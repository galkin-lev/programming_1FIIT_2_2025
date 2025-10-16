#include <iostream>
#include <cmath>
#include <format>
#include <iomanip>

using namespace std;
// TIP To <b>Run</b> code, press <shortcut actionId="Run"/> or click the <icon src="AllIcons.Actions.Execute"/> icon in the gutter.
#include <iostream>

int setBit(int x, int i) {
    return x | (1 << i);
}

int main() {

    unsigned int i, x;

    while ( true  ) {
        cout << "Enter x (0<x<10^9) : ";
        cin >> x;

        if ( cin.fail() || x > 1e9 ) { // pow(10,9) || 10 000 000 00
            cin.clear(); // Clear error flag
            cin.ignore( numeric_limits< streamsize>::max(), '\n'); // Discard bad input
            cout << "Invalid input. Please enter a valid number.\n";
        } else {
            break; // Exit loop on valid input
        }
    }

    cout << "\n initial value: " << format("{:b}", x) << endl;

    while ( true  ) { // x to i input limits
        cout << "\nEnter the bit you want to replace with \"1\" \n(value from 0 to " << (bit_width(x)-1) << ") :" ;
        cin >> i;

        if ( cin.fail() || i > (bit_width(x)-1)) {
            cin.clear(); // Clear error flag
            cin.ignore( numeric_limits< streamsize>::max(), '\n'); // Discard bad input
            cout << "Invalid input. Please enter a valid number.\n";
        } else {
            break; // Exit loop on valid input
        }
    }


    // std::cout << setBit(x, i) << std::endl;

    cout << "\n changed value: " << format("{:b}", setBit(x, i)) << endl;

    return 0;
}