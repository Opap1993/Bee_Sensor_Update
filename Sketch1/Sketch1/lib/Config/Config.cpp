#include "Config.h"

FlashStorage(config_storage, remote_config);

Config::Config()
{
}

void Config::init()
{
    config = config_storage.read();
    if (config.valid == false)
    {
        SerialUSB.println("EEPROM Init");
        // Wake Up
        config.alarm_hour = DEFAULT_ALARM_HOUR;
        config.alarm_min = DEFAULT_ALARM_MINUTE;
        config.alarm_sec = DEFAULT_ALARM_SEC;

        config.enable_adr = DEFAULT_ENABLE_ADR;
        config.gps_timeout = DEFAULT_GPS_TIMEOUT_S;
        config.confirmed_uplink_interval = DEFAULT_CONFIRMED_UPLINK_INTERVAL;

        config.fft_threshold = MAX_THRESHOLD;
        config.sum_bands = SUM_BANDS;
        config.adc_samples = ADC_SAMPLES;

        config.vol_max = DB_MAX;
        config.vol_min = DB_MIN;

        // Bands
        config.bands[0] = 144;
        config.bands[1] = 197;
        config.bands[2] = 245;
        config.bands[3] = 96;
        config.bands[4] = 120;

        config.bands[5] = 168;
        config.bands[6] = 192;
        config.bands[7] = 216;
        config.bands[8] = 240;
        config.bands[9] = 264;

        config.valid = true;
        config_storage.write(config);

        SerialUSB.print("Config Alarm Sec ");
        SerialUSB.println(config.alarm_sec);
    }

    SerialUSB.println(config.bands[3]);
}

void Config::printDownlink(byte *downlink)
{
    for (int i = 0; i < MAX_DOWNLINK_SIZE; i++)
    {
        SerialUSB.print(downlink[i]);
        SerialUSB.print(" ");
    }
    SerialUSB.println();
}

void Config::applyConfig(byte *downlink)
{
    bool set_alarm = downlink[0] & (1 << 7);
    bool set_bands = downlink[0] & (1 << 6);
    bool sum_bands = downlink[0] & (1 << 5);
    bool fft_threshold = downlink[0] & (1 << 4);
    bool adc_samples = downlink[0] & (1 << 3);
    bool gps_timeout = downlink[0] & (1 << 2);
    bool save_to_eeprom = downlink[0] & 1;

    bool set_volume = downlink[1] & (1 << 7);

    int index = 2;

    if (set_alarm)
    {
        SerialUSB.print("Config Alarm Time: ");
        SerialUSB.println(index);
        setAlarm(downlink[index], downlink[index + 1], downlink[index + 2]);
        index += 3;
    }

    if (sum_bands)
    {
        SerialUSB.println("Config FFT Bands SUM: ");
        sumBands(downlink, index);
        index += 1;
    }

    if (fft_threshold)
    {
        SerialUSB.println("Config FFT Threshold: ");
        setFFTThreshold(downlink, index);
        index += 2;
    }

    if (adc_samples)
    {
        SerialUSB.println("Config ADC Samples: ");
        setADCSamples(downlink, index);
        index += 2;
    }

    if (gps_timeout)
    {
        SerialUSB.println("Config GPS Timeout: ");
        setGPSTimeout(downlink, index);
        index += 1;
    }

    if (set_volume)
    {
        SerialUSB.println("Config Volume Min - Max: ");
        setVolumeMinMax(downlink, index);
        index += 2;
    }

    if (set_bands)
    {
        SerialUSB.print("Set Bands: ");
        SerialUSB.println(index);
        setBands(downlink, index);
    }

    if (save_to_eeprom)
    {
        SerialUSB.println("Write to EEPROM");
        config_storage.write(config);
    }

    printDownlink(downlink);

    memset(downlink, 0, MAX_DOWNLINK_SIZE);
}

void Config::setVolumeMinMax(byte *downlink, int index)
{
    int min = downlink[index] - 125;
    int max = downlink[index + 1] - 125;

    config.vol_min = min;
    config.vol_max = max;

    SerialUSB.print("Volume min");
    SerialUSB.print(min);
    SerialUSB.print(" max");
    SerialUSB.println(max);

    SerialUSB.print(" index ");
    SerialUSB.println(index);
}

void Config::setGPSTimeout(byte *downlink, int index)
{
    int timeout = downlink[index] * 2;
    config.gps_timeout = timeout;
    SerialUSB.print("GPS Timeout ");
    SerialUSB.println(timeout);
    SerialUSB.print(" index ");
    SerialUSB.println(index);
}

void Config::setADCSamples(byte *downlink, int index)
{
    int samples = downlink[index];
    config.adc_samples = samples;
    SerialUSB.print("ADC Samples ");
    SerialUSB.println(samples);
    SerialUSB.print(" index ");
    SerialUSB.println(index);
}

void Config::sumBands(byte *downlink, int index)
{
    int sum = downlink[index];
    config.sum_bands = sum;
    SerialUSB.print("Sum Bands ");
    SerialUSB.println(sum);
    SerialUSB.print(" index ");
    SerialUSB.println(index);
}

void Config::setFFTThreshold(byte *downlink, int index)
{
    int max;
    max = (downlink[index] << 8);
    max |= downlink[index + 1];

    config.fft_threshold = max;
    SerialUSB.print("FFT Threshold ");
    SerialUSB.print(max);
    SerialUSB.print(" index ");
    SerialUSB.println(index);
}

void Config::setBands(byte *downlink, int index)
{
    int bands = downlink[index];
    index += 1;
    int band;

    SerialUSB.print("Bands ");
    SerialUSB.println(bands);

    for (int i = 0; i < bands; i++)
    {
        band = downlink[index] << 8;
        band |= downlink[index + 1];
        index += 2;
        config.bands[i] = band;
        SerialUSB.print("Band ");
        SerialUSB.print(i);
        SerialUSB.print(" ");
        SerialUSB.println(band);
    }
}

void Config::setAlarm(uint8_t hour, uint8_t min, uint8_t sec)
{
    if ((uint8_t)hour < 24 & (uint8_t)min < 60 & (uint8_t)sec < 60)
    {
        config.alarm_hour = (uint8_t)hour;
        config.alarm_min = (uint8_t)min;
        config.alarm_sec = (uint8_t)sec;

        SerialUSB.print("Alarm ");
        SerialUSB.print(config.alarm_hour);
        SerialUSB.print(":");
        SerialUSB.print(config.alarm_min);
        SerialUSB.print(":");
        SerialUSB.print(config.alarm_sec);
        SerialUSB.println();
    }
}

void Config::unconfirmedSent()
{
    unconfirmed_uplinks++;
}

void Config::confirmedSent()
{
    unconfirmed_uplinks = 0;
}