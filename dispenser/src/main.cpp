#include "main.hpp"

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define SCREEN_ADDRESS 0x3C

Main::Main() {
	// CREATING CLASS INSTANCES
	this->buttonHandler = new ButtonHandler();
	this->displayHandler = new DisplayHandler();
	this->menuHandler = new MenuHandler();
	this->pillHandler = new PillHandler();

	// READING DATA FROM EEPROM
	uint16_t eeprom_index = 0;

	this->pillHandler->readData(&eeprom_index);

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
	delete this->pillHandler;
}

bool Main::update() {
	this->displayHandler->update();
	this->buttonHandler->update();
	this->pillHandler->update();

	return true;
}
