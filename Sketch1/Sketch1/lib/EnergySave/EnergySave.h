// EnergySave.h

#ifndef _ENERGYSAVE_h
#define _ENERGYSAVE_h
#include "../RTCZero/RTCZero.h"
#include "LoRaWan.h"
#include "../../Parameters.h"

class EnergySaveClass
{
	protected:
	RTCZero rtc;
	int alarm_hour = 0;
	int alarm_sec = 0;
	int alarm_min = 0;

	public:
	void init();
	void setMCUSleepMode(voidFuncPtr alarmMatchCallback);
	void standByMode();
	void setAlarmDateTime(uint8_t h, uint8_t min, uint8_t s);
	void loraWakeUp();
	void loraSleep();
	void lowPowerPins(bool led_removed);
	void enableI2CDevices();
	void disableI2CDevices();
};

extern EnergySaveClass EnergySave;

#endif

