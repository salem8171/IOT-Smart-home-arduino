#include <Arduino.h>
#include <NewPing.h>
#include <AsynchronousDelayer.h>

#ifndef MOTION_SENSOR
#define MOTION_SENSOR

class MotionSensor
{
    private:
        NewPing* interior_ultrasonic_sensor;
        NewPing* exterior_ultrasonic_sensor;
        bool interior_obstacle_detected;
        bool exterior_obstacle_detected;
        AsynchronousDelayer interior_obstacle_cooldown;
        AsynchronousDelayer exterior_obstacle_cooldown;
        AsynchronousDelayer motion_cooldown;
        bool new_motion_detected;
        int new_motion_type;
    
    public:
        static const int MOTION_TYPE_INWARD = 0;
        static const int MOTION_TYPE_OUTWARD = 1;
        void setup(uint8_t, uint8_t, int, uint8_t, uint8_t, int);
        void handle();
        bool newMotionDetected();
        int getMotionType();
};

void MotionSensor::setup (
    uint8_t interior_ultrasonic_sensor_trig_pin,
    uint8_t interior_ultrasonic_sensor_echo_pin,
    int interior_ultrasonic_senosr_max_distance,
    uint8_t exterior_ultrasonic_sensor_trig_pin,
    uint8_t exterior_ultrasonic_sensor_echo_pin,
    int exterior_ultrasonic_sensor_max_distance
) {
    interior_ultrasonic_sensor = new NewPing (
        interior_ultrasonic_sensor_trig_pin,
        interior_ultrasonic_sensor_echo_pin,
        interior_ultrasonic_senosr_max_distance
    );
    exterior_ultrasonic_sensor = new NewPing (
        exterior_ultrasonic_sensor_trig_pin,
        exterior_ultrasonic_sensor_echo_pin,
        exterior_ultrasonic_sensor_max_distance
    );
    motion_cooldown.startTimer(0);
}

void MotionSensor::handle()
{
    if (! motion_cooldown.coolDownElapsed()) return;
    interior_obstacle_detected = ! interior_obstacle_cooldown.coolDownElapsed();
    exterior_obstacle_detected = ! exterior_obstacle_cooldown.coolDownElapsed();

    if (interior_ultrasonic_sensor -> ping_cm() != 0 && ! interior_obstacle_detected)
    {
        if (exterior_obstacle_detected)
        {
            new_motion_detected = true;
            new_motion_type = MOTION_TYPE_INWARD;
            motion_cooldown.startTimer(500);
        }
        else
        {
            interior_obstacle_detected = true;
            interior_obstacle_cooldown.restartTimer(100);
        }
    }

    delay(5);
    if (exterior_ultrasonic_sensor -> ping_cm() != 0 && ! exterior_obstacle_detected)
    {
        if (interior_obstacle_detected)
        {
            new_motion_detected = true;
            new_motion_type = MOTION_TYPE_OUTWARD;
            motion_cooldown.startTimer(500);
        }
        else
        {
            exterior_obstacle_detected = true;
            exterior_obstacle_cooldown.restartTimer(100);
        }
    }
}

bool MotionSensor::newMotionDetected()
{
    if (new_motion_detected)
    {
        new_motion_detected = false;
        return true;
    } else
    {
        return false;
    }
    
}

int MotionSensor::getMotionType()
{
    return new_motion_type;
}

#endif