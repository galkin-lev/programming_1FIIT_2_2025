//
// Created by lv on 10/24/25.
//

#ifndef LAB6_MATRIX_UTILS_H
#define LAB6_MATRIX_UTILS_H


class Matrix {
private:
    int** data;
    int rows;
    int cols;

public:
    //Constructor
    Matrix(int** matrix, int n, int m);
    // Constructor
    Matrix(int n, int m);
    // Destructor
    ~Matrix();
    // Copy constructor
    Matrix(const Matrix& other);

    // Assignment operator (Rule of Three - don't forget this!)
    Matrix& operator=(const Matrix& other);

    // Getter methods
    int getRows() const;
    int getCols() const;
    int** getData() const;

    // Access elements
    int* operator[](int index);
    const int* operator[](int index) const;
};


// Function declarations
void sortRowsByAbsoluteSum(int** matrix, int n, int m);
bool hasDuplicateColumnsAndCheckPrimes(int** matrix, int n, int m);
void printMatrix(int** matrix, int n, int m);
void printArray(int arr[], int size);
bool compareArrays(int arr1[], int arr2[], int size);
bool isPrime(int num);
void deleteMatrix(int** matrix, int n);



Matrix* fillTheMatrix();



#endif //LAB6_MATRIX_UTILS_H