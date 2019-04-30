#include <Arduino.h>

#ifndef SIMPLE_ANALOG_SESNOR
#define SIMPLE_ANALOG_SENSOR

class SimpleAnalogSensor{
    private:
        uint8_t pin;
    public:
        void setup(uint8_t);
        int getValue();
};

void SimpleAnalogSensor::setup(uint8_t pin)
{
    this -> pin = pin;
}

int SimpleAnalogSensor::getValue()
{
    return analogRead(pin);
}

#endif