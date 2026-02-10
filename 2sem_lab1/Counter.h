//
// Created by lv on 1/20/26.
//

#ifndef COUNTER_H
#define COUNTER_H

#include <QObject>

class Counter
{
public:
    Counter();
    Counter(int startValue);
    ~Counter(); // deconstructor

    void increment();
    void decrement();
    int getValue() const;
    void reset();

private:
    int value;
};

#endif // COUNTER_H