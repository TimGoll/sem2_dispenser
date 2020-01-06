#ifndef EEPROM_HPP
#define EEPROM_HPP

#include <Arduino.h>
#include <EEPROM.h>

class EEpromWrapper {
	public:
		uint8_t readUInt8(uint16_t* address);
		uint16_t readUInt16(uint16_t* address);
		uint32_t readUInt32(uint16_t* address);
		void readString(uint16_t* address, uint8_t* string, uint8_t length);

		void writeUInt8(uint16_t* address, uint8_t num);
		void writeUInt16(uint16_t* address, uint16_t num);
		void writeUInt32(uint16_t* address, uint32_t num);
		void writeString(uint16_t* address, uint8_t* string, uint8_t length);

	private:
		uint8_t readInternal(uint16_t* address);
		void writeInternal(uint16_t* address, uint8_t num);
};

extern EEpromWrapper* EEPROMW;

#endif
