#include "ArduinoBusSlave.h"

#include <Arduino.h>
#include <Wire.h>

bool SD;

byte writebuffer[3];

void HandleCommand(byte command, byte pin, byte data) {
	switch (command) {
	case 0xFF: //Command wrong size
		writebuffer[0] = 0xFF;
		writebuffer[1] = 0xFF;
		writebuffer[2] = 0xFF;
		break;


	case 0x01: //Set PinMode 
		writebuffer[0] = command;
		writebuffer[1] = pin;
		writebuffer[2] = data;
		switch (data) {
		case 0x01:
			pinMode(pin, INPUT);
			if (SD) {
				Serial.println("Action:");
				Serial.print("pinMode (");
				Serial.print(pin);
				Serial.println(", Input)");
				Serial.println();
			}
			break;
		case 0x02:
			pinMode(pin, OUTPUT);
			if (SD) {
				Serial.println("Action:");
				Serial.print("pinMode (");
				Serial.print(pin);
				Serial.println(", Output)");
				Serial.println();
			}
			break;
		case 0x03:
			pinMode(pin, INPUT_PULLUP);
			if (SD) {
				Serial.println("Action:");
				Serial.print("pinMode (");
				Serial.print(pin);
				Serial.println(", Input_Pullup)");
				Serial.println();
			}
			break;
		}
		break;

	case 0x02: //digialRead
		writebuffer[0] = command;
		writebuffer[1] = pin;
		writebuffer[2] = digitalRead(pin);
		if (SD) {
			Serial.println("Action:");
			Serial.print("digitalRead (");
			Serial.print(pin);
			Serial.println(")");
			Serial.println();
		}
		break;

	case 0x03: //digitalWrite
		writebuffer[0] = command;
		writebuffer[1] = pin;
		writebuffer[2] = data;
		digitalWrite(pin, data);
		if (SD) {
			Serial.println("Action:");
			Serial.print("digitalWrite(");
			Serial.print(pin, DEC);
			Serial.print(", ");
			Serial.print(data, BIN);
			Serial.println(")");
			Serial.println();
		}
		break;


	case 0x04: //analogWrite
		writebuffer[0] = command;
		writebuffer[1] = pin;
		writebuffer[2] = data;
		analogWrite(pin, data);
		if (SD) {
			Serial.println("Action:");
			Serial.print("analogWrite(");
			Serial.print(pin, DEC);
			Serial.print(", ");
			Serial.print(data, DEC);
			Serial.println(")");
			Serial.println();
		}
		break;
	}
}

void receiveData(int byteCount) {
	byte buffer[3];
	if (byteCount == 3) {
		Wire.readBytes(buffer, 3);

		if (SD) {
			Serial.println("Request:");
			Serial.print("Command: 0x");
			Serial.println(buffer[0], HEX);
			Serial.print("Pin: ");
			Serial.println(buffer[1], DEC);
			Serial.print("Data: 0x");
			Serial.println(buffer[2], HEX);
			Serial.println();
		}
	}
	else {
		if (SD) {
			buffer[0]=0xFF;
			Serial.println("Command error");
			Serial.println();
		}
	}
	HandleCommand(buffer[0], buffer[1], buffer[2]);
}

void sendData() {
	Wire.write(byte(writebuffer[0]));
	Wire.write(byte(writebuffer[1]));
	Wire.write(byte(writebuffer[2]));

	if (SD) {
		Serial.println("Answer:");
		Serial.print("Command: 0x");
		Serial.println(writebuffer[0], HEX);
		Serial.print("Pin: ");
		Serial.println(writebuffer[1], DEC);
		Serial.print("Data: 0x");
		Serial.println(writebuffer[2], HEX);
		Serial.println();
	}
}



ArduinoBusSlave::ArduinoBusSlave(bool SerialDebug) {
	SD = SerialDebug;
}

void ArduinoBusSlave::begin(byte address) {
	Wire.begin(address);
	Wire.onReceive(receiveData);
	Wire.onRequest(sendData);
	if (SD) {
		Serial.print("Slave ready");
		Serial.println();
	}
}





