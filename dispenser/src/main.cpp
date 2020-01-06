#include "main.hpp"

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define SCREEN_ADDRESS 0x3C

Main::Main() {
	this->buttonHandler = new ButtonHandler();
	this->displayHandler = new DisplayHandler();
	this->menuHandler = new MenuHandler();

	uint16_t address = 0;

	EEPROMW->writeUInt32(&address, 87543);

	address = 0;

	Serial.println(EEPROMW->readUInt32(&address));

	// SET UP DISPLAY
	displayHandler->init(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_ADDRESS);
	displayHandler->setMenu(
		this->menuHandler->menuOpenPtr(),
		this->menuHandler->menuIndexPtr(),
		this->menuHandler->menuOffsetPtr()
	);

	// REGISTERING MENU BUTTON CALLBACKS
	this->buttonHandler->addCallback(10, RISING, this->menuHandler, &MenuHandler::buttonPrev);
	this->buttonHandler->addCallback(11, RISING, this->menuHandler, &MenuHandler::buttonNext);
	this->buttonHandler->addCallback(12, RISING, this->menuHandler, &MenuHandler::buttonDown, 1000, 100);
	this->buttonHandler->addCallback(13, RISING, this->menuHandler, &MenuHandler::buttonUp, 1000, 100);
};

Main::~Main() {
	delete this->buttonHandler;
	delete this->displayHandler;
	delete this->menuHandler;
}

bool Main::update() {
	this->displayHandler->update();
	this->buttonHandler->update();

	return true;
}
