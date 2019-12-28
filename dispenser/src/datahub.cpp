#include "datahub.hpp"

DataPoints::DataPoints(uint8_t max_data_points) {
	this->string_pos = 0;
	this->line_size = max_data_points * 2 * 8;

	this->data_line = new uint8_t[this->line_size];
	this->prefix = new uint8_t[5];
}

DataPoints::~DataPoints() {
	delete[] this->data_line;
	delete[] this->prefix;
}

void DataPoints::getPrefix(uint8_t* prefix) {
	strcpy(prefix, this->prefix);
}

void DataPoints::setPrefix(uint8_t* prefix) {
	strcpy(this->prefix, prefix);

	this->string_pos = 0; //TODO remove
}

bool DataPoints::getNextDataPoint(uint8_t* type, uint8_t* data) {
	bool is_type = true;
	uint8_t type_pos = 0;
	uint8_t data_pos = 0;

	while (true) {
		// make sure data line is no empty string
		if (this->data_line[this->string_pos] == '\0')
			return false;

		// copy type tag
		if (is_type) {
			type[type_pos] = this->data_line[this->string_pos];
			type_pos++;

			if (type_pos == 3) {
				type[type_pos] = '\0';

				is_type = false;
			}
		} else {
			data[data_pos] = this->data_line[this->string_pos];
			data_pos++;

			if (this->data_line[this->string_pos + 1] == '\0' or this->data_line[this->string_pos + 1] == '#') {
				data[data_pos] = '\0';
				this->string_pos+=2;

				return true;
			}
		}

		this->string_pos++;
	}
}

void DataPoints::setNextDataPoint(uint8_t* type, uint8_t* data) {
	for (uint8_t i = 0; type[i] != '\0'; i++) {
		this->data_line[this->string_pos] = type[i];
		this->string_pos++;
	}

	for (uint8_t i = 0; data[i] != '\0'; i++) {
		this->data_line[this->string_pos] = data[i];
		this->string_pos++;
	}

	this->data_line[this->string_pos] = '#';
	this->string_pos++;
	this->data_line[this->string_pos] = '\0';
}

void DataPoints::getDataLine(uint8_t* line) {
	strcpy(line, this->prefix);
	strcat(line, this->data_line);
}

void DataPoints::setDataLine(uint8_t* line) {
	for (uint8_t i = 0; i < this->line_size + 4; i++) {
		if (i < 4) {
			this->prefix[i] = line[i];
		} else {
			this->data_line[i - 4] = line[i];
		}

		if (line[i] == '\0') {
			this->prefix[4] = '\0';
			this->data_line[i - 4] = '\0';

			break;
		}
	}
}

DataHub::DataHub() : Callback(8) {
	this->card = new Sd2Card();
	this->volume = new SdVolume();
	this->file_root = new SdFile();

	this->card->init(SPI_HALF_SPEED, CHIP_SELECT);

	this->volume->init(this->card);

	this->file_root->openRoot(this->volume);

	//open or create config file
}

DataHub::~DataHub() {

}

void DataHub::readFromSD() {
	// read one line at a time and pass data as DataPoints object to the reader via callback
}

void DataHub::writeToSD() {
	// write one line at a time and call callbacks individually and pass an empty data object each time
}
