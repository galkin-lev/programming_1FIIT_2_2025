#include <any>
#include <iostream>
#include <iostream>
#include <cmath>
#include <format>
#include <iomanip>
#include <oneapi/tbb/detail/_task.h>

#include <algorithm>
#include <string>
using namespace std;

// TIP To <b>Run</b> code, press <shortcut actionId="Run"/> or click the <icon src="AllIcons.Actions.Execute"/> icon in the gutter.

// Функция для проверки, начинается ли число с цифры 1

const int matrixDefaultSize = 100;
// int m = 100;

void printArray(int arr[], int size) {
    cout << "[";
    for(int i = 0; i < size; i++) {
        cout << arr[i];
        if(i < size - 1) cout << ", ";
    }
    cout << "]" ;
}

void printMatrix(int matrix[][matrixDefaultSize], int n, int m) {

    // Вывод с индексами столбцов
    cout << "   ";  // отступ для строки с индексами
    for (int j = 0; j < m; j++) {
        cout << setw(3) << " [" << j << "]";
    }
    cout << endl;

    // Вывод данных с индексами строк
    for (int i = 0; i < n; i++) {
        cout << "[" << i << "]";
        for (int j = 0; j < m; j++) {
            cout << setw(4) << matrix[i][j] << " ";
        }
        cout << endl;
    }
}


// 1 exercise functions
bool startsWithOne(int num) { // with string
    string str = to_string(num);
    return !str.empty() && str[0] == '1';
}

// 2 exercise functions
int getFirstDigit(int number) { // by dividing by 10
    if (number == 0) return 0;
    number = abs(number);
    while (number >= 10) number /= 10;
    return number;
}

int getDigitDigitsSum(int number) {
    if (number == 0) return 0;
    number = abs(number);
    int sum = 0;
    while (number > 0) {
        sum += number % 10;
        number /= 10;
    }
    return sum;
}

bool compare(int a, int b) { // compare function for sort
    int firstA = getFirstDigit(a);
    int firstB = getFirstDigit(b);
    if (firstA != firstB) return firstA < firstB; // 1. first comparison: if first digit of b > a then a <-> b

    int sumA = getDigitDigitsSum(a);
    int sumB = getDigitDigitsSum(b);
    if (sumA != sumB) return sumA < sumB; // 2. compare by the sum of digits (same)

    return a < b;
}

int main() {


    int tasknum;

    while (true) {
        cout << "Enter the exercise (1-5): ";
        cin >> tasknum;
        if (tasknum == 1) { // 1 задание



            // var 1
            /*1.  Дана последовательность натуральных чисел {aj}j=1...n (n<=10000). Если в последовательности есть хотя
             *бы одно число, начинающееся цифрой 1, упорядочить последовательность по неубыванию.*/

            unsigned int n;
            while ( true  ) {
                cout << "Enter sequence size (n) (n<=10000) : ";
                cin >> n;

                if ( cin.fail() || n > 10000 ) {
                    cin.clear(); // Clear error flag
                    cin.ignore( numeric_limits< streamsize>::max(), '\n'); // Discard bad input
                    cout << "Invalid input. Please enter a valid number.\n";
                } else {
                    break; // Exit loop on valid input
                }
            }
             int sequence[n];
            // fill the sequence
            for (int i = 0; i < n; ++i) {


                while ( true  ) {
                    cout << "Enter number №" << i << " of the sequence (should be a natural number):";

                    cin >> sequence[i];

                    if ( cin.fail() || sequence[i] <= 0) { // Проверка, что число натуральное (положительное)
                        cin.clear(); // Clear error flag
                        cin.ignore( numeric_limits< streamsize>::max(), '\n'); // Discard bad input
                        cout << "Invalid input. Please enter a valid number.\n";
                    } else {
                        break; // Exit loop on valid input
                    }
                }
            }

            cout << "entered sequence:";
            printArray(sequence, n);
            cout << " of size: " << (sizeof(sequence)/sizeof(sequence[0])) << endl;


            bool hasNumberStartingWithOne = false;
            for (int num : sequence) {
                if (startsWithOne(num)) {
                    hasNumberStartingWithOne = true;
                    break;
                }
            }

            if (hasNumberStartingWithOne) {

                cout << "there is a number that starts with 1 >> here is the sorted sequence (not bubble): " << endl;
                // sort(sequence, sequence + (sizeof(sequence)/sizeof(sequence[0])));
                sort(&sequence[0], &sequence[n]);
                printArray(sequence, n); cout << endl;
            }

        }


/*2.  Ввести последовательность натуральных чисел {Aj}j=1...n (n<=1000). Упорядочить последовательность по неубыванию
 *первой цифры числа, числа с одинаковыми первыми цифрами дополнительно упорядочить по неубыванию суммы цифр числа, числа
 *с одинаковыми первыми цифрами и одинаковыми суммами цифр дополнительно упорядочить по неубыванию самого числа.
 */
        if ( tasknum == 2 ) {
            cout << "Enter sequence size (n) (n<=10000) : ";
            unsigned int n;
            while ( true  ) {
                cout << "Enter sequence size (n) (n<=10000) : ";
                cin >> n;

                if ( cin.fail() || n > 10000 ) {
                    cin.clear(); // Clear error flag
                    cin.ignore( numeric_limits< streamsize>::max(), '\n'); // Discard bad input
                    cout << "Invalid input. Please enter a valid number.\n";
                } else {
                    break; // Exit loop on valid input
                }
            }
            int sequence[n];
            // fill the sequence
            for (int i = 0; i < n; ++i) {


                while ( true  ) {
                    cout << "Enter number №" << i << " of the sequence (should be a natural number):";

                    cin >> sequence[i];

                    if ( cin.fail() || sequence[i] <= 0) { // Проверка, что число натуральное (положительное)
                        cin.clear(); // Clear error flag
                        cin.ignore( numeric_limits< streamsize>::max(), '\n'); // Discard bad input
                        cout << "Invalid input. Please enter a valid number.\n";
                    } else {
                        break; // Exit loop on valid input
                    }
                }
            } // copy from first ( enter sequence )

            cout << "entered sequence:";
            printArray(sequence, n);
            cout << " of size: " << (sizeof(sequence)/sizeof(sequence[0])) << endl;

            sort(&sequence[0], &sequence[n], compare);
            cout << "sorted sequence:";
            printArray(sequence, n); cout << endl;



        }

        if ( tasknum == 3 ) {
            
            cout << "Enter sequence size (n) (n<=10000) : ";
            unsigned int n;
            unsigned int m;
            while ( true  ) {
                cout << "Enter sequence size (n) (n<=10000) strings : ";
                cin >> n;

                if ( cin.fail() || n > 100 ) {
                    cin.clear(); // Clear error flag
                    cin.ignore( numeric_limits< streamsize>::max(), '\n'); // Discard bad input
                    cout << "Invalid input. Please enter a valid number.\n";
                } else {
                    break; // Exit loop on valid input
                }
            }
            while ( true  ) {
                cout << "Enter sequence size (m) (m<=10000) columns : ";
                cin >> m;

                if ( cin.fail() || m > 100 ) {
                    cin.clear(); // Clear error flag
                    cin.ignore( numeric_limits< streamsize>::max(), '\n'); // Discard bad input
                    cout << "Invalid input. Please enter a valid number.\n";
                } else {
                    break; // Exit loop on valid input
                }
            }

            
            int matrix[matrixDefaultSize][matrixDefaultSize];
            int sums[100];

            // fill the matrix
            for (int i = 0; i < n; ++i) {
                sums[i] = 0;
                for (int j = 0; j < m; j++) {

                while ( true  ) {
                    cout << "Enter number [" << i << "] [" << j << "] of the matrix (should be a natural number):";

                    cin >> matrix[i][j];

                     sums[i] += matrix[i][j];
                    if ( cin.fail() || matrix[i][j] <= 0) { // Проверка, что число натуральное (положительное)
                        cin.clear(); // Clear error flag
                        cin.ignore( numeric_limits< streamsize>::max(), '\n'); // Discard bad input
                        cout << "Invalid input. Please enter a valid number.\n";
                    } else {
                        break; // Exit loop on valid input
                    }
                }
                }
            }

            cout << "entered sequence:" << endl;
            printMatrix(matrix, n, m);

            // // Вывод с индексами столбцов
            //
            // cout << "   ";  // отступ для строки с индексами
            // for (int j = 0; j < m; j++) {
            //     cout << setw(4) << "[" << j << "]";
            // }
            // cout << endl;
            //
            // // Вывод данных с индексами строк
            // for (int i = 0; i < n; i++) {
            //     cout << "[" << i << "]";
            //     for (int j = 0; j < m; j++) {
            //         cout << setw(5) <<  matrix[i][j]; // setw(
            //     }
            //     cout << endl;
            // }
            // cout << " of size: "  << endl;



        }

    }








    // TIP See CLion help at <a href="https://www.jetbrains.com/help/clion/">jetbrains.com/help/clion/</a>. Also, you can try interactive lessons for CLion by selecting 'Help | Learn IDE Features' from the main menu.
}