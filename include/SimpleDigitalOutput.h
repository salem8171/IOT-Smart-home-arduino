#include <Arduino.h>

#ifndef SIMPLE_DIGITAL_OUTPUT
#define SIMPLE_DIGITAL_OUTPUT

class SimpleDigitalOutput
{
    private:
        uint8_t pin;
    
    public:
        void setup(uint8_t);
        void turnOn();
        void turnOff();
};

void SimpleDigitalOutput::setup(uint8_t pin)
{
    this -> pin = pin;
    pinMode(pin, OUTPUT);
}

void SimpleDigitalOutput::turnOn()
{
    digitalWrite(pin, HIGH);
}

void SimpleDigitalOutput::turnOff()
{
    digitalWrite(pin, LOW);
}

#endif