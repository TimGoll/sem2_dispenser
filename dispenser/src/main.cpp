#include "main.hpp"

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define SCREEN_ADDRESS 0x3C

Main::Main() {
	// SETTING UP RTC
	RTC->begin();

	if (not RTC->isrunning()) {
		Serial.println("ERROR: RTC is not running!");

		// stay stuck if no RTC is available
		while (true);
	}

	// CREATING CLASS INSTANCES
	this->buttonHandler = new ButtonHandler();
	this->displayHandler = new DisplayHandler();
	this->menuHandler = new MenuHandler();
	this->pillHandler = new PillHandler();

	// READING DATA FROM EEPROM
	uint16_t eeprom_index = 32;

	this->pillHandler->readData(&eeprom_index);

	// SET UP DISPLAY
	this->displayHandler->init(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_ADDRESS);
	this->displayHandler->setMenu(
		this->menuHandler->menuOpenPtr(),
		this->menuHandler->menuIndexPtr(),
		this->menuHandler->menuOffsetPtr(),
		this->pillHandler
	);

	// GIVE MENU HANDLER ACCESS TO PILL DATA
	this->menuHandler->setObjectPointer(this->pillHandler);

	// REGISTERING MENU BUTTON CALLBACKS
	this->buttonHandler->addCallback(10, RISING, this->menuHandler, &MenuHandler::buttonPrev);
	this->buttonHandler->addCallback(11, RISING, this->menuHandler, &MenuHandler::buttonNext);
	this->buttonHandler->addCallback(12, RISING, this->menuHandler, &MenuHandler::buttonDown, 1000, 100);
	this->buttonHandler->addCallback(13, RISING, this->menuHandler, &MenuHandler::buttonUp, 1000, 100);

	this->buttonHandler->addCallback(52, RISING, this->pillHandler, &PillHandler::containerPlaced);

	// add a debug button to prepare a fake pill event
	this->buttonHandler->addCallback(53, RISING, this, &Main::prepareCustomPill);
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

static void Main::prepareCustomPill(Main* self, uint8_t type) {
	Serial.println("Inserting fake pill event");
	self->pillHandler->createFakeTrigger();
}
