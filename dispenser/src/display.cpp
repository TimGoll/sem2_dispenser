#include "display.hpp"

DisplayHandler::DisplayHandler() {
	this->last_draw = 0;
}

DisplayHandler::~DisplayHandler() {

}

void DisplayHandler::init(uint8_t width, uint8_t height, uint8_t address) {
	//create a new instance of the OLED library and save it
	this->display = new Adafruit_SSD1306(width, height, &Wire, -1);

	// now initialize the display
	this->display->begin(SSD1306_SWITCHCAPVCC, address);

	// after the class is created, the connection should be initialized
	this->display->clearDisplay();
}

void DisplayHandler::setMenu(MenuElement** menu_open, uint8_t* menu_index, uint8_t* menu_offset) {
	this->menu_open = menu_open;
	this->menu_index = menu_index;
	this->menu_offset = menu_offset;
}

void DisplayHandler::update() {
	// make sure to update display only when needed on not on every loop cycle
	if (millis() - this->last_draw <= DRAWTIME)
		return;

	// get current time
	DateTime time = RTC->now();

	this->display->clearDisplay();

	// draw common graphics
	this->drawText(this->display, DAYS[time.dayOfTheWeek()]);

	char buffer[21];
	if (time.unixtime() % 2 >= 1)
		sprintf(buffer, "%02d:%02d", time.hour(), time.minute());
	else
		sprintf(buffer, "%02d %02d", time.hour(), time.minute());
	
	this->drawText(this->display, buffer, 1, 98, 0);

	this->display->drawFastHLine(0, 9, 128, SSD1306_WHITE);

	if ((*this->menu_open)->isTopLevel())
		this->drawMainScreen(13);
	else
		this->drawMenuScreen(13);

	// update display
	this->display->display();

	// set last time to now
	this->last_draw = millis();
}

void DisplayHandler::drawMainScreen(uint8_t start_y) {
	this->drawText(
		this->display,
		"Hello World!\nThis is a new line!",
		/*size:*/1, /*x:*/0, /*y:*/start_y
	);
}

void DisplayHandler::drawMenuScreen(uint8_t start_y) {
	for (uint8_t i = *this->menu_offset; i < min(MENU_DISPLAY_LINES + *this->menu_offset, (*this->menu_open)->getChildAmount()); i++) {
		this->drawText(this->display, (*this->menu_open)->getChildAtIndex(i)->getName(), 1, 0, start_y + (i - *this->menu_offset) * 8);
	}
}

void DisplayHandler::drawText(Adafruit_SSD1306* display, uint8_t* text) {
	this->drawText(display, text, 1, 0, 0);
}

void DisplayHandler::drawText(Adafruit_SSD1306* display, uint8_t* text, uint8_t size, uint8_t posX, uint8_t posY) {
	display->setTextColor(SSD1306_WHITE);
	display->setTextSize(size);
	display->setCursor(posX, posY);
	display->cp437(true);

	for (uint8_t i = 0; text[i] != '\0'; i++) {
		if (text[i] == '\n')
			display->setCursor(posX, display->getCursorY() + size * 8);
		else
			display->write(text[i]);
	}
}
