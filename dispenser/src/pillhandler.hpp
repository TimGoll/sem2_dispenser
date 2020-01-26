#ifndef PILLHANDLER_HPP
#define PILLHANDLER_HPP

#include <Arduino.h>
#include <RTClib.h>
#include <Servo.h>

#include "eeprom.hpp"

#define MAX_AMOUNT_PILLSTACKS 2

#define POS_PILL_OUT 120
#define POS_PILL_IN 20

#define THROW_TIME 1000

class PillStack {
	public:
		PillStack(uint8_t pin);
		~PillStack();

		void setPillAmount(uint8_t amount);

		void setPillRefTime(uint32_t time);

		void setPillInterval(uint32_t time);

		uint8_t getPillAmount();

		uint32_t getPillRefTime();

		uint32_t getPillInterval();

		bool isPillReady();

		void update();

		bool isPillReadyFirstTime();

		void setContainerState(bool state);

		void throwPill();
	private:
		uint8_t pill_amount;
		uint32_t pill_ref_time;
		uint32_t pill_interval;

		uint32_t retraction_time;

		Servo *servo;
};

class PillHandler {
	public:
		PillHandler();
		~PillHandler();

		void readData(uint16_t* start_index);

		void update();

		static void containerPlaced(PillHandler* self, uint8_t type);

	private:
		uint8_t data_index;

		bool container_placed;

		PillStack* pillStack[MAX_AMOUNT_PILLSTACKS];
};

#endif
