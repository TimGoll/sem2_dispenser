#include "src/main.hpp"

void setup() {
	Serial.begin(115200);

	Serial.println("Setting up main class ...");

	Main* mainClass = new Main();

	Serial.println("Done.");
	Serial.println("Software is now running.");

	bool alive = true;
	while (alive) {
		alive = mainClass->update();
	}

	Serial.println("Quitting Software.");

	delete mainClass;

	// stay stuck in here forever
	while(true);
}
