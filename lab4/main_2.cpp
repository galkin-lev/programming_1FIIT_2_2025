#include <iostream>
#include <limits>
#include <string>

using namespace std;

int main() {
    setlocale(LC_ALL, "ru");

    // Цены на напитки
    const int WATER_PRICE = 30;
    const int COLA_PRICE = 50;
    const int ENERGY_DRINK_PRICE = 80;

    int choice;
    int amount;
    int price = 0;
    string drink_name;

    cout << "=== АВТОМАТ ПО ПРОДАЖЕ НАПИТКОВ ===" << endl;
    cout << "1. Вода - " << WATER_PRICE << " руб." << endl;
    cout << "2. Кола - " << COLA_PRICE << " руб." << endl;
    cout << "3. Энергетик - " << ENERGY_DRINK_PRICE << " руб." << endl;
    cout << "===================================" << endl;




    while ( true  ) {

        // Выбор напитка
        cout << "Выберите напиток (1-3): ";
        cin >> choice;

        if ( cin.fail() || choice > 3 || choice < 1 ) { // pow(10,9) || 10 000 000 00
            cin.clear(); // Clear error flag
            cin.ignore( numeric_limits< streamsize>::max(), '\n'); // Discard bad input
            cout << "Неправильный ввод!\n";
        } else {
            break; // Exit loop on valid input
        }
    }


    // Проверка выбора и установка цены
    switch(choice) {
        case 1:
            price = WATER_PRICE;
            drink_name = "Вода";
            break;
        case 2:
            price = COLA_PRICE;
            drink_name = "Кола";
            break;
        case 3:
            price = ENERGY_DRINK_PRICE;
            drink_name = "Энергетик";
            break;
        default:
            cout << "Ошибка: неверный выбор напитка!" << endl;
            return 1;
    }

    // Ввод суммы
    cout << "Вы выбрали: " << drink_name << " (" << price << " руб.)" << endl;
    cout << "Внесите сумму: ";
    cin >> amount;

    // Проверка суммы и выдача результата
    if (amount < price) {
        cout << "Недостаточно средств! Не хватает " << (price - amount) << " руб." << endl;
        cout << "Деньги возвращены." << endl;
    } else if (amount == price) {
        cout << "Спасибо! Заберите ваш напиток: " << drink_name << endl;
        cout << "Сдача: 0 руб." << endl;
    } else {
        int change = amount - price;
        cout << "Спасибо! Заберите ваш напиток: " << drink_name << endl;
        cout << "Сдача: " << change << " руб." << endl;
    }

    return 0;
}