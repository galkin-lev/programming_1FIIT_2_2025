#include <iostream>

// TIP To <b>Run</b> code, press <shortcut actionId="Run"/> or click the <icon src="AllIcons.Actions.Execute"/> icon in the gutter.
int main() {



    double value = 3.14159;
    // pointer stored in the stack and the value itself in the heap
    double* pointer1 = &value;
    // creating one more pointer in the stack referring to the same value in heap
    double* pointer2 = pointer1 + 2; // Сдвиг на 2 элемента вперёд

    std::cout << "Адрес pointer1: " << pointer1 << std::endl;
    std::cout << "Адрес pointer2: " << pointer2 << std::endl;
    std::cout << "Разность (в байтах): "
              << reinterpret_cast<char*>(pointer2) - reinterpret_cast<char*>(pointer1)
              << std::endl;

    // * - pointer - points to the value in the address
    // & - to get an address of the variable

    // deferencing - разыменывание = изменение переменной через
    // указатель (по аддрессу)
/*Разыменование указателя не является безопасным, потому что C++ не
 *гарантирует автоматическое перемещение или управление памятью — в отличие,
 *например, от языков с сборщиком мусора (как Java или Python).*/

    // может не хватить памяти и все сломается


    // 2

    std::cout << std::endl;
    char* arr = new char[8];
 // new char[] - returns address to the first arr element in the heap
    // to which the arr pointer refers

    char* ptr = arr;
    for (int i = 0; i < 8; i++) { // iterates through the dynamic array by pointing to the next element in it
        *(ptr + i) = 'A' + i;
    }

    // Выводим элементы массива
    ptr = arr; // Сбрасываем указатель на начало массива
    for (int i = 0; i < 8; i++) {
        std::cout << *(ptr + i) << " "; // Доступ через арифметику указателей
    }
    std::cout << std::endl;

    // Освобождаем память // delete all the array instead of only the first value (like with: delete arr;)
    delete[] arr;

    return 0;


    return 0;
    // TIP See CLion help at <a href="https://www.jetbrains.com/help/clion/">jetbrains.com/help/clion/</a>. Also, you can try interactive lessons for CLion by selecting 'Help | Learn IDE Features' from the main menu.
}