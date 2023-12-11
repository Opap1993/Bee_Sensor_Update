#include "EnergySave.h"

void EnergySaveClass::init()
{
	lora.init();
}

void EnergySaveClass::setMCUSleepMode(voidFuncPtr alarmMatchCallback){
	#if DEBUG
	SerialUSB.println("Setup MCU Sleep Mode");
	#endif
	rtc.begin();

	rtc.setTime(0, 0, 0);
	rtc.setDate(RTC_MONTH, RTC_DAY, RTC_YEAR);

	rtc.enableAlarm(rtc.MATCH_HHMMSS); // MATCH_HHMMSS

	rtc.attachInterrupt(alarmMatchCallback);
}

void EnergySaveClass::standByMode(){
	#if DEBUG
	SerialUSB.println("Stand By Mode");
	#endif
	rtc.standbyMode();
}

void EnergySaveClass::setAlarmDateTime(uint8_t h, uint8_t m, uint8_t s){
	rtc.setTime(0, 0, 0);
	rtc.setAlarmTime(h, m, s);
	SerialUSB.print("EnergySave Set  Alarm ");
	SerialUSB.print(h);
	SerialUSB.print(":");
	SerialUSB.print(m);
	SerialUSB.print(":");
	SerialUSB.println(s);
}

void EnergySaveClass::loraWakeUp(){
	#if DEBUG
	SerialUSB.println("Lora Wake up");
	#endif
	Serial1.println("AT");
	delay(200);
}

void EnergySaveClass::loraSleep(){
	#if DEBUG
	SerialUSB.println("Lora Sleep");
	#endif
	Serial1.println("AT");
	delay(200);
	lora.setDeviceLowPower();
}

void EnergySaveClass::lowPowerPins(bool led_removed){
	#if DEBUG
	SerialUSB.println("Low power pins");
	#endif
	for (unsigned char i = 0; i < 26; i++) // important, set all pins to HIGH to save power
	{
		if( i != 18 && i != 19){
			pinMode(i, OUTPUT);
			digitalWrite(i, HIGH);
		}
	}
	
	if(!led_removed){
		digitalWrite(13, LOW);
	}
}

void EnergySaveClass::enableI2CDevices(){
	#if DEBUG
	SerialUSB.println("Enable I2C Devices");
	#endif
	pinMode(I2C_VCC_ENABLE_PIN, OUTPUT);
	digitalWrite(I2C_VCC_ENABLE_PIN, HIGH);
}

void EnergySaveClass::disableI2CDevices()
{
	#if DEBUG
	SerialUSB.println("Disable I2C devices");
	#endif
	pinMode(I2C_VCC_ENABLE_PIN, OUTPUT);
	digitalWrite(I2C_VCC_ENABLE_PIN, LOW);
}

EnergySaveClass EnergySave;

