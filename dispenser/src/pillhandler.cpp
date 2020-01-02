#include "pillhandler.hpp"

PillStack::PillStack(uint8_t max_amount) {

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
	self->data_prefix = "PILL";
}

PillHandler::~PillHandler() {

}

static void PillStack::deserializeData(PillHandler* self, DataPoints* data) {
	if (not data->isPrefix(self->data_prefix))
		return;

	while (true) {
		uint8_t type[4];
		uint8_t data[8];

		// try reading data and stop when no valid data is available
		if (not this->dataPoints->getNextDataPoint(type, data))
			break;

		// TODO: deserialize data and set this data to the PillStack objects
		// they should be created by default and not dynamically
	}
}
