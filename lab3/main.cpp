#include <iostream>
#include <cstdlib>
#include <ctime>


using namespace std;

int main() {
    setlocale(LC_ALL, "ru");
    // srand(time(0));
    int n = 10;
    int secret_number = 5;//rand() % n + 1;
    int guess, attempts = 0;

    cout << "Я загадал число от 1 до " << n << ". Угадай его!" << endl;

    do {
        cout << "Введите вашу догадку: ";
        cin >> guess;
        attempts++;

        if (guess < secret_number) {
            cout << "Слишком мало!" << endl;
        } else if (guess > secret_number) {
            cout << "Слишком много!" << endl;
        }
    } while (guess != secret_number);


   /*  последняя цифра - 1 - попытку
        2, 3, 4, - попытки
        5 - 0 - попыток */
    if (attempts % 10 == 1) {
         cout << "Точно! Ты угадал за " << attempts << " попытку!" << endl;

    } else if (attempts % 10 == 2 || attempts % 10 == 3 || attempts % 10 == 4) {
            cout << "Точно! Ты угадал за " << attempts << " попытки!" << endl;

    } else {
        cout << "Точно! Ты угадал за " << attempts << " попыток!" << endl;
    }



    return 0;
}

