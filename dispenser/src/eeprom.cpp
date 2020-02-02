#include "eeprom.hpp"

uint8_t EEpromWrapper::readUInt8(uint16_t* address) {
	uint8_t value = 0;

	EEPROM.get(*address, value);

	*address += sizeof(uint8_t);

	return value;
}

uint16_t EEpromWrapper::readUInt16(uint16_t* address) {
	uint16_t value = 0;

	EEPROM.get(*address, value);

	*address += sizeof(uint16_t);

	return value;
}

uint32_t EEpromWrapper::readUInt32(uint16_t* address) {
	uint32_t value = 0;

	EEPROM.get(*address, value);

	*address += sizeof(uint32_t);

	return value;
}

void EEpromWrapper::readString(uint16_t* address, uint8_t* string, uint8_t length) {
	
}

void EEpromWrapper::writeUInt8(uint16_t* address, uint8_t num) {
	EEPROM.put(*address, num);

	*address += sizeof(uint8_t);
}

void EEpromWrapper::writeUInt16(uint16_t* address, uint16_t num) {
	EEPROM.put(*address, num);

	*address += sizeof(uint16_t);
}

void EEpromWrapper::writeUInt32(uint16_t* address, uint32_t num) {
	EEPROM.put(*address, num);

	*address += sizeof(uint32_t);
}

void EEpromWrapper::writeString(uint16_t* address, uint8_t* string, uint8_t length) {

}

//////////////

uint8_t EEpromWrapper::readInternal(uint16_t* address) {
	if (*address == EEPROM.length()) {
		Serial.println("ERROR: Reached EEPROM END!");

		return 0;
	}

	return EEPROM.read((*address)++);
}

void EEpromWrapper::writeInternal(uint16_t* address, uint8_t num) {
	if (*address == EEPROM.length()) {
		Serial.println("ERROR: Reached EEPROM END!");

		return;
	}

	// we're using here `update()` since eeproms have a final amount of
	// write cycles. `update` only writes to the eeprom, if the given
	// value is different than the already existing value
	return EEPROM.update((*address)++, num);
}

EEpromWrapper* EEPROMW = new EEpromWrapper();
