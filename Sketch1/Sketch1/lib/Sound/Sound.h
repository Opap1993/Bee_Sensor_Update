
#ifndef SOUND_H
#define SOUND_H
#define MAX_SAMPLES 1024
#define MAX_ADC_BITS 1024
#define MAX_AMP 512
#include "Arduino.h"
#include "../FFT/src/arduinoFFT.h"
#include "../FastADC/AnalogReadFast.h"
#include "math.h"

class Sound
{
public:
    Sound(int pin, int samples, int avg_samples);
    void readSamples();
    void compute();
    void printResults();
    void printInfo();
    void getSelectedBands(int *selected, int n, double *bands, int range, int max_threshold);
    double sumBands(int start, int range, int max_threshold);
    float calcDb(float calibration = 0.0);
    int getVolume(int min, int max, float calibration = 0.0);
    void setADCSamples(int samples);
    unsigned long calcTotalAmp();

    float bin_size;
private:
    arduinoFFT FFT = arduinoFFT();
    int pin = A1;
    int avg_samples = 1;

    int soundVolRMS;
    float soundVolRMSflt;

    int samples = 1024;
    float sampling_f;
    float sampling_t;
    unsigned int sampling_period_us;
    unsigned long microseconds;

    unsigned long newTime, oldTime;

    double vReal[MAX_SAMPLES];
    double vImag[MAX_SAMPLES];

    int selected[10][10];

    void calcTiming(unsigned long start);
    int readAvg(int pin, int samples);
    void wait(unsigned long time);
    void printBars(int band, int amp);
};

#endif