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

// SoftwareSerial bluetooth(SOFTWARE_SERIAL_RX_PIN, SOFTWARE_SERIAL_TX_PIN);

void setup()
{
    Serial.begin(9600);
    // bluetooth.begin(9600);
    // return;

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

    // Mqtt setup
    serial_mqtt.setup(new SoftwareSerial(SOFTWARE_SERIAL_RX_PIN, SOFTWARE_SERIAL_TX_PIN));
    // serial_mqtt.connect(); // Bluetooth client only

    serial_mqtt.setCallback([](SerialMqtt::MqttData data)
    {
        // Serial.print("topic: "); Serial.println(data.topic_type);
        // Serial.print("local: "); Serial.println(data.location);
        // Serial.print("spec: "); Serial.println(data.topic_specification);
        // Serial.print("payla: "); Serial.println(data.payload);
        // Serial.println(data.payload);
        serial_mqtt.send(data);
        return;
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

// typedef enum TopicType {cmd, sensor} TopicType;
// typedef enum Location {kitchen, living_room, bed_room} Location;
// typedef enum TopicSpecification {motion, light, bulb} TopicSpecification;

// typedef struct MqttData
// {
//     TopicType topic_type;
//     Location location;
//     TopicSpecification topic_specification;
//     int payload;
// } MqttData;

// typedef union TransferContainer
// {
//     MqttData data;
//     byte dataMirror[sizeof(MqttData)];
// } TransferContainer;

void loop()
{
    // if (Serial.available()) bluetooth.write(Serial.read());
    // if (bluetooth.available()) Serial.write(bluetooth.read());
    // Serial.print(0); Serial.print(".");
    // Serial.print(1); Serial.print(".");
    // Serial.print(2); Serial.print(".");
    // Serial.println(100);

    // TransferContainer recieve_container;
    // if( Serial.available() >= sizeof(MqttData))
    // {
        
    // }

    // return;

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
    
    return;
    serial_mqtt.send({
        SerialMqtt::TopicType::sensor,
        SerialMqtt::Location::bed_room,
        SerialMqtt::TopicSpecification::light,
        light_sensor.getValue()
    });
}