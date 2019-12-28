#ifndef DATAHUB_HPP
#define DATAHUB_HPP

#include <Arduino.h>
#include <SPI.h>
#include <SD.h>

#include "lib/callback/callback.hpp"

// change this to match your SD shield or module;
// Arduino Ethernet shield: pin 4
// Adafruit SD shields and modules: pin 10
// Sparkfun SD shield: pin 8
// MKRZero SD: SDCARD_SS_PIN
#define CHIP_SELECT 4

class DataPoints {
	public:
		DataPoints(uint8_t max_data_points);
		~DataPoints();

		void getPrefix(uint8_t* prefix);

		void setPrefix(uint8_t* prefix);

		bool getNextDataPoint(uint8_t* type, uint8_t* data);

		void setNextDataPoint(uint8_t* type, uint8_t* data);

		void getDataLine(uint8_t* line);

		void setDataLine(uint8_t* line);

	private:
		uint8_t string_pos;
		uint8_t line_size;

		uint8_t* data_line;

		uint8_t* prefix;
};

class DataHub : Callback {
	public:
		DataHub();

		~DataHub();

		void readFromSD();

		void writeToSD();

	private:
		Sd2Card* card;
		SdVolume* volume;
		SdFile* file_root;
};

#endif
