#include "pillhandler.hpp"

PillStack::PillStack(uint8_t pin) {
	this->servo = new Servo();
	this->servo->attach(pin);

	this->retraction_time = 0;
}

PillStack::~PillStack() {

}

void PillStack::setPillAmount(uint8_t amount) {
	this->pill_amount = amount;
}

void PillStack::setPillRefTime(uint32_t time) {
	this->pill_ref_time = time;
}

void PillStack::setPillInterval(uint32_t time) {
	this->pill_interval = time;
}

uint8_t PillStack::getPillAmount() {
	return this->pill_amount;
}

uint32_t PillStack::getPillRefTime() {
	return this->pill_ref_time;
}

uint32_t PillStack::getPillInterval() {
	return this->pill_interval;
}

bool PillStack::isPillReady() {
	return true; //TODO
}

void PillStack::update() {
	if (millis() > this->retraction_time) {
		this->servo->write(POS_PILL_IN);
	} else {
		this->servo->write(POS_PILL_OUT);
	}
}

void PillStack::throwPill() {
	// set throw posittion time flag
	this->retraction_time = millis() + THROW_TIME

	// set new time for next pill
	this->pill_ref_time += this->pill_interval;

	// decrease pill amount
	this->pill_amount -= 1;
}

PillHandler::PillHandler() {
	this->data_index = 0;

	this->pillStack[0] = new PillStack(2);
	this->pillStack[1] = new PillStack(3);

	this->container_placed = false;
}

PillHandler::~PillHandler() {
	
}

void PillHandler::readData(uint16_t* index) {
	// cache storage position for later data update
	this->data_index = *index;

	// PILL HOLDER A
	this->pillStack[0]->setPillAmount(EEPROMW->readUInt8(index));

	this->pillStack[0]->setPillRefTime(EEPROMW->readUInt32(index));

	this->pillStack[0]->setPillInterval(EEPROMW->readUInt32(index));

	// PILL HOLDER B
	this->pillStack[1]->setPillAmount(EEPROMW->readUInt8(index));

	this->pillStack[1]->setPillRefTime(EEPROMW->readUInt32(index));

	this->pillStack[1]->setPillInterval(EEPROMW->readUInt32(index));
}

void PillHandler::update() {
	this->pillStack[0]->update();
}

static void PillHandler::containerPlaced(PillHandler* self, uint8_t type) {
	for (uint8_t i = 0; i < 2; i++) {
		if (self->pillStack[i]->isPillReady()) {
			self->pillStack[i]->throwPill();

			// stop after one pill is thrown, never throw two!
			break;
		}
	}
}
