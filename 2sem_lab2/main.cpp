#include <QCoreApplication>
#include <QDebug>

#include "MyString.h"
#include <iostream>
using namespace std;

void demonstrateRuleOfThree() {
    cout << "=== Демонстрация правила трех ===\n" << endl;

    // 1. Создание объектов
    MyString s1("Hello");
    MyString s2("World");

    cout << "Исходные объекты:" << endl;
    cout << "s1: " << s1 << endl;
    cout << "s2: " << s2 << endl;

    // 2. Конструктор копирования (часть правила трех)
    cout << "\n1. Конструктор копирования:" << endl;
    MyString s3 = s1; // Вызывается конструктор копирования
    cout << "MyString s3 = s1; // Конструктор копирования" << endl;
    cout << "s3: " << s3 << endl;

    // 3. Оператор присваивания (часть правила трех)
    cout << "\n2. Оператор присваивания:" << endl;
    MyString s4;
    s4 = s2; // Вызывается оператор присваивания
    cout << "s4 = s2; // Оператор присваивания" << endl;
    cout << "s4: " << s4 << endl;

    // 4. Демонстрация глубокого копирования
    cout << "\n3. Глубокое копирование (не поверхностное):" << endl;
    MyString original("Оригинал");
    MyString copy = original;

    // Меняем original, copy не должен измениться
    original = MyString("Измененный");
    cout << "original: " << original << endl;
    cout << "copy: " << copy << " (остался неизменным!)" << endl;

    // 5. Проверка на самоприсваивание
    cout << "\n4. Проверка на самоприсваивание:" << endl;
    MyString s5("Test");
    cout << "До самоприсваивания: " << s5 << endl;
    s5 = s5; // Самоприсваивание должно быть безопасным
    cout << "После s5 = s5: " << s5 << " (корректно обработано)" << endl;

    // 6. Деструктор вызывается автоматически при выходе из области видимости
    cout << "\n5. Деструкторы вызываются автоматически при выходе из функции" << endl;
}

void demonstrateOperators() {
    cout << "\n=== Демонстрация перегруженных операторов ===\n" << endl;

    MyString a("Hello");
    MyString b("World");
    MyString c("Hello");

    cout << "a = " << a << ", b = " << b << ", c = " << c << endl;

    // Оператор +
    cout << "\n1. Оператор + (конкатенация):" << endl;
    MyString sum = a + b;
    cout << "a + b = " << sum << endl;

    // Оператор ==
    cout << "\n2. Оператор == (сравнение):" << endl;
    cout << "a == b: " << (a == b ? "true" : "false") << endl;
    cout << "a == c: " << (a == c ? "true" : "false") << endl;

    // Оператор +=
    cout << "\n3. Оператор +=:" << endl;
    MyString d("Start");
    cout << "До: d = " << d << endl;
    d += a;
    cout << "После d += a: " << d << endl;

    // Цепочка операторов
    cout << "\n4. Цепочка операторов:" << endl;
    MyString e("Hello");
    e += MyString(" ") += b += MyString("!");
    cout << "e += \" \" += b += \"!\": " << e << endl;
}

void demonstrateThisPointer() {
    cout << "\n=== Демонстрация использования указателя this ===\n" << endl;

    MyString s("Начало");

    // Метод append возвращает *this, что позволяет делать цепочку вызовов
    cout << "Использование this для цепочки вызовов:" << endl;
    s.append(MyString(" середина")).append(MyString(" конец"));
    cout << "Результат: " << s << endl;
}

int main() {
    cout << "=== Программа демонстрации класса MyString ===\n" << endl;

    demonstrateRuleOfThree();
    demonstrateOperators();
    demonstrateThisPointer();

    cout << "\n=== Все требования выполнены ===" << endl;
    cout << "1. ✓ Динамическое поле char* data" << endl;
    cout << "2. ✓ Правило трех реализовано" << endl;
    cout << "3. ✓ Три перегруженных оператора (+, ==, +=)" << endl;
    cout << "4. ✓ Использование указателя this" << endl;
    cout << "5. ✓ Разделение кода на файлы" << endl;

    return 0;
}