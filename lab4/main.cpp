#include <any>
#include <iostream>
#include <iostream>
#include <cmath>
#include <format>
#include <iomanip>
#include <oneapi/tbb/detail/_task.h>

#include <algorithm>
#include <string>
#include <cstdlib>
#include <ctime>

using namespace std;

// TIP To <b>Run</b> code, press <shortcut actionId="Run"/> or click the <icon src="AllIcons.Actions.Execute"/> icon in the gutter.

// Функция для проверки, начинается ли число с цифры 1

const int matrixDefaultSize = 100;

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


// 1 exercise functions ////////////////////////////////////////////////
bool startsWithOne(int num) { // with string
    string str = to_string(num);
    return !str.empty() && str[0] == '1';
}
////////////////////////////////////////////////////////////////////////

// 2 exercise functions ////////////////////////////////////////////////
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
////////////////////////////////////////////////////////////////////////

// 3 exercise //////////////////////////////////////////////////////////
int findMaxIndex(int arr[], int size) {
    if (size < 0) return -1; // Handle empty array

    int maxIndex = 0;
    for (int i = 1; i < size; i++) {
        if (arr[i] < arr[maxIndex]) {
            maxIndex = i;
        }
    }
    return maxIndex;
}
////////////////////////////////////////////////////////////////////////
// 4 exercise //////////////////////////////////////////////////////////
bool isPrime(int n) {
    if (n <= 1) return false;
    if (n == 2) return true;
    if (n % 2 == 0) return false;

    for (int i = 3; i <= sqrt(n); i += 2) { // heavy
        if (n % i == 0) {
            return false;
        }
    }
    cout << "found prime number " << n <<  ". doubling..." << endl;
    return true;
}
bool contains7not6(int num) {
    if (num == 7) return true;
    string numStr = to_string(num);
    bool has7 = false;
    bool has6 = false;

    for (char digit : numStr) {
        if (digit == '7') has7 = true;
        if (digit == '6') has6 = true;
    }

    return (has7 && !has6);
}
////////////////////////////////////////////////////////////////////////




int main() {

    /*
    #include <cstdlib>
    #include <ctime>
     */
    srand(time(0)); // Initialize random number generator
    int random_number_range = 10;
    int randomNumber = rand() % random_number_range + 1;
    cout << "Secret number range: " << randomNumber << endl;


    int tasknum;

    while (true) {
        cout << "Enter the exercise (1-4) or press any key to exit: ";
        cin >> tasknum;



              // var 1
/*
 * 1.  Дана последовательность натуральных чисел {aj}j=1...n (n<=10000). Если в последовательности есть хотя
 * бы одно число, начинающееся цифрой 1, упорядочить последовательность по неубыванию.
 */
        if (tasknum == 1) { // 1 задание

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

                    if ( cin.fail() || sequence[i] < 0) { // Проверка, что число натуральное (положительное)
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

                    if ( cin.fail() || sequence[i] < 0) { // Проверка, что число натуральное (положительное)
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


/*
 * 3.  Дана целочисленная матрица {Aij}i=1..n,j=1..m (n,m<=100). Найти строку, сумма элементов которой наиболее близка к 0,
 * и заменить все элементы этой строки числом 0.
 */
        if ( tasknum == 3 ) {
            
            unsigned int n;
            unsigned int m;
            while ( true  ) {
                cout << "Enter matrix size (n) (n<=100) rows : ";
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
                cout << "Enter matrix size (m) (m<=100) columns : ";
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
            int sums[10000];

            // fill the matrix
            for (int i = 0; i < n; ++i) {
                sums[i] = 0;
                for (int j = 0; j < m; j++) {

                while ( true  ) {
                    cout << "Enter number [" << i << "] [" << j << "] of the matrix (should be a natural number):";

                    cin >> matrix[i][j];

                     sums[i] += matrix[i][j];
                    if ( cin.fail() || matrix[i][j] < 0) { // Проверка, что число натуральное (положительное)
                        cin.clear(); // Clear error flag
                        cin.ignore( numeric_limits< streamsize>::max(), '\n'); // Discard bad input
                        cout << "Invalid input. Please enter a valid number.\n";
                    } else {
                        break; // Exit loop on valid input
                    }
                }
                }
            }

            cout << "entered matrix:" << endl;
            printMatrix(matrix, n, m);

            cout << "row with the lowest sum value: " << findMaxIndex(sums, n) << endl;

            cout << "the matrix with the nullified row" << endl;
            for (int i = 0; i < m; ++i) {
                matrix[findMaxIndex(sums, n)][i] = 0;
            }
            printMatrix(matrix, n, m);



        }

/*
* 4. Дана последовательность натуральных чисел {Aj}j=1...n (n<=10000). Удалить из последовательности числа, содержащие цифру 7, но не содержащие
* цифру 6, а среди оставшихся продублировать простые числа.(Сначала введите последовательность. Затем удалите и продублируйте элементы.
* Затем выведите полученную последовательность (каждый элемент по одному разу). Используйте в программе только один массив.)
 */
        if (tasknum == 4) {
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
            int sequence[n]; // why
            // fill the sequence
            for (int i = 0; i < n; ++i) {


                while ( true  ) {
                    cout << "Enter number №" << i << " of the sequence (should be a natural number):";

                    cin >> sequence[i];

                    if ( cin.fail() || sequence[i] < 0) { // Проверка, что число натуральное (положительное)
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

            // for ( unsigned int i = 0; i < n; ++i ) {
            //     if (to_string(sequence[i]).find_first_of('7') != string::npos && !to_string(sequence[i]).find_first_of('6') != string::npos) {
            //         // for (int j = i; j < n - 1; ++j) {
            //         //     sequence[j] uence[j + 1];
            //             sequence[n] = NULL;
            //
            //         // }
            //         --n;
            //         cout << "sequence size: " << n << endl;
            //     }
            // }

            // recreating filtered array
            int newSize = 0;
            for (int i = 0; i < n; i++) {
                if (!contains7not6(sequence[i])) {
                    sequence[newSize] = sequence[i]; // basically a new array from the old one
                    newSize++;
                } else {
                    cout << "found a number with 7 and without 6: " << sequence[i] << ". deleting..." << endl;
                }
            }

            // doubling prime numbers
            for (int i = 0; i < newSize; i++) {
                if (isPrime(sequence[i])) {

                    // move all numbers after prime -> right (from the end)
                    for (int j = newSize; j > i + 1; j--) {
                        sequence[j] = sequence[j - 1];
                    }

                    // in vacant place insert a duplicate prime number
                    sequence[i + 1] = sequence[i];
                    newSize++;
                    i++;
                }
            }


            cout << "sorted sequence:";
            printArray(sequence, newSize); cout << endl;



        }


        if (tasknum != 1 && tasknum != 2 && tasknum != 3 && tasknum != 4) {
            cout << "exiting..." << endl;
            return 0;
        }


    }

}