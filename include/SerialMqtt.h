#include <Arduino.h>
#include <SoftwareSerial.h>

#ifndef SERIAL_MQTT
#define SERIAL_MQTT

class SerialMqtt
{
    // Types definitions
    public:
        typedef enum TopicType {cmd, sensor} TopicType;
        typedef enum Location {kitchen, living_room, bed_room} Location;
        typedef enum TopicSpecification {motion, light, bulb} TopicSpecification;

        typedef struct MqttData
        {
            TopicType topic_type;
            Location location;
            TopicSpecification topic_specification;
            int payload;
        } MqttData;
    
    private:
        typedef union TransferContainer
        {
            MqttData data;
            byte dataMirror[sizeof(MqttData)];
        } TransferContainer;

    // Attributes and methods
    private:
        SoftwareSerial* serial;
        bool is_available;
        TransferContainer send_container;
        TransferContainer recieve_container;
        void (*callback)(MqttData);
    
    public:
        void setup(SoftwareSerial*);
        void send(MqttData);
        bool isAvailable();
        MqttData receive();
        void setCallback(void (*)(MqttData));
        void handle();
};

void SerialMqtt::setup(SoftwareSerial* serial)
{
    this -> serial = serial;
    serial -> begin(9600);
}

void SerialMqtt::send(MqttData mqtt_data)
{
    send_container.data.topic_type = mqtt_data.topic_type;
    send_container.data.location = mqtt_data.location;
    send_container.data.topic_specification = mqtt_data.topic_specification;
    send_container.data.payload = mqtt_data.payload;

    // serial -> write(255);
    for (unsigned int i = 0; i < sizeof(MqttData); i++) serial -> write(send_container.dataMirror[i]);
}

bool SerialMqtt::isAvailable()
{
    return serial -> available() >= sizeof(MqttData);
}

SerialMqtt::MqttData SerialMqtt::receive()
{
    // while (serial -> read() != 255);
    for (unsigned int i = 0; i < sizeof(MqttData); i++) recieve_container.dataMirror[i] = serial -> read();
    return recieve_container.data;
}

void SerialMqtt::setCallback(void (*callback)(MqttData))
{
    this -> callback = callback;
}

void SerialMqtt::handle()
{
    if (isAvailable()) callback(receive());
}

#endif