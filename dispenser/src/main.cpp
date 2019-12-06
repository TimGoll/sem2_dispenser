#include "main.hpp"

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define SCREEN_ADDRESS 0x3C

Main::Main() {
	this->buttonHandler = new ButtonHandler();
	this->displayHandler = new DisplayHandler();

	this->buttonHandler->addCallback(10, RISING, this, &callbackTest, 1500, 250);

	displayHandler->init(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_ADDRESS);

	this->test = 10;
};

Main::~Main() {
	delete this->displayHandler;
}

bool Main::update() {
	this->displayHandler->update();
	this->buttonHandler->update();

	return true;
}

static void Main::callbackTest(Main* self, uint8_t type) {
	// since "self" reflects "this", all member functions/variables can be used
	// in a static context
	Serial.print("Pressed in Callback: ");
	Serial.print(type);
	Serial.print(", ");
	Serial.println(self->test);
}