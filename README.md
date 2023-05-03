
# A simple Library to use an Arduino as port Extender

With this library you can use one or more Arduinos as port extender for another Arduino.
Right now, only digitalPins are working.

The library uses standard calls like pinMode, digitalWrite, digitalRead,...

## Info

Please consider that due to performance limitations, the Serial baud rate must be set to at least 115200.
If you use slower rates, the SerialDebug output will use so much time, that the I2C comminication malfunctiones.


## Usage/Examples (comments will be added to the code in /examples)
### Slave

```cpp
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
```

### Master

```cpp
#include "ArduinoBusMaster.h"
ArduinoBusMaster TestSlave(true);
ArduinoBusMaster Test2(true);

bool check;
void setup() {
	Serial.begin(115200);
	TestSlave.begin(0x05);
	Test2.begin(0x01);
	check=TestSlave.pinMode(13, Slave_OUTPUT);
}

// the loop function runs over and over again until power down or reset
void loop() {
	while (check) {
		check=TestSlave.digitalWrite(13, HIGH);
		if (!check) break;
		delay(50);
		check=TestSlave.digitalWrite(13, LOW);
		delay(500);
	}
}
```