#include "buttons.hpp"

Button::Button(uint8_t pin_number, uint8_t type, uint16_t start_time, uint16_t interval_time) {
	this->pin_number = pin_number;
	this->type = type;
	this->start_time = start_time;
	this->interval_time = interval_time;

	this->last_state = LOW;
	this->last_time = 0;
	this->last_interval_time = 0;

	pinMode(this->pin_number, INPUT_PULLUP);
}

uint8_t Button::getType() {
	return this->type;
}

bool Button::isType(uint8_t type) {
	if (this->type == CHANGE and (type == RISING or type == FALLING))
		return true;

	return type == this->type;
}

uint16_t Button::getStartTime() {
	return this->start_time;
}

uint16_t Button::getIntervalTime() {
	return this->interval_time;
}

bool Button::isNextInterval() {
	// no interval callback timing defined
	if (this->start_time == 0 and this->interval_time == 0)
		return false;

	// initial time and interval time are passed
	if (millis() - this->last_time > this->start_time and millis() - this->last_interval_time > this->interval_time)
		return true;

	return false;
}

void Button::doNextInterval(uint8_t type) {
	this->last_interval_time = millis();

	this->runCallback(type);
}

uint8_t Button::state() {
	// cache inverted pinvalue since buttons use pullup resistor
	uint8_t pin_state = not digitalRead(this->pin_number);

	// DEBOUNCE INPUT
	if (millis() - this->last_time >= 25) {
		// RISING EDGE
		if (pin_state and not this->last_state) {
			this->last_time = millis();
			this->last_state = HIGH;

			return RISING;
		}

		// FALLING EDGE
		if (not pin_state and this->last_state) {
			this->last_time = millis();
			this->last_state = LOW;

			return FALLING;
		}
	}

	// when no edge was detected, return last value
	return this->last_state;
}

ButtonHandler::ButtonHandler() {
	this->button_amount = 0;
}

ButtonHandler::~ButtonHandler() {

}

bool ButtonHandler::addCallback(uint8_t pin_number, uint8_t type, void* callback_scope, callback_t callback) {
	return this->addCallback(pin_number, type, callback_scope, callback, 0, 0);
}

bool ButtonHandler::addCallback(uint8_t pin_number, uint8_t type, void* callback_scope, callback_t callback, uint16_t start_time, uint16_t interval_time) {
	if (this->button_amount == MAX_AMOUNT_BUTTONS)
		return false;

	this->button_list[this->button_amount] = new Button(pin_number, type, start_time, interval_time);
	this->button_list[this->button_amount]->registerCallback(callback_scope, callback);
	this->button_amount++;

	return true;
}

void ButtonHandler::update() {
	//Serial.println("a");delay(20);
	for (uint8_t i = 0; i < this->button_amount; i++) {
		//Serial.println("b");delay(20);
		Button* button = this->button_list[i];
		//Serial.println("c");delay(20);
		uint8_t b_state = button->state();
		//Serial.println("d");delay(20);
		// PRESS / RELEASE MODE
		if (button->isType(b_state)) {
			//Serial.println("d2");delay(20);
			button->runCallback(b_state);
			//Serial.println("d3");delay(20);
		}
		//Serial.println("e");delay(20);
		// HOLD MODE
		if (b_state == HIGH and button->isNextInterval()) {
			button->doNextInterval(HIGH);
		}
		//Serial.println("f");delay(20);
	}
}
