#include "ArduinoBusMaster.h"
#include "Arduino.h"
#include <Wire.h>


ArduinoBusMaster::ArduinoBusMaster(bool SerialDebug) {
	this->SerialDebug = SerialDebug;
}

void ArduinoBusMaster::begin(byte address) {
	this->address = address;
	Wire.begin();
}

bool ArduinoBusMaster::pinMode(byte pin, byte mode) {
	char writebuffer[] = { 0x01, pin, mode };
	SendRequest(writebuffer);
	if (Wire.available()) {
		char receivebuffer[3];
		Wire.readBytes(receivebuffer, 3);
		printbuf(receivebuffer, "Response:");
		if (memcmp(writebuffer, receivebuffer, 3) == 0) {
			return true;
		}
		else {
			if (this->SerialDebug) {
				Serial.println("pinMode Response Mismatch!");
				Serial.println();
			}
		}
	}
	if (this->SerialDebug) Serial.println("No answer received!");
	return false;
}

bool ArduinoBusMaster::digitalWrite(byte pin, byte mode) {
	char writebuffer[] = { 0x03, pin, mode };
	SendRequest(writebuffer);

	if (Wire.available()) {
		char receivebuffer[3];
		Wire.readBytes(receivebuffer, 3);
		if (memcmp(writebuffer, receivebuffer, 3) == 0) return true;
	}
	if (this->SerialDebug) Serial.println("digitalWrite Response mismatch!");
	return false;

}

bool ArduinoBusMaster::digitalRead(byte pin) {
	char writebuffer[] = { 0x02, pin, 0x00 };
	SendRequest(writebuffer);

	if (Wire.available()) {
		char receivebuffer[3];
		Wire.readBytes(receivebuffer, 3);
		printbuf(receivebuffer, "Response");
		if (receivebuffer[0] != 0x02 && receivebuffer[1] != pin) return NULL;
		if (receivebuffer[2] == 0 || receivebuffer[2] == 1) return receivebuffer[2];
	}
	return NULL;
}

bool ArduinoBusMaster::analogWrite(byte pin, byte dutycycle) {
	char writebuffer[] = { 0x04, pin, dutycycle };
	SendRequest(writebuffer);

	if (Wire.available()) {
		char receivebuffer[3];
		Wire.readBytes(receivebuffer, 3);
		printbuf(receivebuffer, "Response");
		if (memcmp(writebuffer, receivebuffer, 3) == 0) return true;
	}
	return false;
}

void ArduinoBusMaster::SendRequest(char* writebuffer) {

	Wire.beginTransmission(this->address);
	Wire.write(byte(writebuffer[0]));
	Wire.write(byte(writebuffer[1]));
	Wire.write(byte(writebuffer[2]));
	Wire.endTransmission();

	Wire.requestFrom(this->address, 3);

	printbuf(writebuffer, "Request");
}

void ArduinoBusMaster::printbuf(char* buffer, String caption) {
	if (this->SerialDebug) {
		Serial.println(caption);
		Serial.print("Command: 0x");
		Serial.println(buffer[0], HEX);
		Serial.print("Pin: 0x");
		Serial.println(buffer[1], DEC);
		Serial.print("Data: 0x");
		Serial.println(buffer[2], HEX);
		Serial.println();
	}
}