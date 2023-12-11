//
//
//

#include "GPS.h"
#include "../TinyGPS/TinyGPS.h"

TinyGPS gps_dev;

int gps_lat = 0, gps_lng = 0;

void GPSClass::init()
{
	Serial.begin(9600);
}

void GPSClass::sleep(){
	#if DEBUG
	SerialUSB.println("GPS Sleep");
	#endif
	Serial.println(" ");
	delay(200);
	Serial.println("$PMTK161,0*28");
}

void GPSClass::wakeup(){
	#if DEBUG
	SerialUSB.println("GPS Wake up ");
	#endif
	Serial.println(" ");
	delay(200);
}

void GPSClass::getGpsData(int timeout)
{
	bool newDATA = false;
	wakeup();

	// For {{GPS_SEARCH_TIME}} parse the GPS DATA and report some key values
	unsigned long start = millis();
	int gps_timeout = timeout * 1000;
	SerialUSB.print("============= GPS Timeout");
	SerialUSB.println(gps_timeout);
	while (!newDATA && millis() - start < gps_timeout )
	{
		while (Serial.available())
		{
			char c = Serial.read();
			SerialUSB.print(c);
			if (gps_dev.encode(c)) {
				// valid sentence ?
				newDATA = true;
			}
			
		}
	}

	if (newDATA)
	{
		gps_dev.f_get_position(&lat, &lng, &age);
		gps_dev.get_datetime(&date, &time, &fix_age);
		
		SerialUSB.println("");
		SerialUSB.print(" LAT=");
		lat =  TinyGPS::GPS_INVALID_F_ANGLE == lat ? 0.0 : lat;
		gps_lat =  round((lat + 91) * 100000);
		SerialUSB.print(gps_lat);

		SerialUSB.print(" LNG=");
		lng =  TinyGPS::GPS_INVALID_F_ANGLE == lng ? 0.0 : lng;
		gps_lng = round((lng + 181) * 100000);
		SerialUSB.println(gps_lng);
		
		SerialUSB.println("");
		SerialUSB.print("Lat: ");
		SerialUSB.print(lat, 6);
		SerialUSB.print(" Lng: ");
		SerialUSB.println(lng, 6);
		
	}
	else
	{
		gps_lat = 0;
		gps_lng = 0;
		SerialUSB.println("No GPS Data");
	}
	sleep();
	loraDecode();
}

void GPSClass::loraDecode(){

	float f_lat = 0, f_lng = 0;	
	
	f_lat = ( (float) gps_lat / 100000) - 91;
	f_lng = ( (float) gps_lng / 100000) - 181;
	
	SerialUSB.println("");
	SerialUSB.print(" Decoded Lat: ");
	SerialUSB.print(f_lat, 5);
	SerialUSB.print(" Lng: ");
	SerialUSB.println(f_lng, 5);
}

GPSClass GPS;