#include "callback.hpp"

Callback::Callback() {
	this->has_callback = false;
}

Callback::~Callback() {

}

void Callback::registerCallback(void* scope, callback_t function) {
	this->callback_scope = scope;
	this->callback_function = function;

	this->has_callback = true;
}

void Callback::runCallback(uint8_t value) {
	if (not this->has_callback)
		return;

	this->callback_function(this->callback_scope, value);
}