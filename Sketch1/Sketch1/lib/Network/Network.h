
#ifndef Network_H
#define Network_H

#include "Arduino.h"
#include "../../Parameters.h"
#include "../LoRaWan/LoRaWan.h"
#include "../Config/Config.h"
#include "../EnergySave/EnergySave.h"

class Network
{
public:
    Network();
    bool init(unsigned char timeout, boolean adr);
    bool join(unsigned char timeout);
	bool connected();
    bool transaferPayload(Config &config, byte *payload, unsigned char length, byte *downlink, bool confirmed = false);
    void printDeviceInfo();
private:
	char lora_buffer[256];
	char Appkey[33] = API_KEY;
	char AppEUI[17] = APP_EUI;
	int uplink_timeout = 10;
	bool joined = false;
	bool enable_adr = false;
};

#endif