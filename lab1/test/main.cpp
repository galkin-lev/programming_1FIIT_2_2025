#include <iostream>
#include <limits>
#include <climits>
#include <cfloat>
#include <cstdlib>
#include <iostream>
#include <random>

using namespace std;

template<typename T>
void printTypeInfo(const string& typeName, T value, T min, T max) {
    cout << "Тип: " << typeName
         << " | Размер: " << sizeof(value) << " байт"
         << " | Текущее значение: " << value
         << " | Мин: " << min
         << " | Макс: " << max << endl;
}

template<typename T>
void changeUnknownValue( T x) {

    cout << "\n#2  \n initial value: " << x << " \n changed value: " << (x+ (rand() % 101)) <<  endl;
}

int main() {
    // 1) Основные типы данных
    int i = 42;
    printTypeInfo("int", i, INT_MIN, INT_MAX);

    unsigned int i_unsigned = 42;
    printTypeInfo("unsigned int", i_unsigned, ::numeric_limits<unsigned int>::min() , UINT_MAX);

    float f = 3.14f;
    printTypeInfo("float", f, FLT_MIN, FLT_MAX);


    double d = 2.71828;
    printTypeInfo("double", d, DBL_MIN, DBL_MAX);

    long l = 100000L;
    printTypeInfo("long", l, LONG_MIN, LONG_MAX);

    unsigned long long l_unsigned = 4294967295;
    printTypeInfo("unsigned long", l_unsigned, numeric_limits<unsigned long long>::min(), ULLONG_MAX);

    bool b = true;
    printTypeInfo("bool", b, false, true);

    // 2) Работа с произвольным типом (int)
    int myVar = 100;
    cout << "\nИсходное значение myVar: " << myVar << endl;
    myVar += 50;
    cout << "Измененное значение myVar: " << myVar << endl;


    changeUnknownValue(i);

    return 0;
}