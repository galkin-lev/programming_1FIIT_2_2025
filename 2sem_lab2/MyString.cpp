//
// Created by lv on 2/3/26.
//

#include "MyString.h"
#include <cstring>

using namespace std;

void MyString::copyString(const char* str) {
    if (str) {
        length = strlen(str);
        data = new char[length + 1];
        strcpy(data, str);
    } else {
        data = new char[1];
        data[0] = '\0';
        length = 0;
    }
}

void MyString::clear() {
    delete[] data;
    data = nullptr;
    length = 0;
}

MyString::MyString() {
    data = new char[1];
    data[0] = '\0';
    length = 0;
}

MyString::MyString(const char* str) {
    copyString(str);
}


MyString::~MyString() {
    clear();
}

MyString::MyString(const MyString& other) {
    copyString(other.data);
}

// Оператор присваивания
MyString& MyString::operator=(const MyString& other) {
    if (this != &other) {
        clear();
        copyString(other.data);
    }
    return *this;
}

// overloading standard operators
MyString MyString::operator+(const MyString& other) const {
    char* newData = new char[length + other.length + 1];
    strcpy(newData, data);
    strcat(newData, other.data);

    MyString result(newData);
    delete[] newData;
    return result;
}

bool MyString::operator==(const MyString& other) const {
    return strcmp(data, other.data) == 0;
}

MyString& MyString::operator+=(const MyString& other) {
    size_t newLength = length + other.length;
    char* newData = new char[newLength + 1];

    strcpy(newData, data);
    strcat(newData, other.data);

    delete[] data;
    data = newData;
    length = newLength;

    return *this;
}

MyString& MyString::append(const MyString& other) {
    return *this += other;
}

const char* MyString::c_str() const {
    return data;
}

size_t MyString::size() const {
    return length;
}

bool MyString::empty() const {
    return length == 0;
}

// Дружественные операторы ввода/вывода
ostream& operator<<(ostream& os, const MyString& str) {
    os << str.data;
    return os;
}

istream& operator>>(istream& is, MyString& str) {
    char buffer[1024];
    is >> buffer;
    str = MyString(buffer); // Используем оператор присваивания
    return is;
}