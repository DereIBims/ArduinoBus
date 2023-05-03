#include <ArduinoBusSlave.h>

ArduinoBusSlave Test(true);

void setup() {
	Serial.begin(115200);
	Serial.println("Init Wire");
	Test.begin(0x05);
}

// the loop function runs over and over again until power down or reset
void loop() {

}