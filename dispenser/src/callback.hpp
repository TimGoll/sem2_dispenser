#ifndef CALLBACK_H
#define CALLBACK_H

#include <Arduino.h>

//defining a type for the callback function
typedef void (*callback_t)(void*, uint8_t);

class Callback {
	public:
		Callback();
		~Callback();

		/**************************************************************************/
		/*!
			@brief	Registers a callback function to be called at a later point
			@param scope The scope which should be passed on to the registered function
			@param callback The function to be called
		*/
		/**************************************************************************/
		void registerCallback(void* scope, callback_t callback);

		/**************************************************************************/
		/*!
			@brief	Runs the passed callback function with an uint8_t as a parameter
			@param value The value to be passed
		*/
		/**************************************************************************/
		void runCallback(uint8_t value);

	private:
		callback_t callback_function;
		void* callback_scope;
};

#endif
