#ifndef MAIN_DISPLAY_H
#define MAIN_DISPLAY_H

#include <Arduino.h> // Access to the basic arduino library to make our live easier
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define DRAWTIME 100 //ms

class DisplayHandler {
	public:
	// Defaulconstructor
	DisplayHandler();
	// Destructor
	~DisplayHandler();

	// The init function creates all the needed objects
	void init(uint8_t width, uint8_t height, uint8_t address);

	// The update function is called in the main loop and handles the timing of the display
	void update();

	private:
	// textprint helper
	void draw_text(Adafruit_SSD1306* display, uint8_t* text);
	void draw_text(Adafruit_SSD1306* display, uint8_t* text, uint8_t size, uint8_t posX, uint8_t posY);

	// internal variables
	Adafruit_SSD1306* display;
	uint32_t last_draw;
};
#endif
