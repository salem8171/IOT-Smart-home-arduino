#include <Arduino.h>
#include <NewPing.h>
#include "SimpleAnalogSensor.h"
#include "SimpleDigitalOutput.h"
#include "MotionSensor.h"
#include "SerialMqtt.h"
#include "Config.h"

SimpleAnalogSensor light_sensor;
SimpleDigitalOutput light_bulb;
MotionSensor motion_sensor;
SerialMqtt serial_mqtt;

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
        new NewPing(INTERIOR_ULTRASONIC_SENSOR_TRIG_PIN, INTERIOR_ULTRASONIC_SENSOR_ECHO_PIN, INTERIOR_ULTRASONIC_SENSOR_MAX_DISTANCE),
        new NewPing(EXTERIOR_ULTRASONIC_SENSOR_TRIG_PIN, EXTERIOR_ULTRASONIC_SENSOR_ECHO_PIN, EXTERIOR_ULTRASONIC_SENSOR_MAX_DISTANCE)
    );

    serial_mqtt.setup(new SoftwareSerial(SOFTWARE_SERIAL_RX_PIN, SOFTWARE_SERIAL_TX_PIN));
    serial_mqtt.setCallback([](SerialMqtt::MqttData data)
    {
        if (
            data.topic_type == SerialMqtt::TopicType::cmd &&
            data.location == SerialMqtt::Location::bed_room &&
            data.topic_specification == SerialMqtt::TopicSpecification::bulb
        )
        {
            if (data.payload) light_bulb.turnOn(); else light_bulb.turnOff();
        }
    });
}

void loop()
{
    // Handlers
    motion_sensor.handle();
    serial_mqtt.handle();

    // Publish events
    if (motion_sensor.newMotionDetected())
    {
        if (motion_sensor.getMotionType() == MotionSensor::MOTION_TYPE_INWARD) serial_mqtt.send({
            SerialMqtt::TopicType::sensor,
            SerialMqtt::Location::bed_room,
            SerialMqtt::TopicSpecification::motion,
            MotionSensor::MOTION_TYPE_INWARD
        });
        if (motion_sensor.getMotionType() == MotionSensor::MOTION_TYPE_OUTWARD) serial_mqtt.send({
            SerialMqtt::TopicType::sensor,
            SerialMqtt::Location::bed_room,
            SerialMqtt::TopicSpecification::motion,
            MotionSensor::MOTION_TYPE_OUTWARD
        });
    }
    
    serial_mqtt.send({
        SerialMqtt::TopicType::sensor,
        SerialMqtt::Location::bed_room,
        SerialMqtt::TopicSpecification::light,
        light_sensor.getValue()
    });
}