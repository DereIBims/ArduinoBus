#include "ArduinoBusMaster.h"
ArduinoBusMaster TestSlave(true);
ArduinoBusMaster Test2(true);

bool checkmode;
void setup() {
	Serial.begin(115200);
	TestSlave.begin(0x05);
	Test2.begin(0x01);
	checkmode=TestSlave.pinMode(13, Slave_OUTPUT);
}

// the loop function runs over and over again until power down or reset
void loop() {
	while (checkmode) {
		checkmode=TestSlave.digitalWrite(13, HIGH);
		if (!checkmode) break;
		delay(50);
		checkmode=TestSlave.digitalWrite(13, LOW);
		delay(500);
	}
}
