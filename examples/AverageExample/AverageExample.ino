#include <Average.h>

// Reserve space for 10 entries in the average bucket.
// Change the type between < and > to change the entire way the library works.
Average<float> ave(10);

void setup() {
	Serial.begin(9600);
}

void loop() {
	// Add a new random value to the bucket
	ave.push(rand());

	// Display the current data set
	for (int i = 0; i < 10; i++) {
		Serial.print(ave.get(i));
		Serial.print(" ");
	}
	Serial.println();

	// And show some interesting results.
	Serial.print("Mean:   "); Serial.println(ave.mean());
	Serial.print("Mode:   "); Serial.println(ave.mode());
	Serial.print("Max:    "); Serial.println(ave.maximum());
	Serial.print("Min:    "); Serial.println(ave.minimum());
	Serial.print("StdDev: "); Serial.println(ave.stddev());
	delay(1000);
}
