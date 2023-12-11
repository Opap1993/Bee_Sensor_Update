#include "Network.h"

Network::Network()
{
}

bool Network::init(unsigned char timeout, boolean adr_enabled)
{
	lora.init();
	EnergySave.loraWakeUp();

	printDeviceInfo();

	lora.setId(NULL, NULL, AppEUI);
	lora.setKey(NULL, NULL, Appkey);

	lora.setDeciveMode(LWOTAA);
	lora.setAdaptiveDataRate(enable_adr);
	lora.setDataRate(DR0, EU868);

	lora.setChannel(0, 868.1);
	lora.setChannel(1, 868.3);
	lora.setChannel(2, 868.5);

	lora.setReceiceWindowFirst(0, 868.1);
	lora.setReceiceWindowSecond(869.5, DR0);

	lora.setDutyCycle(false);
	lora.setJoinDutyCycle(false);

	lora.setPower(14);

	return join(timeout);
	EnergySave.loraSleep();
}

bool Network::join(unsigned char timeout)
{
	joined = lora.setOTAAJoin(JOIN, timeout);

	if (joined)
	{
		SerialUSB.println("------------ Joined Succeed ---------- ");
	}
	else
	{
		SerialUSB.println("------------ Join Failed ---------- ");
	}
	return joined;
}

bool Network::connected()
{
	return joined;
}

bool Network::transaferPayload(Config &config, byte *payload, unsigned char length, byte *downlink, bool confirmed)
{
	bool result = false;

	EnergySave.loraWakeUp();

	result = lora.transferPacket(payload, length, 10);
	config.unconfirmedSent();
	SerialUSB.print("#Unconfirmed Packets :");
	SerialUSB.println(config.unconfirmed_uplinks);

	SerialUSB.println(" +++++++++++++++++++++ Transfer Packet+++++++++++++++++++++++++ ");

	if (result)
	{
		short length;
		short rssi;

		memset(lora_buffer, 0, 256);
		length = lora.receivePacket(lora_buffer, 256, &rssi);
		if (length)
		{

			SerialUSB.print("Length is: ");
			SerialUSB.println(length);
			SerialUSB.print("RSSI is: ");
			SerialUSB.println(rssi);

			SerialUSB.println("------------------");
			SerialUSB.print("Data is: ");
			for (int i = 0; i < length; i++)
			{
				SerialUSB.print(lora_buffer[i], HEX);
				downlink[i] = lora_buffer[i];
			}
			SerialUSB.println();
			SerialUSB.println("------------------");
		}
	}
	else
	{
#if DEBUG
		SerialUSB.println("Packet Transmission: Failed");
#endif
	}
	EnergySave.loraSleep();
	return result;
}

void Network::printDeviceInfo()
{

	memset(lora_buffer, 0, 256);
	lora.getVersion(lora_buffer, 256, 1);
	SerialUSB.print(lora_buffer);

	memset(lora_buffer, 0, 256);
	lora.getId(lora_buffer, 256, 1);
	SerialUSB.print(lora_buffer);
}