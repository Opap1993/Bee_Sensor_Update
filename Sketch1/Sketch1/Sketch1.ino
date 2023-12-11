//#ifndef UNIT_TEST
#include "Arduino.h"
#include "lib/Sound/Sound.h"
#include "Parameters.h"
#include "lib/Sensors/Sensors.h"
#include "lib/Network/Network.h"
#include "lib/Config/Config.h"
#include "lib/Encoder/Encoder.h"
#include "lib/GPS/GPS.h"
#include "lib/EnergySave/EnergySave.h"
#include "lib/LoRaWan/LoRaWan.h"
//#include "lib/DS18B20/src/DS18B20.h"
//#include "lib/RTCZero/RTCZero.h"
//#include "lib/FFT/src/arduinoFFT.h"
//#include "lib/FixFTT/src/fix_fft.h"
//#include "lib/OneWire/OneWire.h"
//#include "lib/FlashStorage/FlashStorage.h"


Sound sound = Sound(MIC_PIN, 1024, ADC_SAMPLES);
Sensors sensors = Sensors();
Network network = Network();
Config config = Config();
Encoder encoder = Encoder();

byte payload[MAX_PAYLOAD_SIZE] = {0};
byte downlink[MAX_DOWNLINK_SIZE] = {0};

void getBands();
void spectrumAnalyzer();
void alarmMatch();
void sleep();

void setup()
{
	SerialUSB.begin(115200);
	config.init();

	EnergySave.init();
	EnergySave.lowPowerPins(false);
	
	network.init(10, true);
	EnergySave.setMCUSleepMode(alarmMatch);

	GPS.init();
	GPS.getGpsData(config.config.gps_timeout);
}

void loop()
{
	int payload_size = 0;
	int band_n;
	double amps[7] = {0};
	float temp = 0;
	int volume = 0;

	if (!network.connected())
	{
		network.join(10);
	}
	else
	{
		if (config.unconfirmed_uplinks == 0)
		{
			encoder.setPayloadType(encoder.GPS);
			band_n = 4;//4
		}
		else
		{
			band_n = 8;
			encoder.setPayloadType(encoder.MEASUREMENT);
		}

		EnergySave.enableI2CDevices();
		delay(1000);
		sound.setADCSamples(config.config.adc_samples);
		sound.readSamples();

		volume = sound.getVolume(config.config.vol_min, config.config.vol_max);
		encoder.setVolume(volume);
		
		sound.compute();

		sound.printInfo();

		sound.getSelectedBands(config.config.bands, band_n, amps, config.config.sum_bands, config.config.fft_threshold);
		encoder.setBands(config.config.bands, amps, band_n, sound.bin_size);

		temp = sensors.readTemp();
		encoder.setTemp(temp);

		EnergySave.disableI2CDevices();

		encoder.setGPS(GPS.lat, GPS.lng);

		payload_size = encoder.getPayload(payload);
		SerialUSB.print("Payload Size");
		SerialUSB.println(payload_size);
		network.transaferPayload(config, payload, payload_size, downlink, false);
		config.applyConfig(downlink);
	}
	sleep();
}

void sleep(){
	EnergySave.setAlarmDateTime(config.config.alarm_hour,config.config.alarm_min, config.config.alarm_sec);
	EnergySave.standByMode();
}

void alarmMatch() {

}

//#endif