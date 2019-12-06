#include "buttons.hpp"

Button::Button(uint8_t pin_number, uint8_t type, callback_t callback, void* callback_scope, uint16_t start_time, uint16_t interval_time) {
	this->pin_number = pin_number;
	this->type = type;
	this->callback = callback;
	this->callback_scope = callback_scope;
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

void Button::runCallback(uint8_t type) {
	this->callback(this->callback_scope, type);
}

uint16_t Button::GetStartTime() {
	return this->start_time;
}

uint16_t Button::GetIntervalTime() {
	return this->interval_time;
}

bool Button::isNextInterval() {
	// no interval callback timing defined
	if (start_time == 0 and interval_time == 0)
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

bool ButtonHandler::addCallback(uint8_t pin_number, uint8_t type, callback_t callback, void* callback_scope) {
	return this->addCallback(pin_number, type, callback, callback_scope, 0, 0);
}

bool ButtonHandler::addCallback(uint8_t pin_number, uint8_t type, callback_t callback, void* callback_scope, uint16_t start_time, uint16_t interval_time) {
	if (this->button_amount == MAX_AMOUNT_BUTTONS)
		return false;

	this->button_list[this->button_amount] = new Button(pin_number, type, callback, callback_scope, start_time, interval_time);
	this->button_amount++;

	return true;
}

void ButtonHandler::update() {
	for (uint8_t i = 0; i < this->button_amount; i++) {
		Button* button = this->button_list[i];

		uint8_t b_state = button->state();

		// PRESS / RELEASE MODE
		if (button->isType(b_state)) {
			button->runCallback(b_state);
		}

		// HOLD MODE
		if (b_state == HIGH and button->isNextInterval()) {
			button->doNextInterval(HIGH);
		}
	}
}
