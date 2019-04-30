#include <Arduino.h>

#ifndef SIMPLE_ANALOG_SESNOR
#define SIMPLE_ANALOG_SENSOR

class SimpleAnalogSensor{
    private:
        uint8_t pin;
        int threshold;
    public:
        void setup(uint8_t);
        void setup(uint8_t, int);
        int getValue();
        void setThreshold(int);
        bool hasReachedThreshold();
};

void SimpleAnalogSensor::setup(uint8_t pin)
{
    this -> pin = pin;
}

void SimpleAnalogSensor::setup(uint8_t pin, int threshold)
{
    setup(pin);
    setThreshold(threshold);
}

int SimpleAnalogSensor::getValue()
{
    return analogRead(pin);
}

void SimpleAnalogSensor::setThreshold(int threshold)
{
    this -> threshold = threshold;
}

bool SimpleAnalogSensor::hasReachedThreshold()
{
    return getValue() >= threshold;
}

#endif