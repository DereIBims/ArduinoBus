#ifndef _ARDUINOBUSMASTER_h
#define _ARDUINOBUSMASTER_h
#define Slave_INPUT 0x01
#define Slave_OUTPUT 0x02
#define Slave_INPUT_PULLUP 0x03

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif


class ArduinoBusMaster {
public:

	ArduinoBusMaster(bool SerialDebug);
	void begin(byte address);
	bool pinMode(byte pin, byte mode);
	bool digitalWrite(byte pin, byte mode);
	bool digitalRead(byte pin);
	bool analogWrite(byte pin, byte dutycycle);

private:
	void SendRequest(char* writebuffer);
	void printbuf(char* buffer, String caption);
	byte address;
	bool SerialDebug;
};


#endif

