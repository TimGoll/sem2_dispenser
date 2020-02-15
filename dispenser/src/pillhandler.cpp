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

	// print debug information
	Serial.print("Setting Pill Amount at: ");
	Serial.println(this->pin);
	Serial.print("Amount: ");
	Serial.println(this->pill_amount);
	Serial.print("At storage position: ");
	Serial.println(this->i_pill_amount);
	Serial.println("----------------------------");
}

void PillStack::setPillRefTime(uint32_t time) {
	this->pill_ref_time = time;

	// print debug information
	DateTime time2(this->pill_ref_time);
	char buffer[40];
	sprintf(buffer, "%s - %02d:%02d:%02d", DAYS[time2.dayOfTheWeek()], time2.hour(), time2.minute(), time2.second());

	Serial.print("Setting Reference Time at: ");
	Serial.println(this->pin);
	Serial.print("Time: ");
	Serial.println(buffer);
	Serial.print("At storage position: ");
	Serial.println(this->i_pill_ref_time);
	Serial.println("----------------------------");
}

void PillStack::setPillInterval(uint32_t time) {
	this->pill_interval = time;

	// print debug information
	Serial.print("Setting Pill Interval at: ");
	Serial.println(this->pin);
	Serial.print("Time: ");
	Serial.println(this->pill_interval);
	Serial.print("At storage position: ");
	Serial.println(this->i_pill_interval);
	Serial.println("----------------------------");
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
		tone(5, 1000, 750);

		// print debug information
		Serial.print("Pill is ready at pin: ");
		Serial.println(this->pin);
		Serial.println("----------------------------");
	}

	if (millis() > this->retraction_time) {
		this->servo->write(POS_PILL_IN);
	} else {
		this->servo->write(POS_PILL_OUT);
	}
}

void PillStack::throwPill() {
	if (this->pill_amount == 0) {
		// start beeper
		tone(5, 500, 350);

		return;
	}

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

	// print debug info
	DateTime time(this->pill_ref_time);
	char buffer[40];
	sprintf(buffer, "%s - %02d:%02d:%02d", DAYS[time.dayOfTheWeek()], time.hour(), time.minute(), time.second());

	Serial.print("Pill is thrown at pin: ");
	Serial.println(this->pin);
	Serial.print("Next pill ref time: ");
	Serial.println(buffer);
	Serial.print("Leftover pills: ");
	Serial.println(this->pill_amount);
	Serial.println("----------------------------");
}

void PillStack::refillPills() {
	this->pill_amount = 20;

	// print debug info
	Serial.print("Pillstack is refilled at: ");
	Serial.println(this->pin);
	Serial.print("New pill amount: ");
	Serial.println(this->pill_amount);
	Serial.println("----------------------------");

	// update EEPROM storage

	// we have to cache the index variables here since writeUint modifies it
	uint16_t i_pill_amount_ = this->i_pill_amount;

	EEPROMW->writeUInt32(&i_pill_amount_, this->pill_amount);
}

void PillStack::reset() {
	Serial.println("RESETTING DATA");

	// we have to cache the index variables here since writeUint modifies it
	uint16_t i_pill_amount_ = this->i_pill_amount;
	uint16_t pill_interval_ = this->pill_interval;
	uint16_t i_pill_ref_time_ = this->pill_ref_time;

	uint8_t new_pill_amount = 20;
	uint32_t new_pill_interval = 86400;

	DateTime time = RTC->now();

	uint32_t new_pill_ref_time = time.unixtime();

	EEPROMW->writeUInt32(&i_pill_amount_, new_pill_amount);
	EEPROMW->writeUInt32(&pill_interval_, new_pill_interval);
	EEPROMW->writeUInt32(&i_pill_ref_time_, new_pill_ref_time);

	this->setPillAmount(new_pill_amount);
	this->setPillInterval(new_pill_interval);
	this->setPillRefTime(new_pill_ref_time);
}

void PillStack::createFakeTrigger() {
	Serial.print("Creating fake trigger event at pin: ");
	Serial.println(this->pin);
	Serial.println("----------------------------");

	// get current time
	DateTime time = RTC->now();

	// create a fake pill ready event in 15 seconds
	this->pill_ref_time = time.unixtime() + 10;

	// update EEPROM storage

	// we have to cache the index variables here since writeUint modifies it
	uint16_t i_pill_ref_time_ = this->i_pill_ref_time;

	EEPROMW->writeUInt32(&i_pill_ref_time_, this->pill_ref_time);
}

PillHandler::PillHandler() {
	this->data_index = 0;

	this->pillStack[0] = new PillStack(7);
	this->pillStack[1] = new PillStack(6);

	this->container_placed = false;
}

PillHandler::~PillHandler() {
	
}

void PillHandler::createFakeTrigger() {
	// create a fake trigger for one of the both pillStacks
	this->pillStack[random(0, 2)]->createFakeTrigger();
}

void PillHandler::readData(uint16_t* index) {
	Serial.println("----------------------------");

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
			return;
		}
	}

	// if no pill was thrown and the functions was returned, play sound
	tone(5, 300, 500);
}

static void PillHandler::refillA(PillHandler* self, uint8_t type) {
	self->pillStack[0]->refillPills();
}

static void PillHandler::refillB(PillHandler* self, uint8_t type) {
	self->pillStack[1]->refillPills();
}

static void PillHandler::reset(PillHandler* self, uint8_t type) {
	self->pillStack[0]->reset();
	self->pillStack[1]->reset();
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
