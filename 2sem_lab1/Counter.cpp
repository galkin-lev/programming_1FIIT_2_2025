//
// Created by lv on 1/20/26.
//

#include "Counter.h"
#include <iostream>

using namespace std;

Counter::Counter() : value(0)
{
    cout << "Created a counter,with default constructor (value: 0)" << endl;
}

Counter::Counter(int startValue)
{
    if (startValue < 0) {
        value = 0;
    } else {
        value = startValue;
    }
    cout << "Created a counter, with a not default constructor (value: " << value << ")" << endl;
}

Counter::~Counter()
{
    cout << "Destructor is called, final counter value: " << value << endl;
}

void Counter::increment()
{
    value++;
}


void Counter::decrement()
{
    value--;
}

int Counter::getValue() const
{
    return value;
}

void Counter::reset()
{
    value = 0;
}