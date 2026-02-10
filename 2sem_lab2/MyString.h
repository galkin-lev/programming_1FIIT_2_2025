//
// Created by lv on 2/3/26.
//

#ifndef LAB2_MYSTRING_H
#define LAB2_MYSTRING_H

#include <iostream>

using namespace std;

class MyString {
private:
    char* data;
    size_t length;

    void copyString(const char* str);
    void clear();

public:
    MyString();
    MyString(const char* str);

    // Правило трех
    ~MyString();
    MyString(const MyString& other);
    MyString& operator=(const MyString& other);

    // Перегруженные операторы
    MyString operator+(const MyString& other) const;
    bool operator==(const MyString& other) const;
    MyString& operator+=(const MyString& other);

    // Дружественные операторы ввода/вывода
    friend ostream& operator<<(ostream& os, const MyString& str);
    friend istream& operator>>(istream& is, MyString& str);

    // Методы доступа
    const char* c_str() const;
    size_t size() const;
    bool empty() const;

    // Метод с использованием this
    MyString& append(const MyString& other);
};

#endif //LAB2_MYSTRING_H