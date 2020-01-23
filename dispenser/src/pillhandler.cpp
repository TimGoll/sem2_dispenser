#include "pillhandler.hpp"

PillStack::PillStack() {

}

PillStack::~PillStack() {

}

void PillStack::setPillAmount(uint8_t amount) {

}

void PillStack::setPillRefTime(uint32_t time) {

}

void PillStack::setPillInterval(uint32_t time) {

}

uint8_t PillStack::getPillAmount() {

}

uint32_t PillStack::getPillRefTime() {

}

uint32_t PillStack::getPillInterval() {

}

PillHandler::PillHandler() {
	this->data_index = 0;
}

PillHandler::~PillHandler() {
	this->pillStack[0] = new PillStack();
	this->pillStack[1] = new PillStack();
}

void PillHandler::readData(uint16_t* index) {
	// cache storage position for later data update
	this->data_index = *index;

	// PILL HOLDER A
	this->pillStack[0]->setPillAmount(EEPROMW->readUInt8(index));

	this->pillStack[0]->setRefTime(EEPROMW->readUInt32(index));

	this->pillStack[0]->setPillInterval(EEPROMW->readUInt32(index));

	// PILL HOLDER B
	this->pillStack[1]->setPillAmount(EEPROMW->readUInt8(index));

	this->pillStack[1]->setRefTime(EEPROMW->readUInt32(index));

	this->pillStack[1]->setPillInterval(EEPROMW->readUInt32(index));
}

void PillHandler::update() {
	
}
