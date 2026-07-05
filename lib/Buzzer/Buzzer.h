#ifndef BUZZER_H
#define BUZZER_H

#include <Arduino.h>

class Buzzer {
public:
    Buzzer(int _pin, bool _logic = true);
    void begin();
    void on();
    void off();
    void beep();
private:
    int _pin;
    bool _logic;
};

#endif