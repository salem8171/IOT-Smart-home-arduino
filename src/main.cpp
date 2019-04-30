#include <Arduino.h>
#include <NewPing.h>
#include "SimpleAnalogSensor.h"
#include "SimpleDigitalOutput.h"
#include "MotionSensor.h"
#include "Config.h"

// NewPing interior_ultrasonic_sensor(INTERIOR_ULTRASONIC_SENSOR_TRIG_PIN, INTERIOR_ULTRASONIC_SENSOR_ECHO_PIN, INTERIOR_ULTRASONIC_SENSOR_MAX_DISTANCE);
// NewPing exterior_ultrasonic_sensor(EXTERIOR_ULTRASONIC_SENSOR_TRIG_PIN, EXTERIOR_ULTRASONIC_SENSOR_ECHO_PIN, EXTERIOR_ULTRASONIC_SENSOR_MAX_DISTANCE);

SimpleAnalogSensor light_sensor;
SimpleDigitalOutput light_bulb;
MotionSensor motion_sensor;

void setup()
{
    Serial.begin(9600);

    // Convenience hacks
    pinMode(16, OUTPUT); digitalWrite(16, HIGH);
    pinMode(19, OUTPUT); digitalWrite(19, LOW);
    pinMode(3, OUTPUT); digitalWrite(3, HIGH);

    // Components setup
    light_sensor.setup(LIGHT_SENSOR_PIN);
    light_bulb.setup(LIGHT_BULT_PIN);
    motion_sensor.setup(
        INTERIOR_ULTRASONIC_SENSOR_TRIG_PIN,
        INTERIOR_ULTRASONIC_SENSOR_ECHO_PIN,
        EXTERIOR_ULTRASONIC_SENSOR_MAX_DISTANCE,
        EXTERIOR_ULTRASONIC_SENSOR_TRIG_PIN,
        EXTERIOR_ULTRASONIC_SENSOR_ECHO_PIN,
        EXTERIOR_ULTRASONIC_SENSOR_MAX_DISTANCE
    );
}

void loop()
{
    // Handlers
    motion_sensor.handle();

    // Publish events
    if (motion_sensor.newMotionDetected())
    {
        if (motion_sensor.getMotionType() == motion_sensor.MOTION_TYPE_INWARD) Serial.println("Inward");
        if (motion_sensor.getMotionType() == motion_sensor.MOTION_TYPE_OUTWARD) Serial.println("Outward");
    }
    light_sensor.getValue();
}