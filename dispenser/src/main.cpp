#include "main.hpp"

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define SCREEN_ADDRESS 0x3C

Main::Main() {
	this->buttonHandler = new ButtonHandler();
	this->displayHandler = new DisplayHandler();
	this->menuHandler = new MenuHandler();

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

	this->dataPoints = new DataPoints(8);
	this->dataPoints->setDataLine((uint8_t*)"ABCDMESdata09#ABC078678#DEFhello#");
	//this->dataPoints->setNextDataPoint((uint8_t*) "MES", (uint8_t*) "mydata");
	//this->dataPoints->setNextDataPoint((uint8_t*) "ABC", (uint8_t*) "rand09");
	//this->dataPoints->setNextDataPoint((uint8_t*) "DEF", (uint8_t*) "18.03");

	//this->dataPoints->setPrefix((uint8_t*) "PREF");

	uint8_t prefix[5];
	this->dataPoints->getPrefix(prefix);

	Serial.println((char*) prefix);

	for (uint8_t i = 0; i < 5; i++) {
		uint8_t type[4];
		uint8_t data[8];

		bool valid_data = this->dataPoints->getNextDataPoint(type, data);

		if (not valid_data)
			break;

		Serial.println((char*)type);
		Serial.println((char*)data);
	}
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
