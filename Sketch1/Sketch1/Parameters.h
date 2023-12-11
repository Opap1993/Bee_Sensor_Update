#ifndef PARAMETERS_H
#define PARAMETERS_H

#define MAX_PAYLOAD_SIZE 20
#define MAX_DOWNLINK_SIZE 30

#define I2C_VCC_ENABLE_PIN 38
#define MIC_PIN A3

// === LoRaWAN
#define DEFAULT_CONFIRMED_UPLINK_INTERVAL 96;
#define DEFAULT_ENABLE_ADR false;
#define API_KEY "6589A3E7BAC1CD6F060883F3AB260DBB"
#define APP_EUI "70B3D57ED0023770"

// FFT
#define MAX_THRESHOLD 8000//8000
#define SUM_BANDS 2
#define ADC_SAMPLES 23//15

// Energy Save Alarm
#define RTC_MONTH 0
#define RTC_DAY 0
#define RTC_YEAR 0

// ============== Downlink Configuration
#define DEFAULT_ALARM_HOUR 1
#define DEFAULT_ALARM_MINUTE 0
#define DEFAULT_ALARM_SEC 0

#define DEFAULT_GPS_TIMEOUT_S 60

#define DB_MIN -46
#define DB_MAX 0

#define DEBUG true

#endif