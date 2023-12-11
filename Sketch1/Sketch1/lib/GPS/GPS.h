// GPS.h

#ifndef _GPS_h
#define _GPS_h
#include "Arduino.h"
#include "../../Parameters.h"
#include "../EnergySave/EnergySave.h"

class GPSClass
{
	protected:


	public:
	void init();
	void sleep();
	void wakeup();
	void getGpsData(int timeout);
	void loraDecode();
	char c;
	unsigned long age, fix_age, time, date, speed, course;
	float lat, lng;
};

void displayInfo();
extern GPSClass GPS;
extern int gps_lat;
extern int gps_lng;

#endif

