#ifndef DATA_HPP
#define DATA_HPP

#include <Arduino.h> // Access to the basic arduino library to make our live easier

#define MONDAY 1
#define TUESDAY 2
#define WEDNESDAY 3
#define THURSDAY 4
#define FRIDAY 5
#define SATURDAY 6
#define SUNDAY 7

class DataHandler() {
	public:
	// Defaulconstructor
	DataHandler();
	// Destructor
	~DataHandler();

	bool_t read_from_eeprom();

	bool_t write_to_eeprom();

	private:
	DateTime* dateTime;
}

#endif
