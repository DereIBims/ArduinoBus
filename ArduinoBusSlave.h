#ifndef _ArduinoBusSlave_H
#define _ArduinoBusSlave_H
#include "Arduino.h"

class ArduinoBusSlave {
public:
	ArduinoBusSlave(bool SerialDebug=false);
	void begin(byte address=0x01);

private:

};


#endif