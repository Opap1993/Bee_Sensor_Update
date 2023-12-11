#include "Sensors.h"
DS18B20 ds(TEMP_PIN);

Sensors::Sensors()
{
    enable();
    delay(100);
    ds.getAddress(sensor_address);
}

float Sensors::readTemp() {
    int sensors = ds.getNumberOfDevices();
    if (sensors == 0) {
        return -6;
    }

    temp = ds.getTempC();
    SerialUSB.print("Temp :");
    SerialUSB.println(temp);
    return temp;
}

void Sensors::enable() {
    pinMode(GROOVE_PIN, OUTPUT);
    digitalWrite(GROOVE_PIN, HIGH);
}

void Sensors::disable() {
    pinMode(GROOVE_PIN, OUTPUT);
    digitalWrite(GROOVE_PIN, LOW);
}