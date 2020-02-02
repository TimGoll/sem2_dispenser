#include "pillhandler.hpp"

PillStack::PillStack(uint8_t pin) {
	this->pin = pin;

	this->servo = new Servo();
	this->servo->attach(pin);

	this->retraction_time = 0;

	this->pill_ready = false;
}

PillStack::~PillStack() {

}

void PillStack::setPillAmountIndex(uint16_t index) {
	this->i_pill_amount = index;
}

void PillStack::setPillRefTimeIndex(uint16_t index) {
	this->i_pill_ref_time = index;
}

void PillStack::setPillIntervalIndex(uint16_t index) {
	this->i_pill_interval = index;
}

void PillStack::setPillAmount(uint8_t amount) {
	this->pill_amount = amount;
}

void PillStack::setPillRefTime(uint32_t time) {
	this->pill_ref_time = time;

	//Serial.println(this->i_pill_ref_time);

	//DateTime time2 = RTC->now();

	//uint16_t i_pill_ref_time_ = this->i_pill_ref_time;

	//EEPROMW->writeUInt32(&i_pill_ref_time_, time2.unixtime());

	//uint16_t i_pill_ref_time_2 = this->i_pill_ref_time;

	//Serial.print(i_pill_ref_time_2);
	//Serial.print(" - reftime: ");
	//Serial.println((uint32_t) EEPROMW->readUInt32(i_pill_ref_time_2));
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
	return pill_ready;
}

void PillStack::update() {
	// get current time
	DateTime time = RTC->now();

	if (not this->pill_ready and time.unixtime() >= this->pill_ref_time) {
		this->pill_ready = true;

		// start beeper
		tone(8, 1000, 750);
		Serial.println("pill ready!");
	}

	if (millis() > this->retraction_time) {
		this->servo->write(POS_PILL_IN);
	} else {
		this->servo->write(POS_PILL_OUT);
	}
}

void PillStack::throwPill() {
	// set throw position time flag
	this->retraction_time = millis() + THROW_TIME;

	// set new time for next pill
	this->pill_ref_time += this->pill_interval;

	// decrease pill amount
	this->pill_amount -= 1;

	// pill is no longer ready
	this->pill_ready = false;

	// update EEPROM storage

	// we have to cache the index variables here since writeUint modifies it
	uint16_t i_pill_amount_ = this->i_pill_amount;
	uint16_t i_pill_ref_time_ = this->i_pill_ref_time;

	EEPROMW->writeUInt8(&i_pill_amount_, this->pill_amount);
	EEPROMW->writeUInt32(&i_pill_ref_time_, this->pill_ref_time);
}

void PillStack::createFakeTrigger() {
	Serial.print(this->pin);
	Serial.println(" - Creating fake trigger.");

	// get current time
	DateTime time = RTC->now();

	// create a fake pill ready event in 15 seconds
	this->pill_ref_time = time.unixtime() + 15;

	// update EEPROM storage

	// we have to cache the index variables here since writeUint modifies it
	uint16_t i_pill_ref_time_ = this->i_pill_ref_time;

	EEPROMW->writeUInt32(&i_pill_ref_time_, this->pill_amount);
}

PillHandler::PillHandler() {
	this->data_index = 0;

	this->pillStack[0] = new PillStack(2);
	this->pillStack[1] = new PillStack(3);

	this->container_placed = false;
}

PillHandler::~PillHandler() {
	
}

void PillHandler::createFakeTrigger() {
	// create a fake trigger for one of the both pillStacks
	this->pillStack[random(0, 2)]->createFakeTrigger();
}

void PillHandler::readData(uint16_t* index) {
	// cache storage position for later data update
	this->data_index = *index;

	// PILL HOLDER A
	this->pillStack[0]->setPillAmountIndex(*index);
	this->pillStack[0]->setPillAmount(EEPROMW->readUInt8(index));

	this->pillStack[0]->setPillRefTimeIndex(*index);
	this->pillStack[0]->setPillRefTime(EEPROMW->readUInt32(index));

	this->pillStack[0]->setPillIntervalIndex(*index);
	this->pillStack[0]->setPillInterval(EEPROMW->readUInt32(index));

	// PILL HOLDER B
	this->pillStack[1]->setPillAmountIndex(*index);
	this->pillStack[1]->setPillAmount(EEPROMW->readUInt8(index));

	this->pillStack[1]->setPillRefTimeIndex(*index);
	this->pillStack[1]->setPillRefTime(EEPROMW->readUInt32(index));

	this->pillStack[1]->setPillIntervalIndex(*index);
	this->pillStack[1]->setPillInterval(EEPROMW->readUInt32(index));
}

void PillHandler::update() {
	this->pillStack[0]->update();
	this->pillStack[1]->update();
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

uint8_t PillHandler::getPillAmount(uint8_t id) {
	return this->pillStack[id]->getPillAmount();
}

uint32_t PillHandler::getPillRefTime(uint8_t id) {
	return this->pillStack[id]->getPillRefTime();
}

uint32_t PillHandler::getPillInterval(uint8_t id) {
	return this->pillStack[id]->getPillInterval();
}
