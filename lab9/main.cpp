#include <iostream>
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


void printArray(int arr[], int size) {
    cout << "[";
    for(int i = 0; i < size; i++) {
        cout << arr[i];
        if(i < size - 1) cout << ", ";
    }
    cout << "]" ;
}
int getgetFirstDigit(int number) { // by dividing by 10
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
    int firstA = getgetFirstDigit(a);
    int firstB = getgetFirstDigit(b);
    if (firstA != firstB) return firstA < firstB; // 1. first comparison: if first digit of b > a then a <-> b

    int sumA = getDigitDigitsSum(a);
    int sumB = getDigitDigitsSum(b);
    if (sumA != sumB) return sumA < sumB; // 2. compare by the sum of digits (same)

    return a < b;
}

using namespace std;

// linked node - structure where one element stores the pointer for the next ( not a List - linked List)
/* kotlin:
* class LinkedNode(var value: Int) {
    var next: LinkedNode? = null // Следующий указатель на следующий узел хранения
}
 */

//  A sequence of such linked nodes forms structures like linked lists, where the last node's pointer is set to NULLPTR!!! to mark the end
struct Node {
    int data;
    Node* next; // pointer to the next Node
    Node(int val) : data(val), next(nullptr) {}
};

/*
* Node1: [data: 5, next: 0x1000]  ← Node1's 'next' stores ADDRESS 0x1000
                    ↓
                    points to
                    ↓
Node2: [data: 3, next: 0x2000]  ← This Node2 exists at address 0x1000
                    ↓
                    points to
                    ↓
Node3: [data: 8, next: nullptr] ← This Node3 exists at address 0x2000
 */

void append(Node*& head, int val) {
    Node* newNode = new Node(val);
    if (!head) {
        head = newNode;
        return;
    }
    Node* temp = head;
    while (temp->next) {
        temp = temp->next;
    }
    temp->next = newNode;
}


bool isPrime(int n) {
    if (n <= 1) return false;
    if (n == 2) return true;
    if (n % 2 == 0) return false;

    for (int i = 3; i <= sqrt(n); i += 2) { // heavy
        if (n % i == 0) {
            return false;
        }
    }
    return true;
}

// check whether the prime number has digits (1,5,7)
bool hasDigits(int n) {
    while (n > 0) {
        int digit = n % 10;
        if (digit == 1 || digit == 5 || digit == 7) return true;
        n /= 10;
    }
    return false;
}

int getFirstDigit(int n) {
    n = abs(n);
    while (n >= 10) n /= 10;
    return n;
}

int getLastDigit(int n) {
    return abs(n) % 10;
}

// (305, 207) check order by first or the last digit
bool isOrdered(Node* head) {
    if (!head || !head->next) return true;

    //flags
    bool firstDigitOrder = true;
    bool lastDigitOrder = true;
    Node* curr = head;

    while (curr->next) {
        // compare first digits of the number
        // Проверяем невозрастание для первых цифр
        if (getFirstDigit(curr->data) < getFirstDigit(curr->next->data)) {
            firstDigitOrder = false;
        }
        // compare the last digits of the number
        // Проверяем невозрастание для последних цифр
        if (getLastDigit(curr->data) < getLastDigit(curr->next->data)) {
            lastDigitOrder = false;
        }
        curr = curr->next;
    }
    return firstDigitOrder || lastDigitOrder;
}



void removeNotPrimes(Node*& head) {
    Node* curr = head;
    Node* prev = nullptr;

    while (curr) {
        if (!isPrime(curr->data) && (curr->data != 1)) {
            cout << "Removing composite: " << curr->data << endl; // Отладочный вывод
            if (!prev) {
                head = curr->next;
                delete curr;
                curr = head;
            } else {
                prev->next = curr->next;
                delete curr;
                curr = prev->next;
            }
        } else {
            prev = curr;
            curr = curr->next;
        }
    }
}

void duplicatePrimes(Node*& head) {
    Node* curr = head;
    while (curr) {
        if (isPrime(curr->data) && hasDigits(curr->data)) {
            Node* newNode = new Node(curr->data);
            newNode->next = curr->next;
            curr->next = newNode;
            curr = newNode->next;
        } else {
            curr = curr->next;
        }
    }
}


//
void sortList(Node*& head) {
    if (!head || !head->next) return;
    
    Node* sorted = nullptr;
    Node* curr = head;

    while (curr) {
        Node* next = curr->next;
        if (!sorted || sorted->data >= curr->data) {
            curr->next = sorted;
            sorted = curr;
        } else {
            Node* temp = sorted;
            while (temp->next && temp->next->data < curr->data) {
                temp = temp->next;
            }
            curr->next = temp->next;
            temp->next = curr;
        }
        curr = next;
    }
    head = sorted;
}


void printList(Node* head) {
    while (head) {
        cout << head->data << " ";
        head = head->next;
    }
    cout << endl;
}

void deleteList(Node*& head) {
    while (head) {                    // Continue until head becomes nullptr
        Node* temp = head;            // 1. Store current head in temporary pointer
        head = head->next;            // 2. head == new node (link to the next pointer which is stored as (Node *next))
        delete temp;                  // 3. Delete the node by its pointer
    }
}





int main() {
    // TIP Press <shortcut actionId="RenameElement"/> when your caret is at the <b>lang</b> variable name to see how CLion can help you rename it.


    /* * and & repeat (address stored as number -> int* ptr -> int* is a type-safe way to say "this variable holds integer addresses"

* int number = 42;        // A regular variable

// & gets the address
cout << &number << endl; // Output: 0x7ffeefbff5c8 (memory address)

// * accesses the value at an address
int* pointer = &number; // pointer stores address of number
cout << *pointer << endl; // Output: 42 (value at that address)




int number = 42;

// CORRECT: int* tells compiler this stores address of an integer
int* ptr = &number;
cout << *ptr << endl;  // Output: 42

// The compiler knows:
// - ptr points to an integer
// - Integers are 4 bytes (typically)
// - So when I do *ptr, read 4 bytes from that address

    */





/*2.  Ввести последовательность натуральных чисел {Aj}j=1...n (n<=1000). Упорядочить последовательность по неубыванию
 *первой цифры числа, числа с одинаковыми первыми цифрами дополнительно упорядочить по неубыванию суммы цифр числа, числа
 *с одинаковыми первыми цифрами и одинаковыми суммами цифр дополнительно упорядочить по неубыванию самого числа.
 */

    Node* head = nullptr;
    int num;

    cout << "Enter numbers for linked list (0 to stop):" << endl;

    // create linked NOde
    while (cin >> num && num != 0) {
        append(head, num);
    }

    cout << "Original list: ";
    printList(head);

    // Проверка упорядоченности
    bool ordered = isOrdered(head);
    cout << "Is ordered by first/last digit: " << (ordered ? "YES" : "NO") << endl;

    if (ordered) {
        cout << "Removing composite numbers..." << endl;
        removeNotPrimes(head);
        cout << "After removing composites: ";
        printList(head);

        cout << "Duplicating primes with digits 1,5,7..." << endl;
        duplicatePrimes(head);
        cout << "Final result: ";
        printList(head);
    } else {
        cout << "Sorting list..." << endl;
        sortList(head);
        cout << "Sorted list: ";
        printList(head);
    }

    // Очистка памяти
    deleteList(head);

    return 0;
    // TIP See CLion help at <a href="https://www.jetbrains.com/help/clion/">jetbrains.com/help/clion/</a>. Also, you can try interactive lessons for CLion by selecting 'Help | Learn IDE Features' from the main menu.
}