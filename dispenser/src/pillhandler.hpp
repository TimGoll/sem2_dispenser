#ifndef PILLHANDLER_HPP
#define PILLHANDLER_HPP

#include <Arduino.h>
#include <RTClib.h>

#include "eeprom.hpp"

#define MAX_AMOUNT_PILLSTACKS 2

class PillStack {
	public:
		PillStack();
		~PillStack();

		void setPillAmount(uint8_t amount);

		void setPillRefTime(uint32_t time);

		void setPillInterval(uint32_t time);

		uint8_t getPillAmount();

		uint32_t getPillRefTime();

		uint32_t getPillInterval();
	private:
	
};

class PillHandler {
	public:
		PillHandler();
		~PillHandler();

		uint16_t readData(uint16_t start_index);

	private:
		uint8_t* data_prefix;

		PillStack* pillStack[MAX_AMOUNT_PILLSTACKS];
};

#endif
