#include "eeprom.hpp"

uint8_t EEpromWrapper::readUInt8(uint16_t* address) {
	return this->readInternal(address);
}

uint16_t EEpromWrapper::readUInt16(uint16_t* address) {
	return (uint16_t) (
		((uint16_t) this->readInternal(address) << 8) |
		((uint16_t) this->readInternal(address))
	);
}

uint32_t EEpromWrapper::readUInt32(uint16_t* address) {
	return (uint32_t) (
		((uint32_t) this->readInternal(address) << 24) |
		((uint32_t) this->readInternal(address) << 16) |
		((uint32_t) this->readInternal(address) << 8) |
		((uint32_t) this->readInternal(address))
	);
}

void EEpromWrapper::readString(uint16_t* address, uint8_t* string, uint8_t length) {
	
}

void EEpromWrapper::writeUInt8(uint16_t* address, uint8_t num) {
	this->writeInternal(address, num);
}

void EEpromWrapper::writeUInt16(uint16_t* address, uint16_t num) {
	uint8_t num_1 = (uint8_t) (num >> 8);
	uint8_t num_2 = (uint8_t) (num);

	this->writeInternal(address, num_1);
	this->writeInternal(address, num_2);
}

void EEpromWrapper::writeUInt32(uint16_t* address, uint32_t num) {
	uint8_t num_1 = (uint8_t) (num >> 24);
	uint8_t num_2 = (uint8_t) (num >> 16);
	uint8_t num_3 = (uint8_t) (num >> 8);
	uint8_t num_4 = (uint8_t) (num);

	this->writeInternal(address, num_1);
	this->writeInternal(address, num_2);
	this->writeInternal(address, num_3);
	this->writeInternal(address, num_4);
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
