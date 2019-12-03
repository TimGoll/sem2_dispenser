#include "src/display.hpp"

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define SCREEN_ADDRESS 0x3C

DisplayHandler* displayHandler = new DisplayHandler();

void setup() {
	displayHandler->init(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_ADDRESS);
}

void loop() {
	displayHandler->update();
}