#include "callback.hpp"

Callback::Callback(uint8_t max_amount) {
	this->cb__amount = 0;
	this->cb__max_amount = max_amount;
	this->cb__list = new CallbackInternal*[max_amount];
}

Callback::~Callback() {
	if (this->cb__amount == 0)
		return;

	for (uint8_t i = 0; i < this->cb__amount; i++)
		delete this->cb__list[i];
}

bool Callback::registerCallback(void* scope, callback_t function) {
	if (this->cb__amount == this->cb__max_amount)
		return false;

	this->cb__list[this->cb__amount] = new CallbackInternal(scope, function);
	this->cb__amount++;

	return true;
}