#ifndef CALLBACK_HPP
#define CALLBACK_HPP

#include <Arduino.h>

//defining a type for the callback function
typedef void (*callback_t)(void*, uint8_t);

namespace {
	class CallbackInternal {
		public:
			CallbackInternal(void* scope, callback_t function);
			~CallbackInternal();

			/**************************************************************************/
			/*!
				@brief	Runs the passed callback function with a parameter
				@param value The value to be passed
			*/
			/**************************************************************************/
			template <typename PARAM>
			void runCallbackInternal(PARAM value);

		private:
			callback_t callback_function;
			void* callback_scope;
	};


	CallbackInternal::CallbackInternal(void* scope, callback_t function) {
		this->callback_scope = scope;
		this->callback_function = function;
	}

	CallbackInternal::~CallbackInternal() {

	}

	template <typename PARAM>
	void CallbackInternal::runCallbackInternal(PARAM value) {
		if (this->callback_function == nullptr or this->callback_scope == nullptr)
			return;

		this->callback_function(this->callback_scope, value);
	}
};

class Callback {
	public:
		Callback(uint8_t max_amount);
		~Callback();

		/**************************************************************************/
		/*!
			@brief	Registers a callback function to be called at a later point
			@param scope The scope which should be passed on to the registered function
			@param callback The function to be called
		*/
		/**************************************************************************/
		bool registerCallback(void* scope, callback_t function);

		template <typename PARAM>
		bool runCallback(PARAM value);

		template <typename PARAM>
		bool runCallback(PARAM value, uint8_t id);

	private:
		CallbackInternal** cb__list;
		uint8_t cb__amount;
		uint8_t cb__max_amount;
};

template <typename PARAM>
bool Callback::runCallback(PARAM value) {
	if (this->cb__amount == 0)
		return false;

	for (uint8_t i = 0; i < this->cb__amount; i++)
		this->cb__list[i]->runCallbackInternal(value);

	return true;
}

template <typename PARAM>
bool Callback::runCallback(PARAM value, uint8_t id) {
	if (this->cb__amount == 0)
		return false;

	if (id >= this->cb__amount)
		return false;

	this->cb__list[id]->runCallbackInternal(value);

	return true;
}

#endif
