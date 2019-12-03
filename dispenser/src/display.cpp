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

void DisplayHandler::update() {
	// make sure to update display only when needed on not on every loop cycle
	if (millis() - this->last_draw <= DRAWTIME)
		return;

	// set last time to now
	this->last_draw = millis();

	this->display->clearDisplay();
	this->draw_text(
		this->display,
		"Hello World!\nThis is a new line\nTHIRD!",
		/*size:*/1, /*x:*/0, /*y:*/0
	);

	// update display
	this->display->display();
}

void DisplayHandler::draw_text(Adafruit_SSD1306* display, uint8_t* text) {
	this->draw_text(display, text, 1, 0, 0);
}

void DisplayHandler::draw_text(Adafruit_SSD1306* display, uint8_t* text, uint8_t size, uint8_t posX, uint8_t posY) {
	display->setTextColor(SSD1306_WHITE);
	display->setTextSize(size);
	display->setCursor(posX, posY);
	display->cp437(true);

	uint8_t line_num = 1;

	for (uint8_t i = 0; text[i] != '\0'; i++) {
		if (text[i] == '\n') {
			display->setCursor(posX, posY + size * 8 * line_num);
			line_num++;
		} else {
			display->write(text[i]);
		}
	}
}