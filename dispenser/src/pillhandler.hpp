#ifndef PILLHANDLER_HPP
#define PILLHANDLER_HPP

#include <Arduino.h>
#include <RTClib.h>
#include <Servo.h>

#include "eeprom.hpp"
#include "rtc.hpp"

#define MAX_AMOUNT_PILLSTACKS 2

#define POS_PILL_OUT 120
#define POS_PILL_IN 20

#define THROW_TIME 1000

class PillStack {
	public:
		PillStack(uint8_t pin);
		~PillStack();

		void setPillAmountIndex(uint16_t index);

		void setPillRefTimeIndex(uint16_t index);

		void setPillIntervalIndex(uint16_t index);

		void setPillAmount(uint8_t amount);

		void setPillRefTime(uint32_t time);

		void setPillInterval(uint32_t time);

		uint8_t getPillAmount();

		uint32_t getPillRefTime();

		uint32_t getPillInterval();

		bool isPillReady();

		void update();

		void setContainerState(bool state);

		void throwPill();

		void createFakeTrigger();
	private:
		uint8_t pill_amount;
		uint32_t pill_ref_time;
		uint32_t pill_interval;

		uint16_t i_pill_amount;
		uint16_t i_pill_ref_time;
		uint16_t i_pill_interval;

		uint32_t retraction_time;

		uint8_t pin;

		bool pill_ready;

		Servo *servo;
};

class PillHandler {
	public:
		PillHandler();
		~PillHandler();

		void createFakeTrigger();

		void readData(uint16_t* start_index);

		void update();

		static void containerPlaced(PillHandler* self, uint8_t type);

		uint8_t getPillAmount(uint8_t id);

		uint32_t getPillRefTime(uint8_t id);

		uint32_t getPillInterval(uint8_t id);

	private:
		uint8_t data_index;

		bool container_placed;

		PillStack* pillStack[MAX_AMOUNT_PILLSTACKS];
};

#endif
