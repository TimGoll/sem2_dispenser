#ifndef MAIN_H
#define MAIN_H

#include <Arduino.h>

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <RTClib.h>

#include "display.hpp"
#include "buttons.hpp"
#include "menu.hpp"

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
};

#endif
