
#ifndef Encoder_H
#define Encoder_H

#define ONEWIRE_SEARCH 0
#define TEMP_PIN A1
#define GROOVE_PIN 38
#include "Arduino.h"
#include "../DS18B20/src/DS18B20.h"

class Encoder
{
public:
    Encoder();
    void setGPS(float lat, float lng);
    void setTemp(float temp);
    void setVolume(int volume);

    uint8_t setBands(int *bands, double *amps, int bands_n, float band_size);
    unsigned char getPayload(byte *payload);
    uint8_t encodeBandLevel(byte *payload, uint16_t start, int band, int level);
    uint8_t formatPrefix(byte *payload);

    enum payload_type
    {
        GPS,
        MEASUREMENT
    };
    payload_type type;
    void setPayloadType(enum payload_type type);


private:
    uint8_t encodeGPSPayload(byte *payload, int start);
    uint8_t encodeTemp(byte *payload, uint16_t start);
    uint8_t encodeVolume(byte *payload, uint16_t start);
    uint8_t encodeBands(byte *payload, uint16_t start);
    uint8_t encodeBandSize(byte *payload, uint16_t start);

    int gps_lng = 0;
    int gps_lat = 0;
    int temp = 0;
    int volume = 0;

    int bands_n = 0;
    int band_size = 0;

    int bands[10] = {0};
    double amps[10] = {0};

    int payload_size = 0;
};

#endif