#include <iostream>
#include <any>
#include <iostream>
#include <iostream>
#include <cmath>
#include <format>
#include <iomanip>
#include <oneapi/tbb/detail/_task.h>
#include "matrix_utils.h"  // Include your header file


#include <algorithm>
#include <string>
#include <cstdlib>
#include <ctime>

using namespace std;

const int matrixDefaultSize = 100;

// TIP To <b>Run</b> code, press <shortcut actionId="Run"/> or click the <icon src="AllIcons.Actions.Execute"/> icon in the gutter.

// void printMatrix(int matrix[][matrixDefaultSize], int n, int m) {
//
//     // Вывод с индексами столбцов
//     cout << "   ";  // отступ для строки с индексами
//     for (int j = 0; j < m; j++) {
//         cout << setw(3) << " [" << j << "]";
//     }
//     cout << endl;
//
//     // Вывод данных с индексами строк
//     for (int i = 0; i < n; i++) {
//         cout << "[" << i << "]";
//         for (int j = 0; j < m; j++) {
//             cout << setw(4) << matrix[i][j] << " ";
//         }
//         cout << endl;
//     }
// }
//
// void printMatrix(int** matrix, int n, int m) {
//
//     // Вывод с индексами столбцов
//     cout << "   ";  // отступ для строки с индексами
//     for (int j = 0; j < m; j++) {
//         cout << setw(3) << " [" << j +1<< "]";
//     }
//     cout << endl;
//
//     // Вывод данных с индексами строк
//     for (int i = 0; i < n; i++) {
//         cout << "[" << i +1<< "]";
//         for (int j = 0; j < m; j++) {
//             cout << setw(4) << matrix[i][j] << " ";
//         }
//         cout << endl;
//     }
// }
//
//
// bool isPrime(int n) {
//     if (n <= 1) return false;
//     if (n == 2) return true;
//     if (n % 2 == 0) return false;
//
//     for (int i = 3; i <= sqrt(n); i += 2) { // heavy
//         if (n % i == 0) {
//             return false;
//         }
//     }
//     return true;
// }
//
//
//
// int** fillTheMatrix() {
//
//     unsigned int n;
//     unsigned int m;
//
//     while ( true  ) {
//         cout << "Enter matrix size (n) (n<=10000) : ";
//         cin >> n;
//
//         if ( cin.fail() || n > 100 ) {
//             cin.clear(); // Clear error flag
//             cin.ignore( numeric_limits< streamsize>::max(), '\n'); // Discard bad input
//             cout << "Invalid input. Please enter a valid number.\n";
//         } else {
//             break; // Exit loop on valid input
//         }
//     }
//
//     while ( true  ) {
//         cout << "Enter matrix size (m) (m<=100) columns : ";
//         cin >> m;
//
//         if ( cin.fail() || m > 100 ) {
//             cin.clear(); // Clear error flag
//             cin.ignore( numeric_limits< streamsize>::max(), '\n'); // Discard bad input
//             cout << "Invalid input. Please enter a valid number.\n";
//         } else {
//             break; // Exit loop on valid input
//         }
//     }
//
//
//     int** matrix = new int*[matrixDefaultSize];  // Array of n pointers
//     // fill the matrix
//     for (int i = 0; i < n; ++i) {
//         matrix[i] = new int[m];  // Allocate each row ONCE
//
//
//         // sums[i] = 0;
//         for (int j = 0; j < m; j++) {
//
//
//
//                 cout << "Enter number [" << i +1 << "] [" << j +1 << "] of the matrix (should be a natural number):";
//
//                  cin >> matrix[i][j]; //
//
//         }
//     }
//
//     cout << "Your matrix is: " << endl;
//     printMatrix(matrix, n, m);
//
//     return matrix;
// }
//
// void printArray(int arr[], int size) {
//     cout << "[";
//     for(int i = 0; i < size; i++) {
//         cout << arr[i];
//         if(i < size - 1) cout << ", ";
//     }
//     cout << "]" ;
// }
//
// bool compareArrays(int arr1[], int arr2[], int size) {
//     for (int i = 0; i < size; i++) {
//         if (arr1[i] != arr2[i]) {
//             return false;  // Found a mismatch - arrays are different
//         }
//     }
//     return true;  // All elements are equal
// }
//
// // Check if there are at least two identical columns
// bool hasDuplicateColumnsAndCheckPrimes(int** matrix, int n, int m) {
//
//     bool duplicateFlag = false;
//     bool primeFlag = false;
//
//     // // int previousM = 0;
//     // int currentM = 0;
//     int collumn1[10000];
//     int collumn2[10000];
//
//
//     for (int currentM = 0; currentM < n; currentM++) {
//         cout << "comparison by " << currentM + 1 << " collumn:" << endl;
//         for (int j = 0; j < m; j++) {
//
//         for (int i = 0; i < n; i++) {
//
//              collumn1[i] = matrix[i][currentM];
//              collumn2[i] = matrix[i][j];
//
//
//             if (isPrime(abs(matrix[i][j]))) {
//                 // cout << "result: element a{" << i+1 << ", " << j+1 << "} = "<<matrix[i][j] <<  " is a PRIME in abs >> condition met >> primeFlag is true";
//                 primeFlag = true;
//             }
//         }
//             if (currentM != j) {
//
//                 cout << "           collumn " << currentM +1<< ": ";
//                 printArray(collumn1, n);
//                 cout << endl;
//                 cout << "           collumn " << j +1 << ": ";
//                 printArray(collumn2, n);
//                 cout << endl;
//
//                 if (compareArrays(collumn1, collumn2, n)) {
//                     cout << "result: column " << currentM+1 << " and " << j+1 << " are the SAME >> condition met >> duplicateFlag is true" << endl;
//                     duplicateFlag = true;
//                 };
//             }
//             cout << "-------------------------------------------------------------" << endl;
//     }
//     }
//
//     if (duplicateFlag && primeFlag) {
//         cout << "duplicate flag is collected\nprime flag is collected\n >> conditions met, beginning matrix transformation..." << endl;
//
//         return true;
//     }
//
//     cout << "duplicate flag is " << duplicateFlag << "\nprime flag is " << primeFlag << "\n >> conditions not met, exiting..." << endl;
//     return false;
// }
//
//
//
// // Function to delete matrix and free memory
// void deleteMatrix(int** matrix, int n) {
//     for (int i = 0; i < n; i++) {
//         delete[] matrix[i];
//     }
//     delete[] matrix;
// }
//
// void sortRowsByAbsoluteSum(int** matrix, int n, int m) {
//     // Создаем массив для хранения сумм модулей элементов каждой строки
//     int* sums = new int[n];
//
//     // Вычисляем суммы модулей для каждой строки
//     for (int i = 0; i < n; ++i) {
//         sums[i] = 0;
//         for (int j = 0; j < m; ++j) {
//             sums[i] += abs(matrix[i][j]);
//         }
//     }
//
//     // Сортируем строки матрицы по неубыванию суммы с помощью пузырьковой сортировки
//     for (int i = 0; i < n - 1; ++i) {
//         for (int j = 0; j < n - i - 1; ++j) {
//             if (sums[j] > sums[j + 1]) {
//                 // Обмен сумм
//                 swap(sums[j], sums[j + 1]);
//                 // Обмен указателей на строки
//                 swap(matrix[j], matrix[j + 1]);
//             }
//         }
//     }
//
//     // printMatrix(matrix, n, m);
//
//     // Освобождаем память, выделенную для массива сумм
//     delete[] sums;
// }

// int** fillTheMatrix() {
//
//     unsigned int n;
//     unsigned int m;
//
//     while ( true  ) {
//         cout << "Enter matrix size (n) (n<=100) strings : ";
//         cin >> n;
//
//         if ( cin.fail() || n > 100 ) {
//             cin.clear(); // Clear error flag
//             cin.ignore( numeric_limits< streamsize>::max(), '\n'); // Discard bad input
//             cout << "Invalid input. Please enter a valid number.\n";
//         } else {
//             break; // Exit loop on valid input
//         }
//     }
//
//     while ( true  ) {
//         cout << "Enter matrix size (m) (m<=100) columns : ";
//         cin >> m;
//
//         if ( cin.fail() || m > 100 ) {
//             cin.clear(); // Clear error flag
//             cin.ignore( numeric_limits< streamsize>::max(), '\n'); // Discard bad input
//             cout << "Invalid input. Please enter a valid number.\n";
//         } else {
//             break; // Exit loop on valid input
//         }
//     }
//
//
//     int** matrix = new int*[matrixDefaultSize];  // Array of n pointers
//     // fill the matrix
//     for (int i = 0; i < n; ++i) {
//         matrix[i] = new int[m];  // Allocate each row ONCE
//
//
//         // sums[i] = 0;
//         for (int j = 0; j < m; j++) {
//
//
//
//             cout << "Enter number [" << i +1 << "] [" << j +1 << "] of the matrix (should be a natural number):";
//
//             cin >> matrix[i][j]; //
//
//         }
//     }
//
//     cout << "Your matrix is: " << endl;
//     printMatrix(matrix, n, m);
//
//     return matrix;
// }

// class Matrix {
// private:
//     int** data;
//     int rows;
//     int cols;
//
// public:
//     Matrix(int n, int m) : rows(n), cols(m) {
//         data = new int*[rows];
//         for (int i = 0; i < rows; i++) {
//             data[i] = new int[cols];
//         }
//     }
//
//     ~Matrix() {
//         for (int i = 0; i < rows; i++) {
//             delete[] data[i];
//         }
//         delete[] data;
//     }
//
//     // Getter methods
//     int getRows() const { return rows; }
//     int getCols() const { return cols; }
//     int** getData() const { return data; }
//
//     // Access elements
//     int* operator[](int index) { return data[index]; }
//     const int* operator[](int index) const { return data[index]; }
//
//     // Copy constructor and assignment operator (rule of three)
//     Matrix(const Matrix& other) : rows(other.rows), cols(other.cols) {
//         data = new int*[rows];
//         for (int i = 0; i < rows; i++) {
//             data[i] = new int[cols];
//             for (int j = 0; j < cols; j++) {
//                 data[i][j] = other.data[i][j];
//             }
//         }
//     }
// };

int main() {
    // Create and fill matrix
    //

    Matrix* myMatrix = fillTheMatrix();

    // Matrix myMatrix(3, 4);

    unsigned int n = myMatrix->getRows();
    unsigned int m = myMatrix->getCols();


    //
    // while ( true  ) {
    //     cout << "Enter matrix size (n) (n<=100) strings : ";
    //     cin >> n;
    //
    //     if ( cin.fail() || n > 100 ) {
    //         cin.clear(); // Clear error flag
    //         cin.ignore( numeric_limits< streamsize>::max(), '\n'); // Discard bad input
    //         cout << "Invalid input. Please enter a valid number.\n";
    //     } else {
    //         break; // Exit loop on valid input
    //     }
    // }
    //
    // while ( true  ) {
    //     cout << "Enter matrix size (m) (m<=100) columns : ";
    //     cin >> m;
    //
    //     if ( cin.fail() || m > 100 ) {
    //         cin.clear(); // Clear error flag
    //         cin.ignore( numeric_limits< streamsize>::max(), '\n'); // Discard bad input
    //         cout << "Invalid input. Please enter a valid number.\n";
    //     } else {
    //         break; // Exit loop on valid input
    //     }
    // }
    //
    //
    // int** matrix = new int*[matrixDefaultSize];  // Array of n pointers
    // // fill the matrix
    // for (int i = 0; i < n; ++i) {
    //     matrix[i] = new int[m];  // Allocate each row ONCE
    //
    //
    //     // sums[i] = 0;
    //     for (int j = 0; j < m; j++) {
    //
    //
    //
    //             cout << "Enter number [" << i +1 << "] [" << j +1 << "] of the matrix (should be a natural number):";
    //
    //              cin >> matrix[i][j]; //
    //
    //     }
    // }
    //
    // cout << "Your matrix is: " << endl;
    // printMatrix(matrix, n, m);


    // Process the matrix according to requirements
    // matrix = processMatrix(matrix, n, m);

    if (hasDuplicateColumnsAndCheckPrimes(myMatrix->getData(), n, m)) {
        cout << "";

        sortRowsByAbsoluteSum(myMatrix->getData(), n, m);

    };
    cout << "\nFinal Matrix:" << endl;
    printMatrix(myMatrix->getData(), n, m);

    // Clean up memory
    deleteMatrix(myMatrix->getData(), n);

    return 0;
}

/*
 *Дана целочисленная матрица {Aij}i=1...n;j=1..n , n<=100. Если в матрице есть два одинаковых
 *столбца и есть хотя бы один элемент, абсолютная величина которого - простое число, упорядочить строки матрицы по неубыванию суммы модулей элементов.
 */