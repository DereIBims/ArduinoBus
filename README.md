
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

