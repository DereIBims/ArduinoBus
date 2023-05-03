#include "ArduinoBusMaster.h"
#include "Arduino.h"
#include <Wire.h>

bool initialized = false;

ArduinoBusMaster::ArduinoBusMaster(bool SerialDebug) {
	this->SerialDebug = SerialDebug;
}

void ArduinoBusMaster::begin(byte address) {
	this->address = address;
	if (!initialized) {
		Wire.begin();
		if (this->SerialDebug) {
			Serial.println("Call:");
			Serial.println("Wire.begin()");
			Serial.print("for address 0x");
			Serial.println(this->address);
			Serial.println();
		}
		initialized = true;
	}
	else {
		Serial.println("Function: ");
		Serial.println("Wire.begin()");
		Serial.println("was already called!");
		Serial.println();
	}
	this->reachable = CheckAddress();
}

bool ArduinoBusMaster::pinMode(byte pin, byte mode) {
	if (!this->reachable) return false;
	char writebuffer[] = { 0x01, pin, mode };
	SendRequest(writebuffer);
	if (Wire.available()) {
		char receivebuffer[3];
		Wire.readBytes(receivebuffer, 3);
		printbuf(receivebuffer, "Response");
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
	if (!this->reachable) return false;
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
	if (!this->reachable) return false;
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
	if (!this->reachable) return false;
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

void ArduinoBusMaster::printbuf(char* buffer, const char* caption) {
	if (this->SerialDebug) {
		Serial.print(caption);
		if (caption == "Request") {
			Serial.print(" for address: 0x");
		}
		else {
			Serial.print(" from address: 0x");
		}
		Serial.println(this->address);
		Serial.print("Command: 0x");
		Serial.println(buffer[0], HEX);
		Serial.print("Pin: 0x");
		Serial.println(buffer[1], DEC);
		Serial.print("Data: 0x");
		Serial.println(buffer[2], HEX);
		Serial.println();
	}
}

bool ArduinoBusMaster::CheckAddress() {
	Wire.beginTransmission(this->address);
	if (Wire.endTransmission() == 0) {
		if (this->SerialDebug) {
			Serial.print("Adress: 0x");
			Serial.println(this->address);
			Serial.println("is rechable -> all okay");
			Serial.println();
		}
		return true;
	}
	else {
		if (this->SerialDebug) {
			Serial.print("Adress: 0x");
			Serial.println(this->address);
			Serial.println("is not reachable!");
			Serial.println("Functions for this address are ignored!");
			Serial.println();
		}
		return false;
	}
}