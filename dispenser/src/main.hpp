#ifndef MAIN_HPP
#define MAIN_HPP

#include <Arduino.h>

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <RTClib.h>

#include "display.hpp"
#include "buttons.hpp"
#include "menu.hpp"
#include "pillhandler.hpp"
#include "eeprom.hpp"
#include "rtc.hpp"

class Main {
	public:
		// Defaultconstructor
		Main();
		// Destructor
		~Main();

		/**************************************************************************/
		/*!
			@brief	The update function has to becalled called in the main loop and
					handles the reoccuring updates of the main class
			@param return Stops beeing called / ends program when returned false
		*/
		/**************************************************************************/
		bool update();

	private:
		DisplayHandler* displayHandler;
		ButtonHandler* buttonHandler;
		MenuHandler* menuHandler;
		PillHandler* pillHandler;
};

#endif
