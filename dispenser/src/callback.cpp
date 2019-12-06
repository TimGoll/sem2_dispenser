#include "callback.hpp"

Callback::Callback() {

}

Callback::~Callback() {

}

void Callback::registerCallback(void* scope, callback_t function) {
	this->callback_scope = scope;
	this->callback_function = function;
}

void Callback::runCallback(uint8_t value) {
	this->callback_function(this->callback_scope, value);
}