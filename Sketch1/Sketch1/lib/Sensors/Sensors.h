
#ifndef Sensors_H
#define Sensors_H

#define ONEWIRE_SEARCH 0
#define TEMP_PIN  A1
#define GROOVE_PIN 38
#include "Arduino.h"
#include "../DS18B20/src/DS18B20.h"

class Sensors
{
public:
    Sensors();
    void enable();
    void disable();
    void init();
    float readTemp();
private:
    uint8_t sensor_address[8];
    float temp;
private:
};

#endif