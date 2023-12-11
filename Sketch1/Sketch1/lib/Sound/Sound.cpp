#include "Sound.h"

Sound::Sound(int pin, int samples, int avg_samples)
{
    this->pin = pin;
    this->samples = samples;
    this->avg_samples = avg_samples;
    analogReadResolution(9);
}

void Sound::setADCSamples(int samples)
{
    SerialUSB.print("Set ADC Samples : ");
    SerialUSB.println(samples);
    this->avg_samples = samples;
}

void Sound::readSamples()
{
    unsigned long start = micros();
    oldTime = 0;
    for (int i = 0; i < this->samples; i++)
    {
        newTime = micros() - oldTime;
        vReal[i] = readAvg(this->pin, this->avg_samples);
        vImag[i] = 0;
        oldTime = newTime;
        wait(newTime + sampling_period_us);
    }
    calcTiming(start);
}

int Sound::readAvg(int pin, int samples)
{
    int sum = 0;
    for (int i = 0; i < samples; i++)
    {
        sum += analogReadFast(pin);
    }
    return (int)(sum / samples);
}

void Sound::wait(unsigned long time)
{
    while (micros() < time)
    {
        delay(0);
    }
}

void Sound::compute()
{
    FFT.Windowing(vReal, this->samples, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
    FFT.Compute(vReal, vImag, this->samples, FFT_FORWARD);
    FFT.ComplexToMagnitude(vReal, vImag, this->samples);
}

float Sound::calcDb(float calibration)
{

    int amp;
    float dB;
    int min_sound = ADC_RESOLUTION;
    int max_sound = 0;
    soundVolRMS = 0;
    soundVolRMSflt = 0;
    for (int i = 0; i < this->samples; i++)
    {
        // Peak To Peak
        amp = abs(vReal[i] - MAX_AMP);

        min_sound = min(min_sound, amp);
        max_sound = max(max_sound, amp);
        soundVolRMS += ((long)amp * amp);
    }

    soundVolRMS /= this->samples;
    soundVolRMSflt = sqrt(soundVolRMS);
    dB = 20.0 * log10(soundVolRMSflt / MAX_AMP) + calibration;

    SerialUSB.print("Db:");
    SerialUSB.println(dB);
    return dB;
}

int Sound::getVolume(int min, int max, float calibration)
{
    float db;
    db = calcDb(calibration);

    SerialUSB.print("Sound Min ");
    SerialUSB.println(min);
    SerialUSB.print("Sound Max ");
    SerialUSB.println(max);

    int volume = constrain(map(db, min, max, 0, 255), 0, 255);
    // map(value, fromLow, fromHigh, toLow, toHigh)

    SerialUSB.print("Volume ");
    SerialUSB.println(volume);
    return volume;
}

void Sound::calcTiming(unsigned long start)
{
    this->sampling_t = ((micros() - start) / 1000);
    this->sampling_f = (float)this->samples / (this->sampling_t) * 1000;
    this->bin_size = (this->sampling_f / 2) / (this->samples / 2);
}

unsigned long Sound::calcTotalAmp()
{
    unsigned long sum = 0;
    for (int i = 7; i < this->samples / 2; i++)
    {
        sum += min(10, vReal[i] * 1 / 100);
    }
    // SerialUSB.print("Db:");
    //SerialUSB.println(sum);
    // SerialUSB.println("");
    return sum;
}

void Sound::getSelectedBands(int *selected, int n, double *bands, int range, int max_threshold)
{
    int bin = 0;
    for (int i = 0; i < n; i++)
    {
        bin = selected[i];
        bands[i] = sumBands(bin, range, max_threshold);
        SerialUSB.print("Band ");
        SerialUSB.print(bin);
        SerialUSB.print(" ");
        SerialUSB.print(bin * bin_size);
        SerialUSB.print(" Hz Value ");
        SerialUSB.println(bands[i]);
    }
}

double Sound::sumBands(int band, int range, int max_threshold)
{
    int sum = 0;
    for (int i = 1; i <= range; i++)
    {
        sum += vReal[band + i];
    }
    for (int i = range; i >= 1; i--)
    {
        sum += vReal[band - i];
    }
    return constrain(map(sum, 0, max_threshold, 0, 124), 0, 124);
}

void Sound::printInfo()
{
    SerialUSB.print("Sampling Freq ");
    SerialUSB.print(this->sampling_f);
    SerialUSB.println("Hz");

    SerialUSB.print("Sampling Time ");
    SerialUSB.print(this->sampling_t);
    SerialUSB.println("ms");

    SerialUSB.print("Bin Size ");
    SerialUSB.print(this->bin_size);
    SerialUSB.println("Hz");

    SerialUSB.print("Avg Samples ");
    SerialUSB.println(this->avg_samples);

    SerialUSB.println("=================");
}

void Sound::printResults()
{
    for (int bin = 2; bin < (this->samples / 2); bin++)
    {
        if (vReal[bin] > 300)
        {
            printBars(bin, vReal[bin]);
        }
    }
}

void Sound::printBars(int band, int amp)
{
    // dBv is a logarithmic voltage ratio with a voltage reference of 1 Volt.
    // dBV = 20 * log10(V)
    // V = 10^(dBV/20)
    float freq = band * this->bin_size;

    SerialUSB.print("Band ");
    SerialUSB.print(band);
    SerialUSB.print(" (");

    SerialUSB.print(amp);
    SerialUSB.print(")");

    SerialUSB.print(freq);
    SerialUSB.print("Hz ");

    for (int i = 0; i < amp / 100; i++)
    {
        SerialUSB.print('.');
    }
    SerialUSB.println();
}