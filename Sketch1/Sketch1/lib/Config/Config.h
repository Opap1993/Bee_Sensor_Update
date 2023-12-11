
#ifndef Config_H
#define Config_H

#include "Arduino.h"
#include "../../Parameters.h"
#include "FlashStorage.h"

typedef struct
{
    uint8_t alarm_hour;
    uint8_t alarm_min;
    uint8_t alarm_sec;
    uint16_t confirmed_uplink_interval;
    int gps_timeout;
    boolean enable_uplink_confirmation;
    boolean enable_adr;
    boolean valid;
    int vol_min;
    int vol_max;
    int sum_bands;
    int adc_samples;
    int fft_threshold = 4000;
    int bands[10] = {0};
} remote_config;

class Config
{
public:
    Config();
    void init();
    void unconfirmedSent();
    void confirmedSent();
    void applyConfig(byte *downlink);
    void printDownlink(byte *downlink);

    // Config
    void setAlarm(uint8_t hour, uint8_t min, uint8_t sec);
    void setBands(byte *downlink, int index);
    void sumBands(byte *downlink, int index);
    void setVolumeMinMax(byte *downlink, int index);
    void setFFTThreshold(byte *downlink, int index);
    void setADCSamples(byte *downlink, int index);
    void setGPSTimeout(byte *downlink, int index);


    uint16_t unconfirmed_uplinks;
    uint8_t uplinks_to_send = 0;
    boolean send_configuration = false;
    remote_config config;
private:
};

#endif