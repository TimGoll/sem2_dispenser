#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#include <Arduino.h> // Access to the basic arduino library to make our live easier
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "pillhandler.hpp"
#include "menu.hpp"
#include "rtc.hpp"

#define DRAWTIME 100 //ms

class DisplayHandler {
	public:
		// Defaulconstructor
		DisplayHandler();
		// Destructor
		~DisplayHandler();

		/**************************************************************************/
		/*!
			@brief	Sets up object and creates all needed subobjects
			@param width The width of the display in pixels
			@param height The height of the display in pixels
			@param address The i2c address of the display
		*/
		/**************************************************************************/
		void init(uint8_t width, uint8_t height, uint8_t address);

		void setMenu(MenuElement** menu_open, uint8_t* menu_index, uint8_t* menu_offset, PillHandler* pill_handler);

		/**************************************************************************/
		/*!
			@brief	The update function has to becalled called in the main loop and
					handles the timing of the display
		*/
		/**************************************************************************/
		void update();

	private:
		// internal textprint helper
		void drawText(Adafruit_SSD1306* display, uint8_t* text);
		void drawText(Adafruit_SSD1306* display, uint8_t* text, uint8_t size, uint8_t posX, uint8_t posY);

		void drawMainScreen(uint8_t start_y);
		void drawMenuScreen(uint8_t start_y);

		// internal variables
		Adafruit_SSD1306* display;
		uint32_t last_draw;

		// menu references
		MenuElement** menu_open;
		uint8_t* menu_index;
		uint8_t* menu_offset;
		PillHandler* pill_handler;
};

#endif
