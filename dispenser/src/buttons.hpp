#ifndef BUTTONS_H
#define BUTTONS_H

#include <Arduino.h>

#include "callback.hpp"

#define MAX_AMOUNT_BUTTONS 8

class Button : public Callback {
	public:
		// Defaultconstructor
		Button(uint8_t pin_number, uint8_t type, uint16_t start_time, uint16_t interval_time);
		// Destructor
		~Button();

		/**************************************************************************/
		/*!
			@brief	A convenience function that handles the debouncing
			@param return It returns the button value, be it RISING/FALLING for edges
			or HIGH/LOW for constant values
		*/
		/**************************************************************************/
		uint8_t state();

		/**************************************************************************/
		/*!
			@brief	Gives the type of the callback event
			@param return Returns the type: RISING, FALLING, CHANGE
		*/
		/**************************************************************************/
		uint8_t getType();

		/**************************************************************************/
		/*!
			@brief	Checks if a passed type is the defined callback type, also
					handles the CHANGE callback type
			@param type The type to be checked against
			@param return Returns if the passed type is the defined callback type
		*/
		/**************************************************************************/
		bool isType(uint8_t type);

		/**************************************************************************/
		/*!
			@brief	Returns the start time
			@param return The start time
		*/
		/**************************************************************************/
		uint16_t getStartTime();

		/**************************************************************************/
		/*!
			@brief	Returns the interval time
			@param return The interval time
		*/
		/**************************************************************************/
		uint16_t getIntervalTime();

		/**************************************************************************/
		/*!
			@brief	Checks if an interval callback is set; if it is set, it checks
					wether the timing is correct
			@param return Returns if the next interval callback should be executed
		*/
		/**************************************************************************/
		bool isNextInterval();

		/**************************************************************************/
		/*!
			@brief	Executes the next interval callback while also handling internal
					variables that have to be set prior to the callback call
			@param type The type to be passed
		*/
		/**************************************************************************/
		void doNextInterval(uint8_t type);

	private:
		uint8_t pin_number;
		uint8_t type;
		uint16_t start_time;
		uint16_t interval_time;

		uint8_t last_state;
		uint32_t last_time;
		uint32_t last_interval_time;
};

class ButtonHandler {
	public:
		// Defaultconstructor
		ButtonHandler();
		// Destructor
		~ButtonHandler();

		/**************************************************************************/
		/*!
			@brief	Registers a callback that is called once a button is pressed
					while also handling all button specific stuff (pinMode, ...)
			@param pin_number The pin number of the button to react to
			@param type The type of the callback checks (RISING, FALLING, CHANGE)
			@param callback_scope The callback scope to be passed with the function
			@param callback The callback function to be called
			@param return Returns if registration was successfull
		*/
		/**************************************************************************/
		bool addCallback(uint8_t pin_number, uint8_t type, void* callback_scope, callback_t callback);

		/**************************************************************************/
		/*!
			@brief	Registers a callback that is called once a button is pressed
					while also handling all button specific stuff (pinMode, ...)
					This also features a polling mode, after start_time has passed,
					the callback function can be automatically called everytime
					interval_time has passed as long as the button is still pressed
			@param pin_number The pin number of the button to react to
			@param type The type of the callback checks (RISING, FALLING, CHANGE)
			@param callback_scope The callback scope to be passed with the function
			@param callback The callback function to be called
			@param start_time If != 0, The callback can be polled after start_time
			@param interval_time The polling interval
			@param return Returns if registration was successfull
		*/
		/**************************************************************************/
		bool addCallback(uint8_t pin_number, uint8_t type, void* callback_scope, callback_t callback, uint16_t start_time, uint16_t interval_time);

		/**************************************************************************/
		/*!
			@brief	The update function has to becalled called in the main loop and
					handles the readings of the registered buttons
		*/
		/**************************************************************************/
		void update();

	private:
		Button* button_list[MAX_AMOUNT_BUTTONS];
		uint8_t button_amount;
};

#endif
