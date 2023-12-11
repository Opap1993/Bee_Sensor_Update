#include "Encoder.h"

Encoder::Encoder()
{
}

void Encoder::setGPS(float lat, float lng)
{
    gps_lat = round((lat + 91) * 1000000);
    gps_lng = round((lng + 181) * 1000000);
}

void Encoder::setTemp(float temp)
{
    this->temp = round(temp) + 100;
}

void Encoder::setVolume(int volume)
{
    this->volume = volume;
}


void Encoder::setPayloadType(enum payload_type type)
{
    this->type = type;
}

uint8_t Encoder::formatPrefix(byte *payload)
{
    payload[0] = 0;

    if (type == MEASUREMENT)
    {
        payload[0] |= 1 << 7;
    }
    else
    {
        payload[0] |= 1 << 6;
    }

    return 1;
}

unsigned char Encoder::getPayload(byte *payload)
{
    int payload_size = 0;

    payload_size += formatPrefix(payload);

    if (type == GPS)
    {
        payload_size += encodeGPSPayload(payload, payload_size);
    }

    payload_size += encodeTemp(payload, payload_size);
    payload_size += encodeVolume(payload, payload_size);
    payload_size += encodeBandSize(payload, payload_size);
    payload_size += encodeBands(payload, payload_size);
    return (unsigned char)payload_size;
}

uint8_t Encoder::encodeGPSPayload(byte *payload, int start)
{
    //Battery Voltage
    payload[start + 0] = (gps_lat >> 24) & 0xFF;
    payload[start + 1] = (gps_lat >> 16) & 0xFF;
    payload[start + 2] = (gps_lat >> 8) & 0xFF;
    payload[start + 3] = (gps_lat & 0XFF);

    //Gps Longtitude
    payload[start + 4] = (gps_lng >> 24) & 0xFF;
    payload[start + 5] = (gps_lng >> 16) & 0xFF;
    payload[start + 6] = (gps_lng >> 8) & 0xFF;
    payload[start + 7] = (gps_lng & 0XFF);

    return 8;
}

uint8_t Encoder::encodeTemp(byte *payload, uint16_t start)
{
    payload[start] = temp & 0xFF;
    return 1;
}

uint8_t Encoder::encodeVolume(byte *payload, uint16_t start)
{
    payload[start] = volume & 0xFF;
    return 1;
}

uint8_t Encoder::encodeBandSize(byte *payload, uint16_t start)
{
    payload[start] = (band_size >> 8) & 0xFF;
    payload[start + 1] = (band_size & 0XFF);
    return 2;
}

uint8_t Encoder::encodeBandLevel(byte *payload, uint16_t start, int band, int level)
{
    payload[start] = (level & 0x7F) << 1;
    payload[start] |= (band >> 8) & 0x01;
    payload[start + 1] = band & 0xff;

    return 2;
}

uint8_t Encoder::encodeBands(byte *payload, uint16_t start)
{
    int n = start;
    int c = 0;
    int band;
    int level;
    while (c < bands_n)
    {
        band = bands[c];
        level = amps[c];
        n += encodeBandLevel(payload, n, band, level);
        c++;
    }
    return n - start;
}

uint8_t Encoder::setBands(int *bands, double *amps, int bands_n, float band_size)
{
    memcpy(this->bands, bands, bands_n * sizeof(int));
    memcpy(this->amps, amps, bands_n * sizeof(double));

    this->band_size = round(band_size * 100);
    this->bands_n = bands_n;
}
